//
// Created by Sin Denis on 2019-12-24.
//

#ifndef KVSTORAGE_STORAGE_H
#define KVSTORAGE_STORAGE_H

#include <zmq.hpp>
#include <map>
#include <string>
#include <unistd.h>
#include <cstdint>
#include <thread>
#include <memory>
#include "message.h"
#include "msg_service.h"

void TakePort(zmq::socket_t& sock, uint16_t port) {
	try {
		sock.bind(GetRepUrl(port));
	} catch (const zmq::error_t& err) {
		std::cout << err.what() << std::endl;
		exit(EXIT_SUCCESS);
	}
}

class BaseStore {
public:
	explicit BaseStore(uint16_t port):
		ctx_(), receiver_(ctx_, zmq::socket_type::rep), storage_()
	{
		TakePort(receiver_, port);
	}

protected:
	zmq::context_t ctx_;
	zmq::socket_t receiver_;
	std::map<int32_t, double > storage_;

	void putHandle(std::shared_ptr<IMessage> msg) {
		auto msgPut = std::dynamic_pointer_cast<MessagePut>(msg);

		if (storage_.find(msgPut->key()) != storage_.end()) {
			MsgService::sendRes(ResStatus::already_exist, receiver_);
			return ;
		}

		storage_.insert(std::make_pair(msgPut->key(), msgPut->val()));
		std::cout << msgPut->key() << ": " << storage_[msgPut->key()] << std::endl;
		MsgService::sendRes(ResStatus::ok, receiver_);
	}

	void removeHandle(std::shared_ptr<IMessage> msg) {
		auto msgRemove = std::dynamic_pointer_cast<MessageRemove>(msg);

		if (storage_.find(msgRemove->key()) == storage_.end()) {
			MsgService::sendRes(ResStatus::not_found, receiver_);
			return ;
		}

		storage_.erase(msgRemove->key());
		std::cout << "key " << msgRemove->key() << " is deleted" << std::endl;
		MsgService::sendRes(ResStatus::ok, receiver_);
	}

	void getHandle(std::shared_ptr<IMessage> msg) {
		auto msgGet = std::dynamic_pointer_cast<MessageGet>(msg);

		if (storage_.find(msgGet->key()) == storage_.end()) {
			MsgService::sendRes(ResStatus::not_found, receiver_);
			return ;
		}

		double val = storage_[msgGet->key()];
		std::cout << "val = " << val << std::endl;
		MsgService::sendGetRes(val, receiver_);
	}

private:

};

class MainStore : public BaseStore {
public:
	MainStore(uint16_t port, uint16_t reservePort):
		BaseStore(port), reservePort_(reservePort), pusher_(ctx_, zmq::socket_type::push)
	{
		pusher_.connect(GetReqUrl(reservePort));
	}

	void run() {
		std::shared_ptr<IMessage> msg = nullptr;
		while (true) {
			msg = MsgService::recv(receiver_);
			switch (msg->type()) {
				case MsgType::put:
					putHandle(msg);
					break;
				case MsgType::remove:
					removeHandle(msg);
					break;
				case MsgType::get:
					getHandle(msg);
					break;
				default:
					break;
			}
			MsgService::send(msg, pusher_);
		}
	}

private:
	uint16_t reservePort_;
	zmq::socket_t pusher_;


};

class ReserveStore : public BaseStore {
public:
	ReserveStore(uint16_t port, uint16_t servicePort):
		BaseStore(port), puller_(ctx_, zmq::socket_type::pull)
	{
		TakePort(puller_, servicePort);
	}

	void run() {
		std::thread([this]() { reserveWorker(); }).detach();
		mainWorker();
	}

private:
	zmq::socket_t puller_;

	void reserveWorker() {
		std::shared_ptr<IMessage> msg = nullptr;
		while (true) {
			msg = MsgService::recv(puller_);
			switch (msg->type()) {
				case MsgType::put:
					putReserveHandle(msg);
					break;
				case MsgType::remove:
					removeReserveHandle(msg);
					break;
				default:
					break;
			}
		}
	}

	void mainWorker() {
		std::shared_ptr<IMessage> msg = nullptr;
		while (true) {
			msg = MsgService::recv(receiver_);
			switch (msg->type()) {
				case MsgType::put:
					putHandle(msg);
					break;
				case MsgType::remove:
					removeHandle(msg);
					break;
				case MsgType::get:
					getHandle(msg);
					break;
				default:
					break;
			}
		}
	}

	void putReserveHandle(std::shared_ptr<IMessage> msg) {
		auto msgPut = std::dynamic_pointer_cast<MessagePut>(msg);
		storage_.insert(std::make_pair(msgPut->key(), msgPut->val()));
		std::cout << "Add: " << msgPut->key() << ": " << storage_[msgPut->key()] << std::endl;
	}

	void removeReserveHandle(std::shared_ptr<IMessage> msg) {
		auto msgRemove = std::dynamic_pointer_cast<MessageRemove>(msg);

		if (storage_.find(msgRemove->key()) == storage_.end())
			return ;

		storage_.erase(msgRemove->key());
		std::cout << "key " << msgRemove->key() << " is deleted" << std::endl;
	}
};

#endif //KVSTORAGE_STORAGE_H
