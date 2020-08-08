//
// Created by Sin Denis on 2019-09-04.
//

#ifndef OOP_EXERCISE_1_BIG_INT_H
#define OOP_EXERCISE_1_BIG_INT_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class BigInt
{
public:
	BigInt();
	BigInt(const std::string& num);
	BigInt(long long num);
	BigInt(int num);
	BigInt(const char *num);
	~BigInt() = default;
	BigInt(const BigInt& num);
	BigInt& operator= (const BigInt& num);
	BigInt operator- () const;
	BigInt& operator+= (const BigInt& num);
	BigInt& operator-= (const BigInt& num);
	BigInt& operator*= (const BigInt& num);
	BigInt& operator/= (const BigInt& num);
	BigInt& operator++ ();
	friend const BigInt operator* (const BigInt& l, const BigInt& r);
	friend const BigInt operator/ (const BigInt& l, const BigInt& r);
	friend const BigInt operator^ (const BigInt& l, const BigInt& r);
	friend bool operator< (const BigInt& l, const BigInt& r);
	friend std::ostream& operator<< (std::ostream& out, const BigInt& num);
private:
	char sign;
	static int base;
	std::vector<char> number;
	BigInt& add(const BigInt& num);
	BigInt& sub(const BigInt& num);
	BigInt mul(const BigInt& num) const;
	BigInt div(const BigInt& num) const;
	static BigInt power(const BigInt& l, const BigInt& r);
	static void short_div(std::vector<char>& u, char v);
	static void help_div(std::vector<char>& u, std::vector<char>& v, int i, int q);
	static void compensation_div(std::vector<char>& u, std::vector<char>& v, int i);
};

bool operator< (const BigInt& l, const BigInt& r);
bool operator> (const BigInt& l, const BigInt& r);
bool operator<= (const BigInt& l, const BigInt& r);
bool operator>= (const BigInt& l, const BigInt& r);
bool operator== (const BigInt& l, const BigInt& r);
bool operator!= (const BigInt& l, const BigInt& r);

const BigInt operator+ (const BigInt& l, const BigInt& r);
const BigInt operator- (const BigInt& l, const BigInt& r);
const BigInt operator* (const BigInt& l, const BigInt& r);
const BigInt operator/ (const BigInt& l, const BigInt& r);
const BigInt operator^ (const BigInt& l, const BigInt& r);

#endif //OOP_EXERCISE_1_BIG_INT_H
