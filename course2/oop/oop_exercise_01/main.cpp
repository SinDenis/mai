/*
 * Author: Sin Denis Dmitrievich
 * Group: M80-207B-18
 * Variant: 1
 */

#include <iostream>
#include <string>
#include "src/big_int.h"

int main(int argc, char **argv) {
	if (argc != 4) {
		std::cout << "Using: ./oop_exercise_01 arg1 op arg2\n";
		return 0;
	}
	char op = argv[2][0];
	BigInt a(argv[1]);
	BigInt b(argv[3]);
	BigInt res;

	switch (op) {
		case '+':
			std::cout << (a + b) << std::endl;
			break;
		case '-':
			std::cout << (a - b) << std::endl;
			break;
		case '*':
			std::cout << (a * b) << std::endl;
			break;
		case '/':
			try {
				std::cout << (a / b) << std::endl;
			} catch (const std::invalid_argument& e) {
				std::cout << e.what();
			}
			break;
		case '^':
			std::cout << (a ^ b) << std::endl;
			break;
		default:
			std::cout << "Blank\n";
	}
	return 0;
}