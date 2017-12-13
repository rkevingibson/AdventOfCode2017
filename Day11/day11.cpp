#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

enum class Direction : uint8_t {
	N = 0, 
	S, 
	NE, 
	NW, 
	SE, 
	SW
};

int part_1(std::vector<Direction> steps, int* _max_distance = nullptr) 
{

	//Now, certain operations "cancel" each other out.
	//E.g. opposite directions cancel out - N-S, NE-SW, NW-SE.
	//So I can compute my coordinates along the 3 primary directions. Then what do I do with those?
	//Some can get further combined. E.g. A NW and a S combine to create a SW. So I can do some arithmetic on those pretty easily. 
	//https://www.redblobgames.com/grids/hexagons/ Is a good reference - 
	//using the cube coordinates he mentions, it seems like I can compute my distance instantly.
	//They have a nice invariant property - x + y + z = 0; This means that for every step I take away from the origin,
	//the sum of the absolute values goes up by 2. So I can sum |x|+|y|+|z| and divide by two and get my solution.
	int x = 0, y = 0, z = 0;
	int max_distance = 0;
	for (auto& s : steps) {
		switch (s)
		{
		case Direction::N:
			++y;
			--z;
			break;
		case Direction::S:
			--y;
			++z;
			break;
		case Direction::NW:
			++y;
			--x;
			break;
		case Direction::SE:
			--y;
			++x;
			break;
		case Direction::SW:
			++z;
			--x;
			break;
		case Direction::NE:
			--z;
			++x;
			break;
		}

		int distance = abs(x) + abs(y) + abs(z);
		max_distance = std::max({distance, max_distance});
	}

	//This should all get reduced to bitwise stuff.
	if (_max_distance) *_max_distance = max_distance/2;
	return (abs(x) + abs(y) + abs(z)) / 2;
}

std::vector<Direction> load_file(const char* filename)
{
	std::ifstream fs(filename, std::ios::ate);
	size_t size = fs.tellg();
	std::vector<char> file(size+1, 0);
	
	fs.seekg(0);
	fs.read(file.data(), file.size()-1);

	std::vector<Direction> result;
	char* token = strtok(file.data(), ",");
	while (token != nullptr) {
		if (token[0] == 's') {
			if (token[1] == 'e') {
				result.push_back(Direction::SE);
			}
			else if (token[1] == 'w') {
				result.push_back(Direction::SW);
			}
			else {
				result.push_back(Direction::S);
			}
		}
		else { //token[0] == 'n'
			if (token[1] == 'e') {
				result.push_back(Direction::NE);
			}
			else if (token[1] == 'w') {
				result.push_back(Direction::NW);
			}
			else {
				result.push_back(Direction::N);
			}
		}
		token = strtok(NULL, ",");
	}

	return result;
}

int main(int argc, char** argv)
{
	auto directions = load_file("day11.txt");
	int part_2 = 0;
	std::cout << part_1(directions, &part_2) << std::endl;
	std::cout << part_2 << std::endl;
	std::getchar();
}