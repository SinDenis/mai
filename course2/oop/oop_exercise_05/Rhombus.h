//
// Created by Sin Denis on 2019-11-12.
//

#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <utility>
#include <cmath>

template <typename T1, typename T2>
std::ostream& operator<< (std::ostream& out, const std::pair<T1, T2>& p) {
	out << "(" << p.first << ", " << p.second << ")";
	return out;
}

template<class T>
struct TRhombus {
	using type = T;
	using TVertex = std::pair<T, T>;
	TVertex A, B, C, D;

	TRhombus() = default;

	TRhombus(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) :
			A(x1, y1), B(x2, y2), C(x3, y3), D(x4, y4)
	{
	}

	TRhombus(const TVertex& v1, const TVertex& v2, const TVertex& v3, const TVertex& v4) :
			A(v1), B(v2), C(v3), D(v4)
	{
	}
};

template <class T>
struct IsTRhombus {
	static constexpr bool value = false;
};

template <template <class...> class F, class T>
struct IsTRhombus<F<T>> {
	static constexpr bool value = std::is_same<F<T>, TRhombus<T>>::value;
};

template <class T>
typename std::enable_if<IsTRhombus<T>::value, typename T::type>::type
Square(const T& r) {
	typename T::type d1 = static_cast<typename T::type>(
			sqrt(pow(r.A.first - r.C.first, 2) + pow(r.A.second - r.C.second, 2))
	);
	typename T::type d2 = static_cast<typename T::type>(
			sqrt(pow(r.B.first - r.D.first, 2) + pow(r.B.second - r.D.second, 2))
	);

	return d1 * d2 / 2;
}

template <class T>
void Print(const TRhombus<T>& r) {
	std::cout << r.A << " " << r.B << " " << r.C << " " << r.D;
}

template <class T>
typename std::enable_if<IsTRhombus<T>::value, std::pair<typename T::type, typename T::type>>::type
Center(const T& r) {
	typename T::type x0 = Sum(r.A.first, r.B.first, r.C.first, r.D.first) / 4;
	typename T::type y0 = Sum(r.A.second, r.B.second, r.C.second, r.D.second) / 4;
	return std::make_pair(x0, y0);
}

#endif
