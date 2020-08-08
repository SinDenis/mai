//
// Created by Sin Denis on 2019-09-16.
//

#include "rectnagle.h"

Rectangle::Rectangle() : _x{0, 0}, _y{0, 0}
{}

Rectangle::Rectangle(const Point& x, const Point& y) : _x{x}, _y{y}
{}

double Rectangle::get_area() const
{
	return (_y.X() - _x.X()) * (_x.Y() - _y.Y());
}

Point Rectangle::get_center() const
{
	return Point{_y.X() - (_y.X() - _x.X()) / 2, _x.Y() - (_x.Y() - _y.Y()) / 2};
}

std::ostream& Rectangle::print_coors(std::ostream& out) const
{
	out << _x << " " << _y;
	return out;
}
