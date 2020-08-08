#ifndef FIGURES_H
#define FIGURES_H

#include <utility>
#include <type_traits>
#include <tuple>
#include <cmath>

template <class elem>
elem Sum(const elem& first) {
	return first;
}

template <class elem, class...elems>
elem Sum(const elem& first, const elems&...rest) {
	return first + Sum(rest...);
}

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

	TRhombus(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) :
		A(x1, y1), B(x2, y2), C(x3, y3), D(x4, y4)
	{
	}
};

template<class T>
struct TPentagon {
	using type = T;
	using TVertex = std::pair<T, T>;
	TVertex A, B, C, D, E;

	TPentagon(T x1, T y1, T x2, T y2, T x3, T y3,
			T x4, T y4, T x5, T y5) :
			A(x1, y1), B(x2, y2), C(x3, y3), D(x4, y4), E(x5, y5)
	{
	}
};

template<class T>
struct THexagon {
	using type = T;
	using TVertex = std::pair<T, T>;
	TVertex A, B, C, D, E, F;
	THexagon(T x1, T y1, T x2, T y2, T x3, T y3,
			  T x4, T y4, T x5, T y5, T x6, T y6 ) :
			A(x1, y1), B(x2, y2), C(x3, y3), D(x4, y4), E(x5, y5), F(x6, y6)
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

template <class T>
struct IsTPentagon {
	static constexpr bool value = false;
};

template <template <class...> class F, class T>
struct IsTPentagon<F<T>> {
	static constexpr bool value = std::is_same<F<T>, TPentagon<T>>::value;
};

template <class T>
typename std::enable_if<IsTPentagon<T>::value, typename T::type>::type
Square(const T& p) {
	typename T::type a = static_cast<typename T::type>(
			sqrt(pow(p.A.first - p.B.first, 2) + pow(p.A.second - p.B.second, 2))
	);
	typename T::type h = static_cast<typename T::type>(
			a / 2 * (1. / tan(37))
	);
	return static_cast<typename T::type>(a * h * 5 / 2);
}

template <class T>
void Print(const TPentagon<T>& p) {
	std::cout << p.A << " " << p.B << " " << p.C << " " << p.D << " " << p.E;
}

template <class T>
typename std::enable_if<IsTPentagon<T>::value, std::pair<typename T::type, typename T::type>>::type
Center(const T& p) {
	typename T::type x0 = Sum(p.A.first, p.B.first, p.C.first, p.D.first, p.E.first) / 5;
	typename T::type y0 = Sum(p.A.second, p.B.second, p.C.second, p.D.second, p.E.second) / 5;
	return std::make_pair(x0, y0);
}

template <class T>
struct IsTHexagon {
	static constexpr bool value = false;
};

template <template <class...> class F, class T>
struct IsTHexagon<F<T>> {
	static constexpr bool value = std::is_same<F<T>, THexagon<T>>::value;
};

template <class T>
typename std::enable_if<IsTHexagon<T>::value, typename T::type>::type
Square(const T& h) {
	typename T::type a = static_cast<typename T::type>(
			sqrt(pow(h.A.first - h.C.first, 2) + pow(h.A.second - h.C.second, 2))
	);
	return static_cast<typename T::type>(3 * sqrt(3) * pow(a, 2) / 2);
}

template <class T>
void Print(const THexagon<T>& h) {
	std::cout << h.A << " " << h.B << " " << h.C << " " << h.D << " " << h.E << " " << h.F;
}

template <class T>
typename std::enable_if<IsTHexagon<T>::value, std::pair<typename T::type, typename T::type>>::type
Center(const T& h) {
	typename T::type x0 = Sum(h.A.first, h.B.first, h.C.first, h.D.first, h.E.first, h.F.first) / 6;
	typename T::type y0 = Sum(h.A.second, h.B.second, h.C.second, h.D.second, h.E.second, h.F.second) / 6;
	return std::make_pair(x0, y0);
}

template <class T>
T Square(const std::tuple<std::pair<T, T>, std::pair<T, T>, std::pair<T, T>, std::pair<T, T>>& tRhombus) {
	T d1 = sqrt(pow(std::get<2>(tRhombus).first - std::get<0>(tRhombus).first, 2) +
			pow(std::get<2>(tRhombus).second - std::get<0>(tRhombus).second, 2));
	T d2 = sqrt(pow(std::get<1>(tRhombus).first - std::get<3>(tRhombus).first, 2) +
				pow(std::get<1>(tRhombus).second - std::get<3>(tRhombus).second, 2));
	return d1 * d2 / 2;
}

template <class T>
T Square(const std::tuple<std::pair<T, T>, std::pair<T, T>, std::pair<T, T>, std::pair<T, T>, std::pair<T, T>>& tPentagon) {
	T a = sqrt(pow(std::get<0>(tPentagon).first - std::get<1>(tPentagon).first, 2) +
			pow(std::get<0>(tPentagon).second - std::get<1>(tPentagon).second, 2));
	T h = a / 2 * (1. / tan(37));
	return a * h * 5 / 2;
}

template <class T>
T Square(const std::tuple<std::pair<T, T>, std::pair<T, T>, std::pair<T, T>, std::pair<T, T>, std::pair<T, T>, std::pair<T, T>>& tHexagon) {
	T a = sqrt(pow(std::get<0>(tHexagon).first - std::get<1>(tHexagon).first, 2) +
			pow(std::get<0>(tHexagon).second - std::get<1>(tHexagon).second, 2));
	return 3 * sqrt(3) * pow(a, 2) / 2;
}

#endif // FIGURES_H
