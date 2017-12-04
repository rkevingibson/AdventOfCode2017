#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace filesystem = std::experimental::filesystem;

std::vector<std::vector<std::string>> load_file(const filesystem::path& path) {
	std::vector<std::vector<std::string>> result;
	std::ifstream fs(path, std::ios::in);
	for (std::string line; std::getline(fs, line); ) {
		result.emplace_back();
		std::istringstream is(line);
		std::string tok;
		while (is.good()) {
			is >> tok;
			result.back().push_back(tok);
		}
	}

	return result;
}

bool is_valid_passphrase(std::vector<std::string> pp) {
	std::sort(pp.begin(), pp.end());
	return std::unique(pp.begin(), pp.end()) == pp.end();
}

bool is_valid_passphrase_2(std::vector<std::string> pp) {
	for (auto& s : pp) {
		std::sort(s.begin(), s.end());
	}
	std::sort(pp.begin(), pp.end());
	return std::unique(pp.begin(), pp.end()) == pp.end();
}

int part_1(const std::vector<std::vector<std::string>>& data) {
	int sum = 0;
	for (auto& p : data) {
		sum += is_valid_passphrase(p);
	}
	return sum;
}
int part_2(const std::vector<std::vector<std::string>>& data) {
	int sum = 0;
	for (auto& p : data) {
		sum += is_valid_passphrase_2(p);
	}
	return sum;
}

int main(int argc, char** argv)
{
	auto passphrases = load_file("day4.txt");
	std::cout << part_1(passphrases) << std::endl;
	std::cout << part_2(passphrases) << std::endl;
	std::getchar();
}
