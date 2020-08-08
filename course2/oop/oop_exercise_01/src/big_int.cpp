//
// Created by Sin Denis on 2019-09-17.
//

#include "big_int.h"

int BigInt::base = 10;

BigInt::BigInt()
{
	this->number.push_back(0);
	this->sign = 0;
}

BigInt::BigInt(const std::string& num)
{
	char begin = 0;

	if (num[0] == '-') {
		this->sign = 1;
		++begin;
	} else if (num[0] == '+') {
		this->sign = 0;
		++begin;
	} else {
		this->sign = 0;
	}
	for (int i = num.size() - 1; i >= begin; --i) {
		this->number.push_back(num[i] - '0');
	}
}

BigInt::BigInt(const char *num) : BigInt(static_cast<const std::string>(num))
{}

BigInt::BigInt(long long number)
{
	unsigned long long num = number >= 0 ? number : -number;
	this->sign = number < 0 ? 1 : 0;
	if (num == 0)
		this->number.push_back(0);
	while (num > 0) {
		this->number.push_back(num % 10);
		num /= 10;
	}
}

BigInt::BigInt(int num) : BigInt(static_cast<long long>(num))
{}

BigInt::BigInt(const BigInt& num)
{
	this->number = num.number;
	this->sign = num.sign;
}

BigInt& BigInt::operator= (const BigInt& num)
{
	this->number = num.number;
	this->sign = num.sign;
	return *this;
}

BigInt BigInt::operator- () const
{
	BigInt res = *this;
	res.sign = abs(this->sign - 1);
	return res;
}

bool operator< (const BigInt& l, const BigInt& r)
{
	if (l.number.size() == 1 && l.number[0] == 0 &&
		r.number.size() == 1 && r.number[0] == 0) {
		return false;
	}
	if (l.sign != r.sign) {
		return l.sign > r.sign;
	}
	if (l.sign == 1 && l.number.size() == r.number.size()) {
		for (int i = l.number.size() - 1; i >= 0; --i) {
			if (l.number[i] != r.number[i]) {
				return l.number[i] > r.number[i];
			}
		}
		return false;
	}
	if (l.sign == 1) {
		return l.number.size() > r.number.size();
	}
	if (l.sign == 0 && l.number.size() == r.number.size()) {
		for (int i = l.number.size() - 1; i >= 0; --i) {
			if (l.number[i] != r.number[i]) {
				return l.number[i] < r.number[i];
			}
		}
		return false;
	}
	return l.number.size() < r.number.size();
}

bool operator> (const BigInt& l, const BigInt& r)
{
	return r < l;
}

bool operator<= (const BigInt& l, const BigInt& r)
{
	return !(l > r);
}

bool operator>= (const BigInt& l, const BigInt& r)
{
	return !(l < r);
}

bool operator== (const BigInt& l, const BigInt& r)
{
	return !(l > r) && !(l < r);
}

bool operator!= (const BigInt& l, const BigInt& r)
{
	return !(l == r);
}

BigInt& BigInt::add(const BigInt& num)
{
	char carry = 0;

	for (size_t i = 0; i < num.number.size() || carry; ++i) {
		if (i == this->number.size()) {
			this->number.push_back(0);
		}
		this->number[i] += carry + (i < num.number.size() ? num.number[i] : 0);
		carry = this->number[i] >= BigInt::base;
		if (carry) {
			this->number[i] -= BigInt::base;
		}
	}
	return *this;
}

BigInt& BigInt::sub(const BigInt& num)
{
	char carry = 0;

	for (size_t i = 0; i < num.number.size() || carry; ++i) {
		if (i == this->number.size()) {
			this->number.push_back(0);
		}
		this->number[i] -= carry + (i < num.number.size() ? num.number[i] : 0);
		carry = this->number[i] < 0;
		if (carry) {
			this->number[i] += BigInt::base;
		}
	}
	while (this->number.size() > 1 && this->number.back() == 0)
		this->number.pop_back();
	return *this;
}

BigInt BigInt::mul(const BigInt& num) const
{
	BigInt res;
	res.number.resize(this->number.size() + num.number.size(), 0);
	for (size_t i = 0; i < this->number.size(); ++i) {
		for (size_t j = 0, carry = 0; j < num.number.size() || carry; ++j) {
			int cur = res.number[i + j] +
					  this->number[i] *
					  (j < num.number.size() ? num.number[j] : 0) + carry;
			res.number[i + j] = cur % BigInt::base;
			carry = cur / BigInt::base;
		}
	}
	while (res.number.size() > 1 && res.number.back() == 0) {
		res.number.pop_back();
	}
	return res;
}

BigInt& BigInt::operator+= (const BigInt& num)
{
	if ((*this < 0 && num < 0) || (*this > 0 && num > 0)) {
		this->add(num);
		return *this;
	}
	if (*this < 0 && -(*this) < num) {
		BigInt tmp = num;
		tmp.sub(-(*this));
		*this = tmp;
		return *this;
	} else if (*this < 0 && -(*this) >= num) {
		BigInt tmp = -(*this);
		tmp.sub(num);
		*this = tmp;
		this->sign = 1;
		return *this;
	} else if (*this >= 0 && -num < *this) {
		this->sub(-num);
		return *this;
	}
	BigInt tmp = -(num);
	tmp.sub(*this);
	*this = tmp;
	this->sign = 1;
	return *this;
}

BigInt& BigInt::operator-= (const BigInt& num)
{
	if ((*this < 0 && num > 0) || (*this > 0 && num < 0)) {
		return this->add(num);
	}
	if (*this < 0) {
		return *this += -num;
	}
	if (*this > num) {
		return this->sub(num);
	}
	BigInt tmp = num;
	tmp.sub(*this);
	*this = tmp;
	this->sign = 1;
	return *this;
}

