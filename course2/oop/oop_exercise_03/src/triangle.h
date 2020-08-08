//
// Created by Sin Denis on 2019-09-16.
//

#ifndef OOP_EXERCISE_03_TRIANGLE_H
#define OOP_EXERCISE_03_TRIANGLE_H

#include "figure.h"
#include "point.h"
#include <cmath>

class Triangle : public Figure {
public:
	Triangle();
	Triangle(const Point& x, const Point& y, const Point& z);
	double get_area() const override;
	Point get_center() const override;
	std::ostream& print_coors(std::ostream& out) const override;
private:
	Point _x;
	Point _y;
	Point _z;
};

#endif //OOP_EXERCISE_03_TRIANGLE_H
