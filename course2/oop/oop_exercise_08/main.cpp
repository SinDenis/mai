#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <thread>
#include <queue>
#include "src/event.h"
#include "src/figures.h"
#include "src/event_loop.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cout << "using ./app buf-size\n";
		return 0;
	}

	int bufSize = std::atoi(argv[1]);
	std::vector<std::shared_ptr<IFigure>> shapes;
	EventLoop eventLoop;
	std::shared_ptr<IHandler> handlerPrinter(new HandlerPrinter());
	std::shared_ptr<IHandler> handlerLogger(new HandlerLogger());
	eventLoop.addHandler(EventType::print, handlerPrinter);
	eventLoop.addHandler(EventType::log, handlerLogger);
	RhombusFactory rhombusFactory;
	PentagonFactory pentagonFactory;
	HexagonFactory hexagonFactory;
	std::string cmd;
	int id = 1;

	std::thread thHandler(std::ref(eventLoop));
	while (std::cin >> cmd) {
		if (cmd == "quit") {
			Event ev = Event(EventType::quit, std::make_shared<EventData>(), std::make_shared<EventContext>(), [](auto){});
			eventLoop.addEvent(ev);
			break;
		} else if (cmd == "add") {
			char type;
			std::pair<double, double> center;
			std::pair<double, double> point;
			std::cin >> type >> center.first >> center.second >> point.first >> point.second;
			switch(type) {
				case 'R':
					shapes.push_back(rhombusFactory.FigureCreate(id, center, point));
					break;
				case 'P':
					shapes.push_back(pentagonFactory.FigureCreate(id, center, point));
					break;
				case 'H':
					shapes.push_back(hexagonFactory.FigureCreate(id, center, point));
					break;
				default:
					std::cout << "Primitive isn't added\n";
					break;
			}
			++id;
			std::cout << "Primitive is added\n";
		}
		bool printerDone = false;
		bool ctxDone = false;
		if (shapes.size() == bufSize) {
			Event ev1 = Event(EventType::print, 
				std::make_shared<EventDataPrinter>(shapes),
				std::make_shared<EventContextPrinter>(printerDone),
				[](std::shared_ptr<EventContext> cnt) {
					if(auto ptr = std::static_pointer_cast<EventContextPrinter>(cnt))
                		ptr->done = true;
				});
			Event ev2 = Event(EventType::log,
				std::make_shared<EventDataLogger>(shapes),
				std::make_shared<EventContextLogger>(ctxDone),
				[](std::shared_ptr<EventContext> cnt) {
					if(auto ptr = std::static_pointer_cast<EventContextPrinter>(cnt))
                		ptr->done = true;
				});
			eventLoop.addEvent (ev1);
			eventLoop.addEvent (ev2);
			while(!printerDone && !ctxDone) {
				std::cout << '.';
				std::cout.flush();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			std::cout << "\n";
			shapes.resize(0);
		}
	}
	thHandler.join();
	return 0;
}
