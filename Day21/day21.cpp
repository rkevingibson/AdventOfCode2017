#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <bitset>
#include <iostream>

using u16 = uint_fast16_t;
using Codebook = std::unordered_map<u16, u16>;
//These rotations and shuffles were found using http://programming.sirrida.de/calcperm.php

u16 rot_2x2(u16 x)
{
	return ((x & 0x00000004) << 1)
		| ((x & 0x00000001) << 2)
		| ((x & 0x00000008) >> 2)
		| ((x & 0x00000002) >> 1);
}

u16 flip_2x2(u16 x)
{
	constexpr u16 mask = 0x5555;
	return ((x & mask) << 1) | ((x >> 1) & mask);
}

u16 rot_3x3(u16 x)
{
	return (x & 0x00000010)
		| ((x & 0x00000021) << 2)
		| ((x & 0x00000002) << 4)
		| ((x & 0x00000004) << 6)
		| ((x & 0x00000040) >> 6)
		| ((x & 0x00000080) >> 4)
		| ((x & 0x00000108) >> 2);
}

u16 flip_3x3(u16 x)
{
	return (x & 0x00000092)
		| ((x & 0x00000049) << 2)
		| ((x & 0x00000124) >> 2);
}

u16 extract_blocks(u16 x)
{
	//Rearranges bits from a 4x4 pattern into 4 2x2 patterns, one after another.
	return (x & 0x0000c3c3)
		| ((x & 0x00000c0c) << 2)
		| ((x & 0x00003030) >> 2);
}

void load_input(const char* filename, Codebook& codebook2x2, Codebook& codebook3x3)
{
#define BIT(x) (u16)(x == '#')
	std::ifstream fs(filename);
	std::string line;
	while (std::getline(fs, line)) {
		bool is_2x2 = line.find('/', 0) == 2;
		u16 input, output;
		if (is_2x2) {
			assert(line.size() > 18);
			input = BIT(line[0])
				| (BIT(line[1]) << 1)
				| (BIT(line[3]) << 2)
				| (BIT(line[4]) << 3);

			output = BIT(line[9])
				| (BIT(line[10]) << 1)
				| (BIT(line[11]) << 2)
				| (BIT(line[13]) << 3)
				| (BIT(line[14]) << 4)
				| (BIT(line[15]) << 5)
				| (BIT(line[17]) << 6)
				| (BIT(line[18]) << 7)
				| (BIT(line[19]) << 8);
		}
		else {
			input = BIT(line[0])
				| (BIT(line[1]) << 1)
				| (BIT(line[2]) << 2)
				| (BIT(line[4]) << 3)
				| (BIT(line[5]) << 4)
				| (BIT(line[6]) << 5)
				| (BIT(line[8]) << 6)
				| (BIT(line[9]) << 7)
				| (BIT(line[10]) << 8);

			output = BIT(line[15])
				| (BIT(line[16]) << 1)
				| (BIT(line[17]) << 2)
				| (BIT(line[18]) << 3)
				| (BIT(line[20]) << 4)
				| (BIT(line[21]) << 5)
				| (BIT(line[22]) << 6)
				| (BIT(line[23]) << 7)
				| (BIT(line[25]) << 8)
				| (BIT(line[26]) << 9)
				| (BIT(line[27]) << 10)
				| (BIT(line[28]) << 11)
				| (BIT(line[30]) << 12)
				| (BIT(line[31]) << 13)
				| (BIT(line[32]) << 14)
				| (BIT(line[33]) << 15);
		}


		if (is_2x2) {
			codebook2x2[input] = output;
			for (int i = 0; i < 3; ++i) {
				input = rot_2x2(input);
				if (codebook2x2.count(input) == 0) {
					codebook2x2[input] = output;
				}
			}
			input = flip_2x2(input);
			for (int i = 0; i < 4; ++i) {
				input = rot_2x2(input);
				if (codebook2x2.count(input) == 0) {
					codebook2x2[input] = output;
				}
			}
		}
		else {
			codebook3x3[input] = output;
			for (int i = 0; i < 3; ++i) {
				input = rot_3x3(input);
				if (codebook3x3.count(input) == 0) {
					codebook3x3[input] = output;
				}
			}
			input = flip_3x3(input);
			for (int i = 0; i < 4; ++i) {
				input = rot_3x3(input);
				if (codebook3x3.count(input) == 0) {
					codebook3x3[input] = output;
				}
			}
		}
	}
#undef BIT
}

u16 to_bits(char* start, int num)
{
	u16 x = 0;
	for (int i = 0; i < num; ++i) {
		x |= (start[i] << i);
	}

	return x;
}

