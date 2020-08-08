//
// Created by Sin Denis on 2019-12-24.
//

#ifndef KVSTORAGE_MSG_SERVICE_H
#define KVSTORAGE_MSG_SERVICE_H

#include <zmq.hpp>
#include <memory>
#include <chrono>
#include <unistd.h>
#include "message.h"

const char* REP_URL = "tcp://*:";
const char* REQ_URL = "tcp://localhost:";

std::string GetRepUrl(uint16_t port) {
	return REP_URL + std::to_string(port);
}

std::string GetReqUrl(uint16_t port) {
	return REQ_URL + std::to_string(port);
}

namespace MsgService {

	void sendImpl(std::shared_ptr<IMessage> msg, zmq::socket_t& socket) {
		MsgType msgType = msg->type();
		void* msgBody = msg->serialize();
		zmq::message_t type(&msgType, sizeof(MsgType));
		zmq::message_t body(msgBody, msg->bodySize());
		socket.send(type, zmq::send_flags::sndmore);
		socket.send(body, zmq::send_flags::none);
		delete[] (u_char *)msgBody;
	}

	void sendPut(int32_t& key, double& val, zmq::socket_t& socket) {
		std::shared_ptr<IMessage> msg(new MessagePut(key, val));
		sendImpl(msg, socket);
	}

	void sendRemove(int32_t& key, zmq::socket_t& socket) {
		std::shared_ptr<IMessage> msg(new MessageRemove(key));
		sendImpl(msg, socket);
	}

	void sendGet(int32_t key, zmq::socket_t& socket) {
		std::shared_ptr<IMessage> msg(new MessageGet(key));
		sendImpl(msg, socket);
	}

	void sendGetRes(double val, zmq::socket_t& socket) {
		std::shared_ptr<IMessage> msg(new MessageGetRes(val));
		sendImpl(msg, socket);
	}

	void sendRes(ResStatus status, zmq::socket_t& socket) {
		std::shared_ptr<IMessage> msg(new MessageRes(status));
		sendImpl(msg, socket);
	}

	void send(std::shared_ptr<IMessage> msg, zmq::socket_t& socket) {
		sendImpl(msg, socket);
	}

	std::shared_ptr<IMessage> recv(zmq::socket_t& socket) {
		zmq::message_t type;
		zmq::message_t body;

		bool rcv = socket.recv(type) && socket.recv(body);
		if (!rcv)
			return nullptr;
		MsgType msgType = *(MsgType*)type.data();
		std::shared_ptr<IMessage> msg = nullptr;
		switch (msgType) {
			case MsgType::put:
				msg = std::shared_ptr<IMessage>(new MessagePut());
				break;
			case MsgType::remove:
				msg = std::shared_ptr<IMessage>(new MessageRemove());
				break;
			case MsgType::get:
				msg = std::shared_ptr<IMessage>(new MessageGet());
				break;
			case MsgType::get_response:
				msg = std::shared_ptr<IMessage>(new MessageGetRes());
				break;
			case MsgType::response:
				msg = std::shared_ptr<IMessage>(new MessageRes());
				break;
			default:
				break;
		}
		msg->deserialize(body.data());
		return msg;
	}
};

#endif //KVSTORAGE_MSG_SERVICE_H