BigInt& BigInt::operator++ ()
{
	return *this < 0 ? this->sub(1) : this->add(1);
}

void BigInt::help_div(std::vector<char>& u, std::vector<char>& v, int i, int q)
{
	int carry_mul = 0;
	int carry_div = 0;
	int j = 0;

	for (; i < u.size() && j < v.size() + 1; ++i) {
		int cur = carry_mul + (j < v.size() ? v[j] * q : 0);
		u[i] -= carry_div + cur % 10;
		carry_mul = cur / 10;
		carry_div = u[i] < 0;
		if (carry_div && j != v.size())
			u[i] += 10;
		++j;
	}
}

void BigInt::compensation_div(std::vector<char>& u, std::vector<char>& v, int i)
{
	int carry = 0;
	int j = 0;

	for (; j < v.size() + 1; ++j) {
		u[i] += carry + (j < v.size() ? v[j] : 0);
		carry = u[i] > 10;
		if (carry)
			u[i] -= 10;
		++i;
	}
}

void BigInt::short_div(std::vector<char>& u, char v)
{
	int carry = 0;

	for (int i = u.size() - 1; i >= 0; --i) {
		int cur = u[i] + carry * 10;
		u[i] = cur / v;
		carry = cur % v;
	}
	for (int i = u.size() - 1; i >= 0 && u.back() == 0; --i) {
		u.pop_back();
	}
}

BigInt BigInt::div(const BigInt& num) const
{
	BigInt res;
	BigInt dividend;
	BigInt divider;

	int m = this->number.size() - num.number.size();
	int n = num.number.size();
	int d = BigInt::base / (num.number[num.number.size() - 1] + 1); // Normalize
	if (d == 1) {
		divider = num;
		dividend = *this;
		dividend.number.push_back(0);
	} else {
		dividend = *this * d;
		divider = num * d;
	}
	std::vector<char>& u = dividend.number;
	std::vector<char>& v = divider.number;
	int b = BigInt::base;
	int r = 0;
	int q = 0;
	res.number.resize(m + 1);
	for (int i = m; i >= 0; --i) {
		q = (u[i + n] * b + u[i + n - 1]) / v[n - 1];
		r = (u[i + n] * b + u[i + n - 1]) % v[n - 1];
		if (q == b || (q * v[n - 2] > b * r + u[i + n - 2])) {
			--q;
			r += v[n - 1];
			if (r < b && q * v[n - 2] > b * r + u[i + n - 2]) {
				--q;
				r += v[n - 1];
			}
		}
		help_div(u, v, i, q);
		if (u[i + n] == -1) {
			--q;
			compensation_div(u, v, i);
		}
		res.number[i] = q;
	}
	if (res.number[res.number.size() - 1] == 0)
		res.number.pop_back();
	return res;
}

const BigInt operator* (const BigInt& l, const BigInt& r)
{
	BigInt res = l.mul(r);
	if (l.sign == r.sign)
		return res;
	return -res;
}

BigInt& BigInt::operator*= (const BigInt& num)
{
	BigInt res = *this * num;
	*this = res;
	return *this;
}

const BigInt operator+ (const BigInt& l, const BigInt& r)
{
	BigInt res = l;
	res += r;
	return res;
}

const BigInt operator- (const BigInt& l, const BigInt& r)
{
	BigInt res = l;
	res -= r;
	return res;
}

const BigInt operator/ (const BigInt& l, const BigInt& r)
{
	BigInt res;

	if (r == 0) {
		throw std::invalid_argument("Divide by zero exception");
	}
	if ((l.sign == 0 && r.sign == 0 && l < r) ||
		(l.sign == 1 && r.sign == 1 && -l < -r) ||
		(l.sign == 1 && r.sign == 0 && -l < r) ||
		(l.sign == 0 && r.sign == 1 && l < -r)) {
		res = 0;
		return res;
	}
	if (r.number.size() == 1) {
		res = l;
		BigInt::short_div(res.number, r.number[0]);
	} else {
		res = l.div(r);
	}
	if (l.sign == 1 && r.sign == 1)
		res.sign = 0;
	else if (l.sign != r.sign)
		res.sign = 1;
	return res;
}

BigInt& BigInt::operator/= (const BigInt& num)
{
	if (num == 0) {
		throw std::invalid_argument("Divide by zero exception");
	}
	if ((this->sign == 0 && num.sign == 0 && *this < num) ||
		(this->sign == 1 && num.sign == 1 && -(*this) < -num)) {
		*this = 0;
		return *this;
	}
	if (num.number.size() == 1) {
		BigInt::short_div(this->number, num.number[0]);
	} else {
		*this = this->div(num);
	}
	if (this->sign == 1 && num.sign == 1)
		this->sign = 0;
	else if (this->sign != num.sign)
		this->sign = 1;
	return *this;
}

BigInt BigInt::power(const BigInt& l, const BigInt& r)
{
	if (r == 0) {
		return 1;
	}
	if (r.number[0] % 2 == 0) {
		BigInt tmp = power(l, r / 2);
		return tmp * tmp;
	}
	return l * power(l, r - 1);
}

const BigInt operator^ (const BigInt& l, const BigInt& r)
{
	BigInt res = BigInt::power(l, r);
	if (l.sign == 1) {
		if (r.number[0] % 2 == 0)
			res.sign = 0;
		else
			res.sign = 1;
	}
	return res;
}

std::ostream& operator<< (std::ostream& out, const BigInt& num)
{
	if (num.sign == 1 && !(num.number.size() == 1 && num.number[0] == 0)) {
		out << "-";
	}
	for (int i = num.number.size() - 1; i >= 0; --i) {
		out << static_cast<char>(num.number[i] + '0');
	}
	return out;
}

