//
// Created by Sin Denis on 2019-09-16.
//

#ifndef OOP_EXERCISE_03_RECTNAGLE_H
#define OOP_EXERCISE_03_RECTNAGLE_H

#include "figure.h"
#include "point.h"

class Rectangle : public Figure {
public:
	Rectangle();
	Rectangle(const Point& x, const Point& y);
	double get_area() const override;
	Point get_center() const override;
	std::ostream& print_coors(std::ostream& out) const override;
private:
	Point _x;
	Point _y;
};

#endif //OOP_EXERCISE_03_RECTNAGLE_H
