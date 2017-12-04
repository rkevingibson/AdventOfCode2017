#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

size_t part_1(const std::vector<char>& v)
{
	size_t sum = 0;
	if (v.front() == v.back()) {
		sum += v.front();
	}
	for (auto it = v.begin(); it < v.end() - 1; ++it) {
		if (*it == *(it + 1)) {
			sum += *it;
		}
	}
	return sum;
}

size_t part_2(const std::vector<char>& v)
{
	size_t sum = 0;
	size_t size = v.size();
	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] == v[(i + size/2) % size]) {
			sum += v[i];
		}
	}
	return sum;
}

std::vector<char> load_file(const filesystem::path& filename)
{
	std::ifstream fs(filename, std::ios::binary | std::ios::ate);
	long long size = fs.tellg();
	std::vector<char> result(size);
	fs.seekg(0);
	fs.read(result.data(), result.size());
	//Transform from ascii to numerals.
	std::transform(result.begin(), result.end(), result.begin(), [](char x) { return x - '0'; });
	return result;
}

int main(int argc, char** argv)
{
	auto data = load_file("day1.txt");
	std::cout << part_1(data) << std::endl;
	std::cout << part_2(data) << std::endl;
	std::getchar();
}