#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
namespace filesystem = std::experimental::filesystem;

std::vector<std::vector<int>> load_file(const filesystem::path& path)
{
	std::ifstream fs(path);
	std::vector<std::vector<int>> result;
	if (!fs.good()) return result;
	for (std::string line; std::getline(fs, line);) {
		result.emplace_back();

		std::stringstream ss(line);
		int temp;
		ss >> temp;
		while (!ss.fail()) {
			result.back().push_back(temp);
			ss >> temp;
			
		} 
	}

	return result;
}

int part_1(const std::vector<std::vector<int>>& data)
{
	int sum = 0;
	for (auto& row : data)
	{
		auto minmax = std::minmax_element(row.cbegin(), row.cend());
		sum += (*minmax.second - *minmax.first);
	}
	return sum;
}

int part_2(const std::vector<std::vector<int>>& data)
{
	int sum = 0;
	for (auto row : data)
	{
		//For each element in the row, need to find an element which evenly divides it.
		for (auto& x : row) {
			for (auto& y : row) {
				if (x > y && x % y == 0) {
					sum += x / y;
					goto nextrow;
				}
			}
			
		}
	nextrow:;
	}
	return sum;
}

int main(int argc, char** argv)
{
	std::vector<std::vector<int>> test = {
		{5, 9, 2, 8},
		{9, 4, 7, 3},
		{3, 8, 6, 5}
	};
	auto data = load_file("test.txt");
	std::cout << part_1(data) << std::endl;
	std::cout << part_2(data);
	std::getchar();
}