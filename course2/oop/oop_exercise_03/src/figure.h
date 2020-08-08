//
// Created by Sin Denis on 2019-09-16.
//

#ifndef OOP_EXERCISE_03_FIGURE_H
#define OOP_EXERCISE_03_FIGURE_H

#include <iostream>
#include "point.h"

class Figure {
public:
	virtual double get_area() const = 0;
	virtual Point get_center() const = 0;
	virtual std::ostream& print_coors(std::ostream& out) const = 0;
	virtual ~Figure() = default;
};

#endif //OOP_EXERCISE_03_FIGURE_H
