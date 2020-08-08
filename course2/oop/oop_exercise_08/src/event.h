#ifndef TASK_H
#define TASK_H

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_map>
#include "figures.h"

std::string strGen(int len) {
	std::random_device rd;
	std::mt19937 randGen(rd());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(97, 122);
	std::string res = "";

	for (int i = 0; i < len; ++i) {
		res += (char)(distribution(randGen));
	}
	return res;
}

enum class EventType {
	log,
	print,
	quit
};

class EventContext{
public:
	virtual ~EventContext() = default;
};

struct EventContextPrinter: public EventContext{
    EventContextPrinter(bool &d): done(d) {};
    bool  &done;
};

struct EventContextLogger: public EventContext{
    EventContextLogger(bool &d): done(d) {};
    bool  &done;
};

class EventData {
public:
	virtual ~EventData() = default;
};

struct Event {
	Event(EventType tp, std::shared_ptr<EventData> dt, std::shared_ptr<EventContext> c, std::function<void(std::shared_ptr<EventContext>)> f):
		type(tp), data(dt), ctx(c), callback(f)
	{}
	EventType type;
	std::shared_ptr<EventData> data;
	std::shared_ptr<EventContext> ctx;
	std::function<void(std::shared_ptr<EventContext>)> callback;
};

class EventDataPrinter : public EventData {
public:
	EventDataPrinter(const std::vector<std::shared_ptr<IFigure>>& shapes): Shapes(shapes) {
	}
	const std::vector<std::shared_ptr<IFigure>> Shapes;	
};

class EventDataLogger : public EventData {
public:
	EventDataLogger(const std::vector<std::shared_ptr<IFigure>>& shapes): Shapes(shapes) {
	} 
	const std::vector<std::shared_ptr<IFigure>> Shapes;	
};

class IHandler {
public:
	virtual bool event(Event&) = 0;
	virtual ~IHandler() = default;
};

class HandlerPrinter : public IHandler {
public:
	bool event(Event& event) override {
		std::shared_ptr<EventDataPrinter> data = std::static_pointer_cast<EventDataPrinter>(event.data);
		if (!data)
			return false;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		for (const std::shared_ptr<IFigure>& shape : data->Shapes) {
			shape->Print(std::cout);
		}
		event.callback(event.ctx);
		return true;
	}	
};

class HandlerLogger : public IHandler {
public:
	bool event(Event& event) override {
		std::ofstream file;
		file.open(strGen(10), std::ios_base::out);
		std::shared_ptr<EventDataLogger> data = std::static_pointer_cast<EventDataLogger>(event.data);
		if (!data) {
			file.close();
			return false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		for (const std::shared_ptr<IFigure>& shape : data->Shapes) {
			shape->Print(file);
		}
		event.callback(event.ctx);
		file.close();
		return true;
	}
};

#endif
