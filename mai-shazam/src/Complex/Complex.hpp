#pragma once

#include <iostream>
#include <cmath>

template <class T>
class TComplex {
public:
	explicit TComplex(): Re(0), Im(0) {}

	explicit TComplex(T re, T im): Re(re), Im(im) {}
	
	explicit TComplex(T re) : Re(re), Im(0) {}

	~TComplex() = default;

	TComplex<T>& operator= (const TComplex<T>& c) {
		Re = c.Re;
		Im = c.Im;
		return *this;
	}

	T& Real() { return Re; }
	T& Image() { return Im; }

	const T& Real() const { return Re; }
	const T& Image() const { return Im; }

	TComplex operator+= (const TComplex& rhs) {
		Re += rhs.Re;
		Im += rhs.Im;
		return *this;
	}

	TComplex operator-= (const TComplex& rhs) {
		Re -= rhs.Re;
		Im -= rhs.Im;
		return *this;
	}

	TComplex operator*= (const TComplex& rhs) {
		T a = Re * rhs.Re - Im * rhs.Im;
		T b = Re * rhs.Im + Im * rhs.Re; 
		Re = a;
		Im = b;	
		return *this;
	}

	TComplex operator/= (const TComplex& rhs) {
		T denom = rhs.Re * rhs.Re + rhs.Im * rhs.Im;
		T a = (Re * rhs.Re + Im * rhs.Im) / denom;
		T b = (Im * rhs.Re - Re * rhs.Im) / denom;
		Re = a;
		Im = b;
		return *this;
	}

	T Abs() const {
		return std::sqrt(Re * Re + Im * Im);
	}

	TComplex operator+ (const TComplex& rhs) const {
		TComplex res = *this;
		res +=  rhs;
		return  res;
	}
	
	TComplex operator- (const TComplex& rhs) const {
		TComplex res = *this;
		res -= rhs;
		return res;
	}

	TComplex operator* (const TComplex& rhs) const {
		TComplex res = *this;
		res *= rhs;
		return res;
	}

	TComplex operator/ (const TComplex& rhs) const {
		TComplex res = *this;
		res /= rhs;
		return res;
	}

private:
	T Re;
	T Im;

	template <class P>
	friend std::ostream& operator<< (std::ostream& os, const TComplex<P>& c);
	
};

template <class T>
std::ostream& operator<< (std::ostream& os, const TComplex<T>& c) {
	os << "(" << c.Re << ", " << c.Im << ")";
	return os;
}
