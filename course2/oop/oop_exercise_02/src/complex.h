//
// Created by Sin Denis on 2019-09-04.
//

#ifndef OOP_EXERCISE_1_COMPLEX_H
#define OOP_EXERCISE_1_COMPLEX_H

#include <cmath>
#include <iostream>

class Complex
{
public:
	Complex();
	Complex(double re, double im);
	~Complex() = default;
	Complex(const Complex& num);
	Complex& operator+= (const Complex& num);
	Complex& operator-= (const Complex& num);
	Complex& operator*= (const Complex& num);
	Complex& operator/= (const Complex& num);
	Complex operator+ (const Complex& num) const;
	Complex operator- (const Complex& num) const;
	Complex operator* (const Complex& num) const;
	Complex operator/ (const Complex& num) const;
	bool operator== (const Complex& num) const;
	Complex conj();
	double Re();
	double Im();
	friend bool abs_eq(const Complex& l, const Complex& r);
	friend std::ostream& operator<< (std::ostream& out, const Complex& num);
private:
	double _re;
	double _im;
	double _abs() const;
};

Complex operator"" _i(long double num);

#endif //OOP_EXERCISE_1_COMPLEX_H
