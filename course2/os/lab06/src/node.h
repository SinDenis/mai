#ifndef NODE_H
#define NODE_H

#include <zmq.hpp>
#include <map>
#include <set>
#include <cstdint>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <numeric>
#include "message.h"
#include <utility>
#include <algorithm>
#include <signal.h>
#include <mutex>

const uint16_t BEGIN_PORT = 30000;
const uint32_t MAX_DELAY = 5000;
const uint32_t PING_DELAY = 4000;
const char* IN_URL = "tcp://*:";
const char* OUT_URL = "tcp://localhost:";
const char* MANAGE_OUT = "inproc://output";
const char* MANAGE_CONTROLLER = "inproc://controller";

std::string GetInAdrSock(uint16_t port) {
	return IN_URL + std::to_string(port);
}

std::string GetOutAdrSock(uint16_t port) {
	return OUT_URL + std::to_string(port);
}

uint16_t TakePort(zmq::socket_t& sock)
{
	uint16_t port = BEGIN_PORT;
	while (true) {
		try {
			sock.bind(GetInAdrSock(port));
			break;
		} catch (const zmq::error_t& err) {
			++port;
		}
	}
	return port;
}

struct NodeData {
	NodeData(pid_t pid, uint16_t recvPort, uint16_t regPort):
		Pid(pid), ReceiverPort(recvPort), RegisterPort(regPort)
	{}
	pid_t Pid;
	uint16_t ReceiverPort;
	uint16_t RegisterPort;
};

class BaseNode {
public:
	explicit BaseNode(int32_t id):
		Id(id), Port(BEGIN_PORT), Children(), Ctx(1), Receiver(Ctx, zmq::socket_type::rep)
	{
		TakePort();
	}

	~BaseNode() = default;

	pid_t CreateChild(int32_t id, uint16_t registerPort) {
		pid_t child = fork();
		if (child == 0) {
			using std::to_string;
			execl("calc_node", to_string(id).c_str(), to_string(Id).c_str(), to_string(Port).c_str(), to_string(registerPort).c_str(), NULL);
		}
		return child;
	}

protected:
	int32_t Id;
	uint16_t Port;
	std::map<int32_t, NodeData> Children;
	zmq::context_t Ctx;
	zmq::socket_t Receiver;

	void TakePort() {
		Port = ::TakePort(Receiver);
	}
};

class ManageNode: public BaseNode {
public:
	ManageNode(): BaseNode(-1), CreateDone(true) {
		NodeIds.insert(-1);
	}

	void Run() {
		std::thread([this]() { InputWorker(); }).detach();
		std::thread([this]() { Controller(); }).detach();
		std::thread([this]() { RegisterWorker(); }).detach();
		OutputWorker();
	}

private:
	std::set<int32_t > NodeIds;
	std::mutex CreateBlock;
	std::condition_variable CreateCond;
	bool CreateDone;

