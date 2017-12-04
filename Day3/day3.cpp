#include <algorithm>
#include <iostream>
#include <vector>

int part_1(int i)
{
	i -= 1;//Convert to 0-based position for the spiral.
	if (i == 0) return 0;
	//First, find out what "shell" the number is on.

	auto get_shell_start = [](int n) {
		int t = 2 * n - 1;
		return t*t;
	};
	int n = 1;
	while (i > get_shell_start(n)) {
		++n;
	}
	--n;


	int lowest_index_on_shell = get_shell_start(n);
	int x, y;

	if (i <= lowest_index_on_shell + 2 * n - 1) {
		//Index is on the right-hand side of the shell
		x = n;
		y = (i - lowest_index_on_shell) + (1 - n);
	}
	else if (i <= lowest_index_on_shell + 4 * n - 1) {
		//Index is on the top of the shell
		x = n - (i - (lowest_index_on_shell + 2 * n - 1));
		y = n;
	}
	else if (i <= lowest_index_on_shell + 6 * n - 1) {
		//Index is on the left side of the shell 
		x = -n;
		y = n - (i - (lowest_index_on_shell + 4 * n - 1));
	}
	else {
		//Index is on the bottom of the shell. 
		y = -n;
		x = -n + (i - (lowest_index_on_shell + 6 * n - 1));
	}

	return abs(x) + abs(y);
}

int compute_linear_index(int x, int y)
{
	if (x == 0 && y == 0) return 0;
	int shell = std::max({ abs(x), abs(y) });
	int index = 0;
	if (abs(y) == shell) {
		if (y < 0) {
			//On bottom of shell
			index = 4 * shell*shell + 4 * shell + (x - shell);
		}
		else {
			//On top of shell.
			index = 4 * shell*shell - (x + shell);
		}
	}
	else {
		if (x < 0) {
			//On left side of shell.
			index = 4 * shell*shell + (shell - y);
		}
		else {
			//On right side
			index = 4 * shell*shell - 2 * shell - (shell - y);
		}
	}
	return index;
}

int part_2(int i)
{
	//This part is way harder. Don't need just x and y, need to actually fill up a grid, I think. Need a function that can compute a linear index from the x and y coordinates. Then I can fill up an array.
	std::vector<int> grid{ 1 };
	int x = 1, y = 0;
	int index = 1;
	int side = 1;

	while (true)
	{
		int sum = 0;
		//indices for the 8 neighbours. In practice, at least 4 of these aren't necessary to compute, but eh.
		int ni[] = {
			compute_linear_index(x - 1, y),
			compute_linear_index(x, y - 1),
			compute_linear_index(x - 1, y - 1),
			compute_linear_index(x + 1, y),
			compute_linear_index(x, y + 1),
			compute_linear_index(x + 1, y + 1),
			compute_linear_index(x + 1, y - 1),
			compute_linear_index(x - 1, y + 1),
		};

		for (auto& n : ni) {
			if (n < index) {
				sum += grid[n];
			}
		}
		if( sum > i) return sum;
		grid.push_back(sum);
		index++;
		//Need to move around the grid, incrementing x and y accordingly.
		if (/*on right side*/side == 1) {
			y++;
			if (y == x) {
				//On top now.
				side++;
			}
		}
		else if (/*On top*/side == 2) {
			x--;
			if (-x == y) {
				//On left side now
				side++;
			}
		}
		else if (/*On left*/side == 3) {
			y--;
			if (y == x) {
				//On bottom now
				side++;
			}
		}
		else {
			x++;
			if (x == -y + 1) {
				//On right now
				side = 1;
			}
		}

	}




	return 0;
}


int main(int argc, char** argv)
{
	std::cout << "Part 1" << std::endl;
	std::cout << part_1(1) << std::endl;
	std::cout << part_1(12) << std::endl;
	std::cout << part_1(23) << std::endl;
	std::cout << part_1(1024) << std::endl;
	std::cout << part_1(265149) << std::endl;
	std::cout << "Part 2" << std::endl;
	std::cout << part_2(1) << std::endl;
	std::cout << part_2(2) << std::endl;
	std::cout << part_2(3) << std::endl;
	std::cout << part_2(4) << std::endl;
	std::cout << part_2(5) << std::endl;
	std::cout << part_2(265149) << std::endl;
	std::getchar();
}