u16 get_block (const std::vector<bool>& vector, int x, int y, int dim, int block_size) 
{
	u16 s = 0;
	u16 result = 0;
	for (int j = 0; j < block_size; ++j) {
		for (int i = 0; i < block_size; ++i, ++s) {
			u16 val = vector[x + i + (y + j)*dim];
			result |= (val << s);
		}
	};
	return result;
};

void set_block(std::vector<bool>& vector, int x, int y, int dim, int block_size, u16 block) 
{
	u16 s = 0;
	for (int j = 0; j < block_size; ++j) {
		for (int i = 0; i < block_size; ++i) {
			vector[x + i + (y + j)*dim] = block & 0x1;
			block >>= 1;
		}
	}
};

void print_grid(const std::vector<bool>& grid, int dim)
{
	for (int y = 0; y < dim; ++y) {
		for (int x = 0; x < dim; ++x) {
			if (grid[x + dim * y]) {
				std::cout << "#";
			}
			else {
				std::cout << '.';
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void print_rule(u16 input, u16 output, int dim)
{
	std::vector<bool> input_list(dim*dim);
	set_block(input_list, 0, 0, dim, dim, input);
	std::vector<bool> output_list((dim+1)*(dim+1));
	set_block(output_list, 0, 0, dim+1, dim+1, output);

	for (int i = 0; i < dim*dim; ++i) {
		if (i % dim == 0 && i != 0) std::cout << '/';
		std::cout << ((input_list[i]) ? '#' : '.');
		
	}
	std::cout << " => ";
	for (int i = 0; i < (dim + 1)*(dim + 1); ++i) {
		if (i % (dim+1) == 0 && i != 0) std::cout << '/';
		std::cout << ((output_list[i]) ? '#' : '.');
	}
	std::cout << '\n';
}

int part_1(const Codebook& codebook2x2, const Codebook& codebook3x3)
{

	std::vector<bool> blocks{ 0, 1, 0, 0, 0, 1, 1, 1, 1 };

	int dimension = 3; // Dimension, in bits
	print_grid(blocks, dimension);



	for (int i = 0; i < 18; ++i) {
		if (dimension % 2 == 0) {
			int num_blocks = dimension / 2;
			std::vector<bool> next_bits(9 * num_blocks*num_blocks);
			//Break into 2x2 squares, and then reassemble.
			for (int x = 0; x < num_blocks; ++x) {
				for (int y = 0; y < num_blocks; ++y) {
					//Need to pull out a 2x2 block, convert it, and insert it into a copy.
					u16 block = get_block(blocks, 2*x, 2*y, dimension, 2);
					set_block(next_bits, 3 * x, 3 * y, 3*num_blocks, 3, codebook2x2.at(block));
					//print_rule(block, codebook2x2.at(block), 2);
				}
			}

			dimension = 3 * num_blocks;
			blocks = next_bits;
		}
		else {
			int num_blocks = dimension / 3;
			std::vector<bool> next_bits(16 * num_blocks*num_blocks);
			for (int x = 0; x < num_blocks; ++x) {
				for (int y = 0; y < num_blocks; ++y) {
					u16 block = get_block(blocks, 3 * x, 3 * y, dimension, 3);
					u16 fourbyfour = codebook3x3.at(block);
					//print_rule(block, fourbyfour, 3);
					u16 b = extract_blocks(fourbyfour);
					u16 output[4];
					for (int i = 0; i < 4; ++i) {
						output[i] = (b >> (4 * i)) & 0xF;
					}
					set_block(next_bits, 4* x, 4 * y, 4*num_blocks, 2, output[0]);
					set_block(next_bits, 4 * x + 2, 4 * y, 4 * num_blocks, 2, output[1]);
					set_block(next_bits, 4 * x, 4 * y + 2, 4 * num_blocks, 2, output[2]);
					set_block(next_bits, 4 * x + 2, 4 * y + 2, 4 * num_blocks, 2, output[3]);
				}
			}
			dimension = 4 * num_blocks;
			blocks = next_bits;

		}

		//print_grid(blocks, dimension);
	}

	//Now, just count the bits.

	int num_bits = 0;
	for (auto b : blocks) {
		num_bits += b;
	}


	return num_bits;
}

int main(int, char**)
{
	Codebook cb2x2, cb3x3;
	load_input("day21.txt", cb2x2, cb3x3);

	
	auto count = part_1(cb2x2, cb3x3);
	std::cout << count << std::endl;
	std::getchar();

}