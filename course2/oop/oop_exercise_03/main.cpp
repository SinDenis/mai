/*
 * Author: Sin Denis Dmitrievich
 * Group: M80-207B-18
 * Variant: 1
 * Figures: Triangle, Square, Rectangle
 */

#include <iostream>
#include "src/figure.h"
#include "src/triangle.h"
#include "src/square.h"
#include "src/rectnagle.h"
#include <vector>

inline void read_shape(std::vector<Figure *>& shapes)
{
	double x1, y1, x2, y2, x3, y3;
	char shape_type;
	Triangle *t = nullptr;
	Square *s = nullptr;
	Rectangle *r = nullptr;

	std::cin >> shape_type;
	switch (shape_type) {
		case 't':
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
			try {
				t = new Triangle{Point{x1, y1}, Point{x2, y2}, Point{x3, y3}};
			} catch (std::logic_error& err) {
				std::cout << err.what() << std::endl;
				break;
			}
			shapes.push_back(dynamic_cast<Figure*>(t));
			break;
		case 's':
			std::cin >> x1 >> y1 >> x2 >> y2;
			s = new Square{Point{x1, y1}, Point{x2, y2}};
			shapes.push_back(dynamic_cast<Figure*>(s));
			break;
		case 'r':
			std::cin >> x1 >> y1 >> x2 >> y2;
			r = new Rectangle{Point{x1, y1}, Point{x2, y2}};
			shapes.push_back(dynamic_cast<Figure*>(r));
			break;
		default:
			std::cout << shape_type << "type doesn't exist. There are t, s, r figures\n";
	}
}

int main() {
	int idx;
	std::string operation;
	std::vector<Figure*> shapes;
	std::cout << "add <type> x1 y1 x2 y2 x3 y3\ndel idx\nall\nsize\ncoors idx\ncenter idx\narea idx\n";

	while (std::cin >> operation) {
		if (operation == "add") {
			read_shape(shapes);
		} else if (operation == "del") {
			std::cin >> idx;
			if (idx < 0 || idx >= shapes.size()) {
				std::cout << "Invalid index\n";
				continue;
			}
			delete shapes[idx];
			for (; idx < shapes.size() - 1; ++idx) {
				shapes[idx] = shapes[idx + 1];
			}
			shapes.pop_back();
		} else if (operation == "all") {
			for (int i = 0; i < shapes.size(); ++i) {
				std::cout << i << ":\n";
				std::cout << "Area: " << shapes[i]->get_area() << std::endl;
				std::cout << "Center: " << shapes[i]->get_center() << std::endl;
				std::cout << "Coordinates: ";
				shapes[i]->print_coors(std::cout) << std::endl;
				std::cout << std::endl;
			}
		} else if (operation == "size") {
			std::cout << shapes.size() << std::endl;
		} else if (operation == "coors") {
			std::cin >> idx;
			if (idx < 0 || idx >= shapes.size()) {
				std::cout << "Invalid index\n";
				continue;
			}
			shapes[idx]->print_coors(std::cout) << std::endl;
		} else if (operation == "center") {
			std::cin >> idx;
			if (idx < 0 || idx >= shapes.size()) {
				std::cout << "Invalid index\n";
				continue;
			}
			std::cout << shapes[idx]->get_center() << std::endl;
		} else if (operation == "area") {
			std::cin >> idx;
			if (idx < 0 || idx >= shapes.size()) {
				std::cout << "Invalid index\n";
				continue;
			}
			std::cout << shapes[idx]->get_area() << std::endl;
		} else {
			std::cout << "add <type> x1 y1 x2 y2 x3 y3\ndel idx\nall\nsize\ncoors idx\ncenter idx\narea idx\n";
		}
	}
	for (int i = 0; i < shapes.size(); ++i) {
		delete shapes[i];
	}
	return 0;
}