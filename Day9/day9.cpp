#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>

/*
	Our goal is to find the total number of groups, including nesting.
	We definitely have some recursive structure.

*/
struct Group
{
	std::string_view view;
	int level;
};

static int g_garbage_count = 0;

const char* parse_garbage(const char* text, const char* end)
{
	while (text < end) {
		if (*text == '!') {
			++text; //skip the next character
		}
		else if (*text == '>') {
			break;
		}
		else {
			g_garbage_count++;
		}
		++text;
	}
	return text;
}

const char* parse_group(const char* text, const char* end, int parent_level, std::vector<Group>& groups)
{
	
	auto& group = groups.emplace_back();
	group.level = parent_level + 1;
	const char* group_start = text;
	while (text != end) {
		if (*text == '<') {
			//Parse garbage.
			text = parse_garbage(text + 1, end);
		}
		else if (*text == '{') {
			text = parse_group(text + 1, end, parent_level + 1, groups);
		}
		else if (*text == '}') {
			groups.back().view = std::string_view(group_start, text-group_start);
			break;
		}
		++text;
	}

	return text;
}

int part_1(const std::string& text) 
{
	if (text[0] != '{') {
		throw std::exception("Bad input");
	}
	std::vector<Group> groups;
	parse_group(text.data() + 1, text.data() + text.size(), 0, groups);

	int sum = 0;
	for (auto& g : groups) {
		sum += g.level;
	}

	return sum;
}

std::string load_file(const char* filename)
{
	std::ifstream fs(filename);
	std::string result;
	std::getline(fs, result);
	return result;
}

int main(int argc, char** argv)
{
	std::cout << part_1("{}") << std::endl;
	std::cout << part_1("{{{}}}") << std::endl;
	std::cout << part_1("{{},{}}") << std::endl;
	std::cout << part_1("{{{},{},{{}}}}") << std::endl;
	std::cout << part_1("{<a>,<a>,<a>,<a>}") << std::endl;
	std::cout << part_1("{{<ab>},{<ab>},{<ab>},{<ab>}}") << std::endl;
	std::cout << part_1("{{<!!>},{<!!>},{<!!>},{<!!>}}") << std::endl;
	std::cout << part_1("{{<a!>},{<a!>},{<a!>},{<ab>}}") << std::endl;
	g_garbage_count = 0;
	auto data = load_file("day9.txt");
	std::cout << part_1(data) << std::endl;
	std::cout << g_garbage_count << std::endl;
	std::getchar();
}