/*
 * Author: Sin Denis Dmitrievich
 * Group: M80-207B-18
 * Lab04 OOP
 * Variant: 21
 * Figures: Rhombus, Pentagon, Hexagon
 */

#include <iostream>
#include <tuple>
#include "src/figures.h"

int main() {
	std::string s;
	int id = 0;
	while (std::cin >> s) {
		++id;
		if (s == "RI") {
			int x1, y1, x2, y2, x3, y3, x4, y4;
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
			TRhombus<int> rhombus(x1, y1, x2, y2, x3, y3, x4, y4);
			std::cout << id << " Figure: Rhombus integer\n";
			std::cout << "Square: " << Square(rhombus) << "\n";
			std::cout << "Center: " << Center(rhombus) << "\n";
			std::cout << "Coors: ";
			Print(rhombus);
			std::cout << "\n";
		} else if (s == "RD") {
			double x1, y1, x2, y2, x3, y3, x4, y4;
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
			using point = std::pair<double, double>;
			using std::make_pair;
			std::tuple<point, point, point, point>  tRhombus(make_pair(x1, y1),
					make_pair(x2, y2),  make_pair(x3, y3), make_pair(x4, y4));
			TRhombus<double> rhombus(x1, y1, x2, y2, x3, y3, x4, y4);
			std::cout << id << " Figure: Rhombus double\n";
			std::cout << "Square: " << Square(tRhombus) << "\n";
			std::cout << "Center: " << Center(rhombus) << "\n";
			std::cout << "Coors: ";
			Print(rhombus);
			std::cout << "\n";
		} else if (s == "PI") {
			int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5;
			TPentagon<int> pentagon(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5);
			std::cout << id << " Figure: Pentagon integer\n";
			std::cout << "Square: " << Square(pentagon) << "\n";
			std::cout << "Center: " << Center(pentagon) << "\n";
			std::cout << "Coors: ";
			Print(pentagon);
			std::cout << "\n";
		} else if (s == "PD") {
			double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5;
			TPentagon<double> pentagon(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5);
			std::cout << id << " Figure: Pentagon integer\n";
			std::cout << "Square: " << Square(pentagon) << "\n";
			std::cout << "Center: " << Center(pentagon) << "\n";
			std::cout << "Coors: ";
			Print(pentagon);
			std::cout << "\n";
		} else if (s == "HI") {
			int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5 >> x6 >> y6;
			using point = std::pair<int, int>;
			using std::make_pair;
			std::tuple<point, point, point, point, point, point> tHexagon(
					make_pair(x1, y1), make_pair(x2, y2), make_pair(x3, y3),
					make_pair(x4, y4), make_pair(x5, y5), make_pair(x6, y6));
			THexagon<int> hexagon(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6);
			std::cout << id << " Figure: Hexagon integer\n";
			std::cout << "Square: " << Square(tHexagon) << "\n";
			std::cout << "Center: " << Center(hexagon) << "\n";
			std::cout << "Coors: ";
			Print(hexagon);
			std::cout << "\n";
		} else if (s == "HD") {
			double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
			std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5 >> x6 >> y6;
			THexagon<double> hexagon(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6);
			std::cout << id << " Figure: Hexagon double\n";
			std::cout << "Square: " << Square(hexagon) << "\n";
			std::cout << "Center: " << Center(hexagon) << "\n";
			std::cout << "Coors: ";
			Print(hexagon);
			std::cout << "\n";
		} else {
			std::cout << "Wrong request\n";
		}
	}
	return 0;
}
