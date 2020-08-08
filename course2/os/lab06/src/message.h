#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <utility>
#include <thread>
#include <zmq.hpp>
#include <cstdlib>
#include <string.h>
#include <algorithm>

enum class MsgType : int {
	createReq,
	createRes,
	createFail,
	execReq,
	execRes,
	execFail,
	pingReq,
	pingRes,
	pingFail,
	removeReq,
	removeRes,
	removeNow,
	removeFail,
	relateReq,
	relateRes,
	quit,
	none
};

class Message {
public:

	Message(MsgType type, int32_t sndId, int32_t recvId, size_t bodySize, void* body):
		Type(type), SndId(sndId), RecvId(recvId), BodySize(bodySize), Body(nullptr)
	{
		if (BodySize > 0) {
			Body = operator new(BodySize);
			memcpy(Body, body, BodySize);
		}
	}

	Message(MsgType type, int32_t sndId, int32_t recvId):
		Message(type, sndId, recvId, 0, nullptr)
	{
	}

	Message(MsgType type, int32_t sndId, int32_t recvId, const char* str):
		Message(type, sndId, recvId)
	{
		BodySize = strlen(str) + 1;
		if (BodySize > 0) {
			Body = operator new(BodySize);
			memcpy(Body, (void*)str, BodySize);
		}
	}

	Message():
		Message(MsgType::none, -2, -2)
	{
	}

	~Message() {
		operator delete(Body);
		Body = nullptr;
	}

	Message(const Message& msg) {
		Type = msg.Type;
		SndId = msg.SndId;
		RecvId = msg.RecvId;
		BodySize = msg.BodySize;
		Body = nullptr;
		if (BodySize > 0) {
			Body = operator new(BodySize);
			memcpy(Body, msg.Body, BodySize);
		}
	}

	Message&operator= (const Message& msg) {
		operator delete(Body);
		Body = nullptr;
		Type = msg.Type;
		SndId = msg.SndId;
		RecvId = msg.RecvId;
		BodySize = msg.BodySize;
		if (BodySize > 0) {
			Body = operator new(BodySize);
			memcpy(Body, msg.Body, BodySize);
		}
		return *this;
	}

	Message(Message&& msg) noexcept {
		Type = msg.Type;
		SndId = msg.SndId;
		RecvId = msg.RecvId;
		BodySize = msg.BodySize;
		Body = msg.Body;
		msg.Body = nullptr;
	}

	Message& operator= (Message&& msg) noexcept {
		operator delete(Body);
		Body = nullptr;
		Type = msg.Type;
		SndId = msg.SndId;
		RecvId = msg.RecvId;
		BodySize = msg.BodySize;
		Body = msg.Body;
		msg.Body = nullptr;
		return *this;
	}

	void Send(zmq::socket_t& sock) {
		int headerData[] = {SndId, RecvId, BodySize};
		zmq::message_t type(&Type, sizeof(Type));
		zmq::message_t header(&headerData, sizeof(headerData));
		zmq::message_t body(Body, BodySize);
		sock.send(type, zmq::send_flags::sndmore);
		sock.send(header, zmq::send_flags::sndmore);
		sock.send(body, zmq::send_flags::none);
	}

	void Recv(zmq::socket_t& sock) {
		zmq::message_t ZType;
		zmq::message_t ZHeader;
		zmq::message_t ZBody;
		for (bool t = false; !t; t = GetMsgFromSock(sock, ZType, ZHeader, ZBody)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		Type = *(MsgType*)ZType.data();
		SndId = ((int32_t*)ZHeader.data())[0];
		RecvId = ((int32_t*)ZHeader.data())[1];
		BodySize = ((int32_t*)ZHeader.data())[2];
		operator delete(Body);
		Body = nullptr;
		if (BodySize > 0) {
			Body = operator new(BodySize);
			memcpy(Body, ZBody.data(), BodySize);
		}
	}

	bool Recv(zmq::socket_t& sock, std::chrono::milliseconds time) {
		zmq::message_t ZType;
		zmq::message_t ZHeader;
		zmq::message_t ZBody;
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		int64_t q = 0;
		bool t = false;
		for (; !t && q < time.count(); t = GetMsgFromSock(sock, ZType, ZHeader, ZBody)) {
//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			q = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
		}
		if (!t)
			return false;
		Type = *(MsgType*)ZType.data();
		SndId = ((int32_t*)ZHeader.data())[0];
		RecvId = ((int32_t*)ZHeader.data())[1];
		BodySize = ((int32_t*)ZHeader.data())[2];
		operator delete(Body);
		Body = nullptr;
		if (BodySize > 0) {
			Body = operator new(BodySize);
			memcpy(Body, ZBody.data(), BodySize);
		}
		return true;
	}

	void SetHeader(int32_t sndId, int32_t recvId) noexcept {
		SndId = sndId;
		RecvId = recvId;
	}

	MsgType Type;
	int32_t SndId;
	int32_t RecvId;
	int32_t BodySize;
	void* Body;

private:
	bool GetMsgFromSock(zmq::socket_t& sock, zmq::message_t& type, zmq::message_t& header, zmq::message_t& body) {
		return (sock.recv(type, zmq::recv_flags::dontwait) &&
				sock.recv(header, zmq::recv_flags::dontwait) &&
				sock.recv(body, zmq::recv_flags::dontwait));
	}
};


#endif
