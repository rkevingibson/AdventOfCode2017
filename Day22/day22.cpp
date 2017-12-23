#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <bitset>
#include <string>

struct Coord {
	int32_t x;
	int32_t y;
};

struct CoordHash
{
	size_t operator()(Coord const& c) const noexcept
	{
		size_t h1 = std::hash<int32_t>{}(c.x);
		size_t h2 = std::hash<int32_t>{}(c.y);
		return h1 ^ (h2 << 1);
	}
};

bool operator==(const Coord& a, const Coord& b) {
	return a.x == b.x && a.y == b.y;
}

enum class Status {
	Clean,
	Weakened,
	Infected,
	Flagged
};

using Grid = std::unordered_map<Coord, std::array<Status, 64>, CoordHash>;

uint32_t WorldToTileCoord(const Coord& world_coord, Coord& tile_coord)
{
	tile_coord.x = (world_coord.x >= 0) ? world_coord.x / 8 : (world_coord.x + 1) / 8 - 1;
	tile_coord.y = (world_coord.y >= 0) ? world_coord.y / 8 : (world_coord.y + 1) / 8 - 1;
	uint32_t tile_x = world_coord.x - 8 * tile_coord.x;
	uint32_t tile_y = world_coord.y - 8 * tile_coord.y;
	return tile_x + 8*tile_y;
}

void print_grid(Grid& grid, Coord center, int radius)
{
	Coord pos;
	pos.x = center.x - radius;
	pos.y = center.y + radius;
	for (int y = 0; y < 2 * radius + 1; y++, pos.y--) {
		for (int x = 0; x < 2 * radius + 1; x++, pos.x++) {
			Coord tile_coord;
			auto index = WorldToTileCoord(pos, tile_coord);
			switch (grid[tile_coord][index])
			{
			case Status::Clean:
				std::cout << ". ";
				break;
			case Status::Infected:
				std::cout << "# ";
				break;
			default:
				break;
			}
		}
		std::cout << '\n';
		pos.x = center.x - radius;
	}
	std::cout << '\n';
}

int part_1(Grid grid)
{
	enum Direction {
		Up,
		Right,
		Down,
		Left
	};

	Direction dir = Up;
	Coord position;
	position.x = 0;
	position.y = 0;

	Coord prev_tile_coord{ 0,0 };

	int num_infections = 0;
	for (int i = 0; i < 10'000; ++i) {
		//print_grid(grid, { 0,0 }, 3);
		Coord tile_coord;
		auto tile_index = WorldToTileCoord(position, tile_coord);
		auto& current_tile = grid[tile_coord];

		if (current_tile[tile_index] == Status::Infected) {
			current_tile[tile_index] = Status::Clean;
			switch (dir)
			{
			case Up:
				dir = Right;
				break;
			case Right:
				dir = Down;
				break;
			case Down:
				dir = Left;
				break;
			case Left:
				dir = Up;
				break;
			}
		}
		else {
			current_tile[tile_index] = Status::Infected;
			switch (dir)
			{
			case Up:
				dir = Left;
				break;
			case Right:
				dir = Up;
				break;
			case Down:
				dir = Right;
				break;
			case Left:
				dir = Down;
				break;
			}

			++num_infections;
		}


		switch (dir)
		{
		case Up:
			++position.y;
			break;
		case Right:
			++position.x;
			break;
		case Down:
			--position.y;
			break;
		case Left:
			--position.x;
			break;
		default:
			break;
		}


	}

	return num_infections;
}


int part_2(Grid grid)
{
	enum Direction {
		Up = 0,
		Right,
		Down,
		Left
	};

	Direction dir = Up;
	Coord position;
	position.x = 0;
	position.y = 0;

	Coord prev_tile_coord{ 0,0 };

	int num_infections = 0;
	for (int i = 0; i < 10'000'000; ++i) {
		//print_grid(grid, { 0,0 }, 3);
		Coord tile_coord;
		auto tile_index = WorldToTileCoord(position, tile_coord);
		auto& current_tile = grid[tile_coord];

		switch (current_tile[tile_index])
		{
		case Status::Infected:
		{
			current_tile[tile_index] = Status::Flagged;
			switch (dir)
			{
			case Up:
				dir = Right;
				break;
			case Right:
				dir = Down;
				break;
			case Down:
				dir = Left;
				break;
			case Left:
				dir = Up;
				break;
			}
		}
		break;
		case Status::Clean:
		{
			current_tile[tile_index] = Status::Weakened;
			switch (dir)
			{
			case Up:
				dir = Left;
				break;
			case Right:
				dir = Up;
				break;
			case Down:
				dir = Right;
				break;
			case Left:
				dir = Down;
				break;
			}

		}
			break;
		case Status::Weakened:
		{
			current_tile[tile_index] = Status::Infected;
			++num_infections;
		}
			break;
		case Status::Flagged:
		{
			current_tile[tile_index] = Status::Clean;
			switch (dir)
			{
			case Up:
				dir = Down;
				break;
			case Right:
				dir = Left;
				break;
			case Down:
				dir = Up;
				break;
			case Left:
				dir = Right;
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}

		switch (dir)
		{
		case Up:
			++position.y;
			break;
		case Right:
			++position.x;
			break;
		case Down:
			--position.y;
			break;
		case Left:
			--position.x;
			break;
		default:
			break;
		}


	}

	return num_infections;
}

Grid load_input(const char* filename)
{
	std::fstream fs(filename);
	std::string line;
	std::getline(fs, line);
	int initial_dim = line.size();
	Coord pos;
	pos.x = -initial_dim /2;
	pos.y = initial_dim /2;
	Grid grid;
	do {
		
		for (auto c : line) {
			Coord tile_coord;
			uint32_t tile_index = WorldToTileCoord(pos, tile_coord);
			grid[tile_coord][tile_index] = (c == '#' ? Status::Infected : Status::Clean);
			++pos.x;
		}
		--pos.y;
		pos.x = -initial_dim / 2;
	} while(std::getline(fs, line));

	return grid;
}

int main(int, char**)
{
	auto grid = load_input("day22.txt");
	

	std::cout << part_1(grid) << std::endl;
	std::cout << part_2(grid) << std::endl;
	std::getchar();
}