	void InputWorker() {
		zmq::socket_t sender(Ctx, zmq::socket_type::push);
		sender.connect(MANAGE_CONTROLLER);
		Message inMsg;
		std::string cmd;

		while (true) {
			std::cin >> cmd;
			if (std::cin.eof())
				break;
			if (cmd == "create") {
				inMsg = CreateParse();
				if (CheckBadInput()) {
					std::cout << "Wrong input" << std::endl;
					continue;
				}
				inMsg.Send(sender);
			} else if (cmd == "exec") {
				inMsg = ExecParse();
				if (CheckBadInput()) {
					std::cout << "Wrong input" << std::endl;
					continue;
				}
				inMsg.Send(sender);
			} else if (cmd == "remove") {
				inMsg = RemoveParse();
				if (CheckBadInput()) {
					std::cout << "Wrong input" << std::endl;
					continue;
				}
				inMsg.Send(sender);
			} else if (cmd == "ping") {
				inMsg = PingParse();
				if (CheckBadInput()) {
					std::cout << "Wrong input" << std::endl;
					continue;
				}
				inMsg.Send(sender);
			} else if (cmd == "quit") {
				inMsg = QuitParse();
				inMsg.Send(sender);
				break;
			} else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<int>::max(), '\n');
				std::cout << "Wrong input" << std::endl;
			}
		}
	}

	void Controller() {
		zmq::socket_t receiver(Ctx, zmq::socket_type::pull);
		zmq::socket_t sender(Ctx, zmq::socket_type::push);
		Message msgOut;
		receiver.bind(MANAGE_CONTROLLER);
		sender.connect(MANAGE_OUT);
		bool quit{false};

		while (!quit) {
			Message msgIn;
			msgIn.Recv(receiver);
			switch (msgIn.Type) {
				case MsgType::createReq:
					msgOut = CreateHandle(std::move(msgIn));
					msgOut.Send(sender);
					break;
				case MsgType::execReq:
					msgOut = ExecHandle(std::move(msgIn));
					msgOut.Send(sender);
					break;
				case MsgType::pingReq:
					msgOut = PingHandle(std::move(msgIn));
					msgOut.Send(sender);
					break;
				case MsgType::quit:
					msgOut = QuitHandle(std::move(msgIn), quit);
					msgOut.Send(sender);
					break;
				case MsgType::removeReq:
					msgOut = RemoveHandle(std::move(msgIn));
					msgOut.Send(sender);
				default:
					break;
			}
		}
	}

	void RegisterWorker() {
		Message msgIn;
		Message msgOut;
		while (true) {
			msgIn.Recv(Receiver);
			if (msgIn.Type == MsgType::quit) {
				msgIn.Send(Receiver);
				break;
			}
			msgOut = RegisterHandle(msgIn);
			go();
			msgOut.Send(Receiver);
		}
	}

	void go() {
		std::unique_lock<std::mutex> lck(CreateBlock);
		CreateDone = true;
		CreateCond.notify_one();
	}

	void OutputWorker() {
		zmq::socket_t receiver(Ctx, zmq::socket_type::pull);
		receiver.bind(MANAGE_OUT);
		Message msgIn;
		bool quit{false};

		while (!quit) {
			msgIn.Recv(receiver);
			switch(msgIn.Type) {
				case MsgType::createRes:
					CreateOutputSuccess(msgIn);
					break;
				case MsgType::execRes:
					ExecOutputSuccess(msgIn);
					break;
				case MsgType::pingRes:
					PingOutputSuccess(msgIn);
					break;
				case MsgType::quit:
					QuitOutputHandle(msgIn, quit);
					break;
				case MsgType::removeRes:
					RemoveOutputHandle(msgIn);
					break;
				case MsgType::createFail:
				case MsgType::execFail:
				case MsgType::removeFail:
				case MsgType::pingFail:
					OutputFail(msgIn);
					break;
				default:
					break;
			}
		}
	}

	Message RegisterHandle(Message& msg) {
		auto body = (unsigned char*)msg.Body;
		pid_t pid = *(pid_t*)(body);
		uint16_t recvPort = *(uint16_t*)(body + sizeof(pid_t));
		uint16_t regPort = *(uint16_t*)(body + sizeof(pid_t) + sizeof(uint16_t));
		Children.emplace(msg.SndId, NodeData(pid, recvPort, regPort));
		return Message(MsgType::relateRes, Id, msg.SndId);
	}

	Message QuitParse() {
		return Message(MsgType::quit, Id, Id);
	}

	Message PingParse() {
		int32_t id;
		std::cin >> id;
		return Message(MsgType::pingReq, Id, Id, sizeof(id), (void*)&id);
	}

	Message ExecParse() {
		std::vector<int32_t > data(2);
		std::cin >> data[0] >> data[1];
		data.resize(data[1] + 2);
		for (int i = 2; i < data.size(); ++i) {
			std::cin >> data[i];
		}
		return Message(MsgType::execReq, Id, Id, data.size() * sizeof(int32_t), (void*)data.data());
	}

	Message CreateParse() {
		int32_t data[2];
		std::cin >> data[0] >> data[1];
		return Message(MsgType::createReq, Id, Id, sizeof(data), (void*)&data);
	}

	Message RemoveParse() {
		int32_t id;
		std::cin >> id;
		return Message(MsgType::removeReq, Id, Id, sizeof(id), (void*)&id);
	}

	bool CheckBadInput() {
		if (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			return true;
		}
		return false;
	}

	void CreateOutputSuccess(const Message& msg) {
		std::cout << "Ok: " << *(int32_t *)msg.Body << std::endl;
	}

	void ExecOutputSuccess(const Message& msg) {
		auto data = (int32_t *)msg.Body;
		std::cout << "Ok: " << data[0] << ": " << data[1] << std::endl;
	}

	void PingOutputSuccess(const Message& msg) {
		auto pingRes = *(int32_t *)msg.Body;
		std::cout << "Ok: " << pingRes << std::endl;
	}

	void OutputFail(const Message& msg) {
		std::cout << (char*)msg.Body << std::endl;
	}

	void QuitOutputHandle(Message& msg, bool& quit) {
		quit = true;
	}

	void RemoveOutputHandle(Message& msg) {
		std::cout << "Ok" << std::endl;
	}

	Message CreateHandle(Message&& msg) {
		int32_t id = ((int32_t*)msg.Body)[0];
		int32_t parId = ((int32_t*)msg.Body)[1];
		return CreateHandleImpl(id, parId);
	}

	Message ExecHandle(Message&& msg) {
		std::vector<int32_t > data((int32_t*)msg.Body, (int32_t *)msg.Body + msg.BodySize / sizeof(int32_t));
		if (NodeIds.find(data[0]) == NodeIds.end()) {
			return Message(MsgType::execFail, Id, Id, "Error: Node is not found");
		}
		for (auto child: Children) {
			zmq::socket_t req(Ctx, zmq::socket_type::req);
			req.connect(GetOutAdrSock(child.second.ReceiverPort));
			msg.SetHeader(Id, child.first);
			msg.Send(req);
			Message resMsg;
			if (!resMsg.Recv(req, std::chrono::milliseconds(MAX_DELAY))) {
				req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (resMsg.Type == MsgType::execRes) {
				resMsg.SetHeader(Id, Id);
				return resMsg;
			}
			req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		return Message(MsgType::execFail, Id, Id, "Error: Node is unavailable");
	}

	Message PingHandle(Message&& msg) {
		auto id = *(int32_t*)msg.Body;
		if (NodeIds.find(id) == NodeIds.end()) {
			return Message(MsgType::pingFail, Id, Id, "Error: Not found");
		}
		for (auto child: Children) {
			zmq::socket_t reqSock(Ctx, zmq::socket_type::req);
			reqSock.connect(GetOutAdrSock(child.second.ReceiverPort));
			msg.SetHeader(Id, child.first);
			msg.Send(reqSock);
			Message res;
			if (!res.Recv(reqSock, std::chrono::milliseconds(PING_DELAY))) {
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (res.Type == MsgType::pingRes && *(int32_t *)res.Body) {
				res.SetHeader(Id, Id);
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				return res;
			}
			reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		int32_t pingRes = 0;
		return Message(MsgType::pingRes, Id, Id, sizeof(pingRes), (void*)&pingRes);
	}

	Message QuitHandle(Message&& msg, bool& quit) {
		zmq::socket_t quitRegister(Ctx, zmq::socket_type::req);
		quitRegister.connect(GetOutAdrSock(Port));
		Message msgQuit(MsgType::quit, Id, Id);
		Message tmp;
		msgQuit.Send(quitRegister);
		tmp.Recv(quitRegister);
		quitRegister.disconnect(GetOutAdrSock(Port));
		std::cout << "Delete all nodes" << std::endl;
		for (auto child: Children) {
			zmq::socket_t req(Ctx, zmq::socket_type::req);
			req.connect(GetOutAdrSock(child.second.ReceiverPort));
			msgQuit.Send(req);
			Message msgQuitRes;
			msgQuitRes.Recv(req, std::chrono::milliseconds(MAX_DELAY));
			req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		std::cout << "All nodes is killed" << std::endl;
		quit = true;
		return msgQuit;
	}

	Message RemoveHandle(Message&& msg) {
		auto id = *(int32_t *)msg.Body;
		if (NodeIds.find(id) == NodeIds.end()) {
			return Message(MsgType::removeFail, Id, Id, "Error: Node is not found");
		}
		for (auto child : Children) {
			zmq::socket_t reqSock(Ctx, zmq::socket_type::req);
			reqSock.connect(GetOutAdrSock(child.second.ReceiverPort));
			msg.SetHeader(Id, child.first);
			msg.Send(reqSock);
			Message msgRes;
			if (!msgRes.Recv(reqSock, std::chrono::milliseconds(MAX_DELAY))) {
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (msgRes.Type == MsgType::removeNow) {
				kill(child.second.Pid, SIGTERM);
				Children.erase(id);
				NodeIds.erase(id);
				msgRes.Type = MsgType::removeRes;
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				return msgRes;
			} else if (msgRes.Type == MsgType::removeRes) {
				NodeIds.erase(id);
				msgRes.SetHeader(Id, Id);
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				return msgRes;
			}
			reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		return Message(MsgType::removeFail, Id, Id, "Error: Node is unavailable");
	}

	Message CreateHandleImpl(int32_t id, int32_t parId) {
		if (NodeIds.find(id) != NodeIds.end())
			return Message(MsgType::createFail, Id, Id, "Error: Already exists");
		if (NodeIds.find(parId) == NodeIds.end())
			return Message(MsgType::createFail, Id, Id, "Error: Parent not found");
		if (parId == Id) {
			pid_t pid = CreateChildImpl(id, Port);
			std::unique_lock<std::mutex> lck(CreateBlock);
			CreateDone = false;
			while (!CreateDone) {
				CreateCond.wait(lck);
			}
			return Message(MsgType::createRes, Id, Id, sizeof(pid), (void*)&pid);
		}
		for (auto child: this->Children) {
			zmq::socket_t reqSock(Ctx, zmq::socket_type::req);
			reqSock.connect(GetOutAdrSock(child.second.ReceiverPort));
			int32_t data[] = {id, parId};
			Message req(MsgType::createReq, Id, child.first, sizeof(data), data);
			Message res;
			req.Send(reqSock);
			if (!res.Recv(reqSock, std::chrono::milliseconds(MAX_DELAY))) {
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (res.Type == MsgType::createRes) {
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				NodeIds.insert(id);
				return Message(MsgType::createRes, Id, Id, res.BodySize, res.Body);
			}
			reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		return Message(MsgType::createFail, Id, Id, "Error: Parent is unavailable");
	}

	pid_t CreateChildImpl(int32_t id, uint16_t registerPort) {
		pid_t pid = CreateChild(id, registerPort);
		NodeIds.insert(id);
		return pid;
	}
};

class CalcNode : public BaseNode {
public:
	CalcNode(int32_t id, int32_t parentId, uint16_t parentPort, uint16_t registerPort):
			BaseNode(id), ParentId(parentId), ParentPort(parentPort), ParentRegisterPort(registerPort), Registrator(Ctx, zmq::socket_type::rep)
	{
		TakePort();
		RegisterPort = ::TakePort(Registrator);
		SelfRelate(ParentRegisterPort);
	}

	void Run() {
		std::thread([this]() {
			RegisterWorker();
		}).detach();
		while (true) {
			Message msgOut;
			Message msgIn;
			msgIn.Recv(Receiver);
			switch (msgIn.Type) {
				case MsgType::createReq:
					msgOut = CreateHandle(std::move(msgIn));
					msgOut.Send(Receiver);
					break;
				case MsgType::execReq:
					msgOut = ExecHandle(std::move(msgIn));
					msgOut.Send(Receiver);
					break;
				case MsgType::pingReq:
					msgOut = PingHandle(std::move(msgIn));
					msgOut.Send(Receiver);
					break;
				case MsgType::removeReq:
					msgOut = RemoveHandle(std::move(msgIn));
					msgOut.Send(Receiver);
					break;
				case MsgType::quit:
					msgOut = QuitHandle(std::move(msgIn));
					msgOut.Send(Receiver);
					exit(EXIT_SUCCESS);
				default:
					break;
			}
		}
	}

private:
	int32_t ParentId;
	uint16_t ParentPort;
	uint16_t ParentRegisterPort;
	zmq::socket_t Registrator;
	uint16_t RegisterPort;
	std::mutex CreateBlock;
	std::condition_variable CreateCond;
	bool CreateDone;

	void RegisterWorker() {
		Message msgIn;
		Message msgOut;
		while (true) {
			msgIn.Recv(Registrator);
			if (msgIn.Type == MsgType::quit) {
				msgIn.Send(Registrator);
				break;
			}
			msgOut = RegisterHandle(std::move(msgIn));
			go();
			msgOut.Send(Registrator);
		}
	}

	void go() {
		std::unique_lock<std::mutex> lck(CreateBlock);
		CreateDone = true;
		CreateCond.notify_one();
	}

	void* CreateRelateBody(pid_t pid, uint16_t recvPort, uint16_t regPort) {
		auto mem = new unsigned char[sizeof(pid_t) + 2 * sizeof(uint16_t)];
		auto memPid = (pid_t*)(mem);
		auto memRecvPort = (uint16_t *)(mem + sizeof(pid_t));
		auto memRegPort = (uint16_t *)(mem + sizeof(pid_t) + sizeof(uint16_t));
		*memPid = pid;
		*memRecvPort = recvPort;
		*memRegPort = regPort;
		return (void*)mem;
	}

	void SelfRelate(uint16_t registerPort) {
		zmq::socket_t sender(Ctx, zmq::socket_type::req);
		sender.connect(GetOutAdrSock(registerPort));
		void* relBody = CreateRelateBody(getpid(), Port, RegisterPort);
		Message msg(MsgType::relateReq, Id, ParentId, 2 * sizeof(uint16_t) + sizeof(pid_t), relBody);
		msg.Send(sender);
		msg.Recv(sender);
		sender.disconnect(GetOutAdrSock(registerPort));
		delete [] (unsigned char*)relBody;
		relBody = nullptr;
	}

	Message RegisterHandle(Message&& msg) {
		auto body = (unsigned char*)msg.Body;
		pid_t pid = *(pid_t*)(body);
		uint16_t recvPort = *(uint16_t*)(body + sizeof(pid_t));
		uint16_t regPort = *(uint16_t*)(body + sizeof(pid_t) + sizeof(uint16_t));
		Children.emplace(msg.SndId, NodeData(pid, recvPort, regPort));
		return Message(MsgType::relateRes, Id, msg.SndId);
	}

	Message CreateHandle(Message&& msg) {
		int32_t id = ((int32_t*)msg.Body)[0];
		int32_t parId = ((int32_t*)msg.Body)[1];
		return CreateHandleImpl(id, parId, msg.SndId);
	}

	Message CreateHandleImpl(int32_t id, int32_t parId, int32_t sndId) {
		if (parId == Id) {
			pid_t pid = CreateChild(id, RegisterPort);
			std::unique_lock<std::mutex> lck(CreateBlock);
			CreateDone = false;
			while (!CreateDone) {
				CreateCond.wait(lck);
			}
			return Message(MsgType::createRes, Id, ParentId, sizeof(pid), &pid);
		} else if (Children.empty())
			return Message(MsgType::createFail, Id, ParentId);
		int32_t pid = -1;
		for (auto child: Children) {
			zmq::socket_t sender(Ctx, zmq::socket_type::req);
			sender.connect(GetOutAdrSock(child.second.ReceiverPort));
			int32_t data[] = {id, parId};
			Message req(MsgType::createReq, Id, child.first, sizeof(data), (void*)data);
			Message res;
			req.Send(sender);
			if (!res.Recv(sender, std::chrono::milliseconds(MAX_DELAY))) {
				sender.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (res.Type == MsgType::createRes) {
				pid = *(int32_t *)res.Body;
				sender.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				break;
			}
			sender.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		if (pid == -1)
			return Message(MsgType::createFail, Id, sndId);
		return Message(MsgType::createRes, Id, sndId, sizeof(pid), &pid);
	}

	Message PingHandle(Message&& msg) {
		int32_t id = *(int32_t *)msg.Body;
		if (id == Id) {
			int32_t pingRes = 1;
			return Message(MsgType::pingRes, Id, ParentId, sizeof(pingRes), (void *) &pingRes);
		}
		for (auto child: Children) {
			zmq::socket_t req(Ctx, zmq::socket_type::req);
			req.connect(GetOutAdrSock(child.second.ReceiverPort));
			msg.SetHeader(Id, child.first);
			msg.Send(req);
			Message res;
			if (!res.Recv(req, std::chrono::milliseconds(PING_DELAY))) {
				req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (res.Type == MsgType::pingRes && *(int32_t *)res.Body) {
				res.SetHeader(Id, ParentId);
				return res;
			}
			req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		int32_t pingRes = 0;
		return Message(MsgType::pingRes, Id, Id, sizeof(pingRes), (void*)&pingRes);
	}

	Message ExecHandle(Message&& msg) {
		std::vector<int32_t > vec((int32_t*)msg.Body, (int32_t *)msg.Body + msg.BodySize / sizeof(int32_t));
		if (vec[0] == Id) {
			vec[1] = std::accumulate(vec.begin() + 2, vec.end(), 0);
			return Message(MsgType::execRes, Id, ParentId, sizeof(int32_t) * 2, (void*)vec.data());
		}
		for (auto child : this->Children) {
			zmq::socket_t req(Ctx, zmq::socket_type::req);
			req.connect(GetOutAdrSock(child.second.ReceiverPort));
			msg.SetHeader(Id, child.first);
			msg.Send(req);
			Message resMsg;
			if (!resMsg.Recv(req, std::chrono::milliseconds(MAX_DELAY))) {
				req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (resMsg.Type == MsgType::execRes) {
				resMsg.SetHeader(Id, ParentId);
				req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				return resMsg;

			}
			req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		return Message(MsgType::execFail, Id, ParentId, "Error: Node is unavailable");
	}

	Message QuitHandle(Message&& msg) {
		Message msgQuit(MsgType::quit, Id, Id);
		for (auto child: Children) {
			zmq::socket_t req(Ctx, zmq::socket_type::req);
			req.connect(GetOutAdrSock(child.second.ReceiverPort));
			msgQuit.Send(req);
			Message msgQuitRes;
			msgQuitRes.Recv(req, std::chrono::milliseconds(MAX_DELAY));
			req.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		zmq::socket_t req(Ctx, zmq::socket_type::req);
		req.connect(GetOutAdrSock(RegisterPort));
		msgQuit.Send(req);
		msgQuit.Recv(req);
		req.disconnect(GetOutAdrSock(RegisterPort));
		return msgQuit;
	}

	Message RemoveHandle(Message&& msg) {
		auto id = *(int32_t *)msg.Body;
		if (id == Id) {
			return RemoveImpl();
		}
		for (auto child : Children) {
			zmq::socket_t reqSock(Ctx, zmq::socket_type::req);
			reqSock.connect(GetOutAdrSock(child.second.ReceiverPort));
			msg.SetHeader(Id, child.first);
			msg.Send(reqSock);
			Message msgRes;
			if (!msgRes.Recv(reqSock, std::chrono::milliseconds(MAX_DELAY))) {
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				continue;
			}
			if (msgRes.Type == MsgType::removeNow) {
				Children.erase(id);
				kill(child.second.Pid, SIGTERM);
				msgRes.Type = MsgType::removeRes;
				msgRes.SetHeader(Id, ParentId);
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				return msgRes;
			} else if (msgRes.Type == MsgType::removeRes) {
				msgRes.SetHeader(Id, ParentId);
				reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
				return msgRes;
			}
			reqSock.disconnect(GetOutAdrSock(child.second.ReceiverPort));
		}
		return Message(MsgType::removeFail, Id, ParentId, "Error: Node is unavailable");
	}

	Message RemoveImpl() {
		if (Children.empty())
			return Message(MsgType::removeNow, Id, ParentId);
		AddNewParent();
		RebindChildren();
		return Message(MsgType::removeNow, Id, ParentId);
	}

	void AddNewParent() {
		auto firstChild = *Children.begin();
		zmq::socket_t req(Ctx, zmq::socket_type::req);
		req.connect(GetOutAdrSock(ParentRegisterPort));
		void* msgBody = CreateRelateBody(firstChild.second.Pid, firstChild.second.ReceiverPort, firstChild.second.RegisterPort);
		Message msg(MsgType::relateReq, firstChild.first, ParentId, 2 * sizeof(uint16_t) + sizeof(pid_t), msgBody);
		msg.Send(req);
		msg.Recv(req);
		delete [] (unsigned char*)msgBody;
	}

	void RebindChildren() {
		auto firstChild = *Children.begin();
		zmq::socket_t req(Ctx, zmq::socket_type::req);
		req.connect(GetOutAdrSock(firstChild.second.RegisterPort));
		for (auto it = ++Children.begin(); it != Children.end(); ++it)
		{
			void* msgBody = CreateRelateBody(it->second.Pid, it->second.ReceiverPort, it->second.RegisterPort);
			Message msg(MsgType::relateReq, it->first, firstChild.first, 2 * sizeof(uint16_t) + sizeof(pid_t), msgBody);
			msg.Send(req);
			msg.Recv(req);
			delete [] (unsigned char*)msgBody;
		}
	}
};

#endif
