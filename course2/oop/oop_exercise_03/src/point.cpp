//
// Created by Sin Denis on 2019-09-16.
//

#include "point.h"
#include <cmath>

static double get_eps()
{
	double epsilon = 1.0;

	while (1.0 + 0.5 * epsilon != 1.0) {
		epsilon *= 0.5;
	}
	return epsilon;
}

Point::Point() : _x{0}, _y{0}
{}

Point::Point(double x, double y) : _x{x}, _y{y}
{}

double Point::X() const
{
	return _x;
}

double Point::Y() const
{
	return _y;
}

bool operator== (const Point& x, const Point& y)
{
	double eps = get_eps();
	return abs(x.X() - y.X()) < eps && abs(x.Y() - y.Y()) < eps;
}

std::ostream& operator<< (std::ostream& out, const Point& p)
{
	out << "(" << p.X() << " " << p.Y() << ")";
	return out;
}
