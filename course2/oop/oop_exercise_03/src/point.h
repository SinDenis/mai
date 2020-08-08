//
// Created by Sin Denis on 2019-09-16.
//

#ifndef OOP_EXERCISE_03_POINT_H
#define OOP_EXERCISE_03_POINT_H

#include <iostream>

class Point {
public:
	Point();
	Point(double x, double y);
	double X() const;
	double Y() const;
private:
	double _x;
	double _y;
};

bool operator== (const Point& x, const Point& y);
std::ostream& operator<< (std::ostream& out, const Point& p);

#endif //OOP_EXERCISE_03_POINT_H
