//
// Created by Sin Denis on 2019-09-16.
//

#ifndef OOP_EXERCISE_03_SQUARE_H
#define OOP_EXERCISE_03_SQUARE_H

#include "figure.h"
#include "point.h"
#include <cmath>

class Square : public Figure {
public:
	Square();
	Square(const Point& x, const Point& y);
	double get_area() const override;
	Point get_center() const override;
	std::ostream& print_coors(std::ostream& out) const override;
private:
	Point _x;
	Point _y;
};

#endif //OOP_EXERCISE_03_SQUARE_H
