#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

std::pair<int, int> part_1(std::vector<int> memory_banks)
{
	auto hash = [](const std::vector<int>& x) noexcept -> size_t {
		size_t h = 0;
		std::hash<int> f;
		for (auto& i : x) h += f(i);
		return h;
	};

	std::unordered_map<std::vector<int>, int, decltype(hash)> set(memory_banks.size(), hash);

	int number_iterations = 0;
	do {
		//Do a redistribution cycle.
		set.insert(std::make_pair(memory_banks, number_iterations));
		auto max = std::max_element(memory_banks.begin(), memory_banks.end());
		int num_blocks = *max;
		*max = 0;
		++max;
		//Could do this in a single pass, but probably more work than necessary.
		for (; num_blocks != 0; max = (max + 1), --num_blocks) {
			if (max == memory_banks.end()) max = memory_banks.begin();
			++(*max);
		}

		++number_iterations;
	} while (set.count(memory_banks) == 0);

	int loop_size = number_iterations - set[memory_banks];

	return std::make_pair(number_iterations, loop_size);
}



int main(int argc, char** argv)
{
	std::vector<int> test = { 0, 2, 7, 0 };
	std::vector<int> data = { 4, 1,15,12,0,9,9,5,5,8,7,3,14,5,12,3 };
	auto result = part_1(data);

	std::cout << result.first << "\n" << result.second << std::endl;
	std::getchar();
}