#include <vector>
#include <fstream>
#include <iostream>

std::vector<int> load_file(const char* filename)
{
	std::ifstream fs(filename);
	std::vector<int> result;
	while (fs.good()) {
		int n;
		fs >> n;
		result.push_back(n);
	}
	return result;
}

int part_1(std::vector<int> instructions)
{
	int steps = 0;
	auto it = instructions.begin();
	while (it < instructions.end()) {
		auto next = it + *it;
		(*it)++;
		it = next;
		++steps;
	}

	return steps;
}

int part_2(std::vector<int> instructions)
{
	int steps = 0;
	auto it = instructions.begin();
	while (it < instructions.end() && it >= instructions.begin()) {
		auto next = it + *it;
		if (*it >= 3) {
			(*it)--;
		}
		else {
			(*it)++;
		}
		it = next;
		++steps;
	}

	return steps;
}


int main(int argc, char** argv)
{
	std::vector<int> data = {0, 3, 0, 1, -3};
	data = load_file("day5.txt");
	std::cout << part_1(data) << std::endl;
	std::cout << part_2(data) << std::endl;
	std::getchar();
}