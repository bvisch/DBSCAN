#include "clustering.hpp"

#include <iostream>
using namespace clustering;

int dist(int const& lhs, int const& rhs) {
	return lhs - rhs;
}

int main() {

	int* arr = new int[10]{ 1,2,3,3,2,9,9,9,9,10 };
	std::vector<cluster<int>> clusters = DBSCAN(arr, 10, 2, 3, &dist);

	delete arr;

	for (cluster<int> c : clusters) {
		for (int i : c) {
			std::cout << i;
		}
		std::cout << std::endl;
	}
}