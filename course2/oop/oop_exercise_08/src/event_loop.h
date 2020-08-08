#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
#include "figures.h"
#include "event.h"

class EventManager {
public:
	void subscribe(EventType type, std::shared_ptr<IHandler>& handler) {
		Handlers.emplace(type, handler);
	}

	void notify(EventType type, Event& event) {
		auto its = Handlers.equal_range(type);
		std::for_each(its.first, its.second, [&event](auto p) {
			p.second->event(event);
		});
	}

private:
	std::unordered_multimap<EventType, std::shared_ptr<IHandler>> Handlers;
};

class EventLoop {
public:
	void addHandler(EventType type, std::shared_ptr<IHandler>& handler) {
		EvManager.subscribe(type, handler);
	}

	void addEvent(Event& event) {
		std::lock_guard<std::mutex> guard(mut);
		Events.push(event);
	}

	void operator()() {
		while (!quit) {
			if (!Events.empty()) {
				Event ev = Events.front();
				Events.pop();

				switch(ev.type) {
					case EventType::quit:
						quit = true;
						break;
					default:
						EvManager.notify(ev.type, ev);
				}
			} else {
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
		}
	}

private:
	bool quit = false;
	std::queue<Event> Events;
	EventManager EvManager;
	std::mutex mut;
};

#endif