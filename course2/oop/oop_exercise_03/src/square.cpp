//
// Created by Sin Denis on 2019-09-16.
//

#include "square.h"

Square::Square() : _x{0, 0}, _y{0, 0}
{}

Square::Square(const Point& x, const Point& y) : _x{x}, _y{y}
{}

double Square::get_area() const
{
	return (_y.X() - _x.X()) * (_y.X() - _x.X());
}

Point Square::get_center() const
{
	return Point{_y.X() - (_y.X() - _x.X()) / 2, _x.Y() - (_y.X() - _x.X()) / 2};
}

std::ostream& Square::print_coors(std::ostream& out) const
{
	out << _x << " " << _y;
	return out;
}
