//
// Created by Sin Denis on 2019-09-15.
//

#include "complex.h"

static double get_eps()
{
	double epsilon = 1.0;

	while (1.0 + 0.5 * epsilon != 1.0) {
		epsilon *= 0.5;
	}
	return epsilon;
}

Complex::Complex(double re, double im) : _re(re), _im(im)
{}

Complex::Complex() : Complex(0, 0)
{}

Complex::Complex(const Complex& num)
{
	this->_re = num._re;
	this->_im = num._im;
}

Complex& Complex::operator+= (const Complex& num)
{
	this->_re += num._re;
	this->_im += num._im;
	return *this;
}

Complex& Complex::operator-= (const Complex& num)
{
	this->_re -= num._re;
	this->_im -= num._im;
	return *this;
}

Complex& Complex::operator*= (const Complex& num)
{
	double t_re = _re * num._re - _im * num._im;
	double t_im = _re * num._im + _im * num._re;
	_re = t_re;
	_im = t_im;
	return *this;
}

Complex& Complex::operator/= (const Complex& num)
{
	double eps = get_eps();
	if (abs(num._re) < eps && abs(num._im) < eps)
		throw std::logic_error("Zero Division Error");
	double t_re = (_re * num._re + _im * num._im) / (num._re * num._re + num._im * num._im);
	double t_im = (_im * num._re - _re * num._im) / (num._re * num._re + num._im * num._im);
	_re = t_re;
	_im = t_im;
	return *this;
}

Complex Complex::operator+ (const Complex& num) const
{
	Complex res = *this;
	res += num;
	return res;
}

Complex Complex::operator- (const Complex& num) const
{
	Complex res = *this;
	res -= num;
	return res;
}

Complex Complex::operator* (const Complex &num) const
{
	Complex res = *this;
	res *= num;
	return res;
}

Complex Complex::operator/ (const Complex &num) const
{
	Complex res = *this;
	res /= num;
	return res;
}

bool Complex::operator== (const Complex& num) const
{
	double epsilon = get_eps();
	return abs(_re - num._re) < epsilon && abs(_im - num._im) < epsilon;
}

double Complex::_abs() const
{
	return sqrt(this->_re * this->_re + this->_im * this->_im);
}

bool abs_eq(const Complex& l, const Complex& r)
{
	return l._abs() - r._abs() < get_eps();
}

Complex Complex::conj()
{
	Complex res{_re, -_im};
	return res;
}

double Complex::Re()
{
	return _re;
}

double Complex::Im()
{
	return _im;
}

Complex operator"" _i(long double num)
{
	Complex res{0, static_cast<double>(num)};
	return res;
}

std::ostream& operator<< (std::ostream& out, const Complex& num)
{
	double eps = get_eps();
	out << num._re;
	if (abs(num._im) > eps) {
		if (num._im > 0)
			out << " + " << num._im << "i";
		else
			out << " - " << -num._im << "i";
	}
	return out;
}
