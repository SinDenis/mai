//
// Created by Sin Denis on 2019-09-16.
//

#include "triangle.h"

Triangle::Triangle() : _x{0, 0}, _y{0, 0}
{}

Triangle::Triangle(const Point& x, const Point& y, const Point& z) : _x{x}, _y{y}, _z{z}
{
	double a, b, c;

	a = sqrt((_x.X() - _y.X()) * (_x.X() - _y.X()) + (_x.Y() - _y.Y()) * (_x.Y() - _y.Y()));
	b = sqrt((_x.X() - _z.X()) * (_x.X() - _z.X()) + (_x.Y() - _z.Y()) * (_x.Y() - _z.Y()));
	c = sqrt((_z.X() - _y.X()) * (_z.X() - _y.X()) + (_z.Y() - _y.Y()) * (_z.Y() - _y.Y()));
	if (a + b <= c || a + c <= b || c + b <= a) {
		throw std::logic_error("Triangle doesn't exist");
	}
}

double Triangle::get_area() const
{
	return ((_x.X() - _z.X()) * (_y.Y() - _z.Y()) - (_y.X() - _z.X()) * (_x.Y() - _z.Y())) / 2;
}

Point Triangle::get_center() const
{
	return Point{(_x.X() + _y.X() + _z.X()) / 3, (_x.Y() + _y.Y() + _z.Y()) / 3};
}

std::ostream& Triangle::print_coors(std::ostream& out) const
{
	out << _x << " " << _y << " " << _z;
	return out;
}
