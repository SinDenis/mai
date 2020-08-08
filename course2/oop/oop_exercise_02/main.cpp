/*
 * Author: Sin Denis Dmitrievich
 * Group: M80-207B-18
 * Variant: 1
 * Class Complex
 */


#include <iostream>
#include "src/complex.h"

int main() {
	char op;
	double x1, x2, y1, y2;

	std::cin >> op >> x1 >> y1;

	Complex A{x1, y1};
	Complex B;
	if (op != '~') {
		std::cin >> x2 >> y2;
		B = Complex(x2, y2);
	}


	switch (op) {
		case '+':
			std::cout << A + B << std::endl;
			break;
		case '-':
			std::cout << A - B << std::endl;
			break;
		case '*':
			std::cout << A * B << std::endl;
			break;
		case '/':
			try {
				std::cout << A / B << std::endl;
			} catch (const std::logic_error& err) {
				std::cout << err.what() << std::endl;
			}
			break;
		case '~':
			std::cout << A.conj() << std::endl;
			break;
		default:
			std::cout << "Blank" << std::endl;
	}
	return 0;
}
