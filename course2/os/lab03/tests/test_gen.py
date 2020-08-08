#!/usr/bin/env python3
import random
import sys


def matrix_gen(h, w):
	for i in range(h):
		for j in range(w):
			print(random.randint(-10000, 10000), end=" ")
		print()


def test_gen(h, w):
	print(random.randint(1, 10))
	print(random.randint(2, 10), random.randint(2, 10), h, w)
	matrix_gen(h, w)


def main():
	matrix_height = int(sys.argv[1])
	matrix_widht = int(sys.argv[2])
	test_gen(matrix_height, matrix_widht)


if __name__ == "__main__":
	main()
