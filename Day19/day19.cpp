#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <utility>
#include <tuple>

struct Maze {
	int w;
	std::vector<char> board;
};

enum class Direction {
	N,
	S,
	E,
	W
};

std::pair<std::string, int> part_1(const Maze& maze)
{
	size_t current_pos = 0;
	//Find the starting position.
	while (maze.board[current_pos] != '|') ++current_pos;
	Direction dir = Direction::S;
	std::string result;

	auto up = [](int x, const Maze& maze) {
		return std::make_tuple(
			(x - maze.w >= 0) ? maze.board[x - maze.w] : ' ', 
			x- maze.w,
			Direction::N);
	};
	auto down = [](int x, const Maze& maze) {
		return std::make_tuple(
			(x + maze.w < maze.board.size()) ? maze.board[x + maze.w] : ' ',
			x + maze.w,
			Direction::S);
	};
	auto left = [](int x, const Maze& maze) {
		return std::make_tuple(
			(x % maze.w != 0) ? maze.board[x - 1] : ' ',
			x - 1,
			Direction::W);
	};
	auto right = [](int x, const Maze& maze) {
		return std::make_tuple(
			((x + 1) % maze.w != 0) ? maze.board[x + 1] : ' ',
			x + 1,
			Direction::E);
	};

	bool at_end = false; 
	int num_steps =0;
	while (!at_end) {
		num_steps++;
		//There's a lot of repetitive logic here, would be good to refactor a bit.
		char next_char;
		using DirFn = std::tuple<char, int, Direction>(*)(int x, const Maze& maze);
		DirFn fns[3];

		switch (dir)
		{
		case Direction::N:
			fns[0] = up;
			fns[1] = left;
			fns[2] = right;
			break;
		case Direction::S:
			fns[0] = down;
			fns[1] = left;
			fns[2] = right;
			break;
		case Direction::E:
			fns[0] = right;
			fns[1] = up;
			fns[2] = down;
			break;
		case Direction::W:
			fns[0] = left;
			fns[1] = up;
			fns[2] = down;
			break;
		default:
			break;
		}
	
		at_end = true;
		for (int i = 0; i < 3; i++) {
			char c;
			int next_pos;
			Direction next_dir;
			std::tie(c, next_pos, next_dir) = fns[i](current_pos, maze);
			if (c != ' ') {
				current_pos = next_pos;
				dir = next_dir;
				next_char = c;
				at_end = false;
				break;

			}
		}

		if (next_char >= 'A' && next_char <= 'Z') {
			result.append(1, next_char);
		}
	}
	return std::make_pair(result, num_steps);
}

Maze load_input(const char* filename)
{
	std::ifstream fs(filename);
	std::string line;
	Maze maze;
	while (std::getline(fs, line)) {
		maze.board.insert(maze.board.end(), line.begin(), line.end());
		maze.w = line.size();
	}
	return maze;
}

int main(int argc, char** argv)
{
	auto maze = load_input("day19.txt");
	auto result = part_1(maze);
	std::cout <<  result.first << std::endl;
	std::cout << result.second << std::endl;
	std::getchar();
}