//
// Created by Sin Denis on 2019-12-24.
//

#ifndef KVSTORAGE_MESSAGE_H
#define KVSTORAGE_MESSAGE_H

#include <cstring>

enum class MsgType : int {
	put,
	get,
	remove,
	get_response,
	response,
	none
};

enum class ResStatus : int {
	ok,
	already_exist,
	not_found,
	storage_fail,
	none
};

std::string statusToStr(ResStatus status) {
	switch (status) {
		case ResStatus::ok:
			return "ok";
		case ResStatus::already_exist:
			return "already exist";
		case ResStatus::not_found:
			return "not found";
		case ResStatus::storage_fail:
			return "all stores are died";
		default:
			return "";
	}
}

class IMessage {
public:
	virtual MsgType type() = 0;
	virtual void* serialize() = 0;
	virtual void deserialize(void* body) = 0;
	virtual size_t bodySize() = 0;
	virtual ~IMessage() = default;
};

class MessagePut : public IMessage {
public:
	MessagePut():
		key_(0), val_(0)
	{
	}

	MessagePut(int32_t key, double val):
		key_(key), val_(val)
	{
	}

	MsgType type() override {
		return MsgType::put;
	}

	void* serialize() override {
		auto body = new u_char[sizeof(key_) + sizeof(val_)];
		void* keyPart = body;
		void* valPart = body + sizeof(key_);
		std::memcpy(keyPart, &key_, sizeof(key_));
		std::memcpy(valPart, &val_, sizeof(val_));
		return (void*)body;
	}

	void deserialize(void* data) override {
		key_ = *(int32_t*)data;
		val_ = *(double*)((u_char *)data + sizeof(key_));
	}

	size_t bodySize() override {
		return sizeof(key_) + sizeof(val_);
	}

	int32_t& key() { return key_; }

	double& val() { return val_; }

private:
	int32_t key_;
	double val_;
};

class MessageGet : public IMessage {
public:
	MessageGet():
		key_(0)
	{
	}

	explicit MessageGet(int32_t key):
		key_(key)
	{
	}

	MsgType type() override {
		return MsgType::get;
	}

	void* serialize() override {
		auto body = new u_char[sizeof(key_)];
		memcpy(body, &key_, sizeof(key_));
		return body;
	}

	void deserialize(void* data) override {
		key_ = *(int32_t*)data;
	}

	size_t bodySize() override {
		return sizeof(key_);
	}

	int32_t& key() {
		return key_;
	}

private:
	int32_t key_;
};

class MessageRemove : public IMessage {
public:
	MessageRemove():
		key_(0)
	{
	}

	explicit MessageRemove(int32_t key):
		key_(key)
	{
	}

	MsgType type() override {
		return MsgType::remove;
	}

	void* serialize() override {
		auto body = new u_char[sizeof(key_)];
		memcpy(body, &key_, sizeof(key_));
		return body;
	}

	void deserialize(void* data) override {
		key_ = *(int32_t*)data;
	}

	size_t bodySize() override {
		return sizeof(key_);
	}

	int32_t& key() {
		return key_;
	}

private:
	int32_t key_;
};

class MessageGetRes : public IMessage {
public:
	MessageGetRes():
		val_(0)
	{
	}

	explicit MessageGetRes(double val):
		val_(val)
	{
	}

	MsgType type() override {
		return MsgType::get_response;
	}

	void* serialize() override {
		auto body = new u_char[sizeof(val_)];
		memcpy(body, &val_, sizeof(val_));
		return body;
	}

	void deserialize(void* data) override {
		val_ = *(double*)data;
	}

	size_t bodySize() override {
		return sizeof(val_);
	}

	double val() {
		return val_;
	}

private:
	double val_;
};

class MessageRes : public IMessage {
public:
	MessageRes():
		status_(ResStatus::none)
	{
	}

	explicit MessageRes(ResStatus status):
		status_(status)
	{
	}

	MsgType type() override {
		return MsgType::response;
	}

	void* serialize() override {
		auto body = new u_char[sizeof(status_)];
		memcpy(body, &status_, sizeof(status_));
		return body;
	}

	void deserialize(void* data) override {
		status_ = *(ResStatus *)data;
	}

	size_t bodySize() override {
		return sizeof(status_);
	}

	ResStatus status() {
		return status_;
	}

private:
	ResStatus status_;
};

#endif //KVSTORAGE_MESSAGE_H
