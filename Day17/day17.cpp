#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

uint32_t part_1(uint32_t num_steps)
{
	std::vector<uint32_t> buffer(1,0);
	uint32_t current_position = 0;
	buffer.reserve(2018);
	for (uint32_t i = 1; i <= 2017; ++i) {
		current_position = (current_position + num_steps) % buffer.size();
		auto p = buffer.insert(buffer.begin()+current_position + 1, i);
		current_position = std::distance(buffer.begin(), p);
	}

	return buffer[current_position + 1];
}

uint32_t part_2(uint32_t num_steps)
{
	
	uint32_t current_position = 0;
	uint32_t current_size = 1;
	uint32_t after_0_value = 1;
	for (uint32_t i = 1; i <= 50'000'000; ++i) {
		current_position = (current_position + num_steps) % current_size;
		++current_size;
		if(current_position == 0) after_0_value = i;
		++current_position;
	}

	return after_0_value;
}

int main(int argc, char** argv)
{
	std::cout << part_1(328) << std::endl;
	std::cout << part_2(328) << std::endl;
	std::getchar();
}