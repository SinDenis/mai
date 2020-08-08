/*
 * Author: Sin Denis Dmitrievich
 * Group: M80-207B-18
 * Lab04 OOP
 * Variant: 21
 * Figures: Rhombus
 * Collection: Queue
 */

#include <iostream>
#include <utility>
#include "Rhombus.h"
#include "Queue.h"

const int RHOMBUS_NUM_COORS = 4;

bool isThrombus(const std::pair<int, int>* coors)
{
	std::pair<int, int> a, b;
	a.first = (coors[0].first - coors[2].first);
	a.second = (coors[0].second - coors[2].second);
	b.first = (coors[1].first - coors[3].first);
	b.second = (coors[1].second - coors[3].second);
	int res = a.first * b.first + a.second * b.second;
	return res == 0;
}

void Insert(TQueue<TRhombus<int> >& q)
{
	std::pair<int, int> coors[RHOMBUS_NUM_COORS];
	for (int i = 0; i < RHOMBUS_NUM_COORS; ++i) {
		int x, y;
		std::cin >> x >> y;
		coors[i] = std::make_pair(x, y);
	}
	if (isThrombus(coors)) {
		q.Push(TRhombus<int>(coors[0], coors[1], coors[2], coors[3]));
		std::cout << "Rhombus is added\n";
	} else
		std::cout << "This figure isn't Rhombus\n";


}

void Remove(TQueue<TRhombus<int> >& q)
{
	int64_t idx;

	std::cin >> idx;
	if (idx >= q.Size()) {
		std::cout << "Such rhombus doesn't exist\n";
	} else {
		auto it = q.begin();
		std::advance(it, idx);
		q.Erase(it);
		std::cout << "Rhombus with idx = " << idx << " is deleted\n";
	}
}

void Print(TQueue<TRhombus<int> >& q)
{
	std::for_each(
			q.begin(),
			q.end(),
			[](const TRhombus<int>& rmb) {
				Print(rmb);
				std::cout << "\n";
	});
}

void Count(TQueue<TRhombus<int> >& q)
{
	int square;
	std::cin >> square;
	int res = std::count_if(
			q.begin(),
			q.end(),
			[square](const TRhombus<int>& rmb) -> bool {
				return Square(rmb) < square;
	});
	std::cout << res << "\n";
}

int main() {
	TQueue<TRhombus<int>> q;
	std::string cmd;

	while (std::cin >> cmd) {
		if (cmd == "insert") {
			Insert(q);
		} else if (cmd == "remove") {
			Remove(q);
		} else if (cmd == "print") {
			Print(q);
		} else if (cmd == "count") {
			Count(q);
		} else {
			std::cout << "Bad input\n";
			continue;
		}
	}
	return 0;
}