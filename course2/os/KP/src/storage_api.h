//
// Created by Sin Denis on 2019-12-24.
//

#ifndef KVSTORAGE_STORAGE_API_H
#define KVSTORAGE_STORAGE_API_H

#include <zmq.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "message.h"
#include "msg_service.h"

namespace NStoreAPI {

	class StorageAPI {
	public:
		StorageAPI(uint16_t port1, uint16_t port2):
			ctx_()
		{
			ports.emplace_back(true, port1);
			ports.emplace_back(true, port2);
			senderRefresh();
		}

		~StorageAPI() = default;

		ResStatus put(int32_t key, double val) {
			ResStatus status;
			try {
				status = command(MsgService::sendPut, key, val, sender_);
			} catch (const std::exception& exc) {
				return ResStatus::storage_fail;
			}

			return status;
		}

		ResStatus remove(int32_t key) {
			ResStatus status;
			try {
				status = command(MsgService::sendRemove, key, sender_);
			} catch (const std::exception& exc) {
				return ResStatus::storage_fail;
			}

			return status;
		}

		ResStatus get(int32_t key, double& val) {
			ResStatus status;
			try {
				status = getImpl(key, val);
			} catch (const std::exception& exc) {
				return ResStatus::storage_fail;
			}

			return status;
		}

	private:
		zmq::context_t ctx_;
		zmq::socket_t sender_;
		std::vector<std::pair<bool, uint16_t >> ports;

		void senderRefresh() {
			sender_.close();
			sender_ = zmq::socket_t(ctx_, zmq::socket_type::req);
			sender_.setsockopt(ZMQ_LINGER, 0);
			sender_.setsockopt(ZMQ_RCVTIMEO, 3000);
		}

		ResStatus getImpl(int32_t key, double& val) {
			auto livePort = std::find_if(
					ports.begin(), ports.end(), [](const std::pair<bool, uint16_t>& p) {
						return p.first;
					});
			if (livePort == ports.end())
				throw std::exception();

			sender_.connect(GetReqUrl(livePort->second));
			MsgService::sendGet(key, sender_);
			std::shared_ptr<IMessage> msg = MsgService::recv(sender_);
			if (msg == nullptr) {
				livePort->first = false;
				senderRefresh();
				return getImpl(key, val);
			}
			if (msg->type() == MsgType::response) {
				auto msgRes = std::dynamic_pointer_cast<MessageRes>(msg);
				return msgRes->status();
			}
			auto msgGetRes = std::dynamic_pointer_cast<MessageGetRes>(msg);
			val = msgGetRes->val();
			return ResStatus::ok;
		}

		template <class...args>
		ResStatus command(void(*f)(args&...), args&... params) {
			auto livePort = std::find_if(
					ports.begin(), ports.end(), [](const std::pair<bool, uint16_t>& p) {
						return p.first;
					});
			if (livePort == ports.end())
				throw std::exception();

			sender_.connect(GetReqUrl(livePort->second));
			f(params...);
			std::shared_ptr<IMessage> msgRes = MsgService::recv(sender_);
			if (msgRes == nullptr) {
				senderRefresh();
				livePort->first = false;
				return command(f, params...);
			}
			auto res = std::dynamic_pointer_cast<MessageRes>(msgRes);
			return res->status();
		}

		uint16_t getLivePort() {
			auto livePort = std::find_if(
					ports.begin(), ports.end(), [](const std::pair<bool, uint16_t>& p) {
						return p.first;
					});
			if (livePort == ports.end())
				return 0;
			return livePort->second;
		}
	};

}


#endif //KVSTORAGE_STORAGE_API_H
