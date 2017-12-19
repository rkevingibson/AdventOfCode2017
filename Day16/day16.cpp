#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <string>
#include <fstream>
#include <unordered_map>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

struct Instruction {
	enum class Op {
		Exchange,
		Spin,
		Partner
	};
	Op op;
	char p1;
	char p2;
};

using State = std::array<char, 16>;

void run_inst(State& state, const Instruction& inst)
{
	switch (inst.op)
	{
	case Instruction::Op::Spin:
		std::rotate(state.rbegin(), state.rbegin() + inst.p1, state.rend());
		break;
	case Instruction::Op::Exchange:
		std::swap(state[inst.p1], state[inst.p2]);
		break;
	case Instruction::Op::Partner:
	{
		auto i1 = std::find(state.begin(), state.end(), inst.p1);
		auto i2 = std::find(state.begin(), state.end(), inst.p2);
		std::swap(*i1, *i2);
	}
		break;
	default:
		break;
	}
}

State part_1(std::vector<Instruction> instructions)
{
	State state;
	std::iota(state.begin(), state.end(), 'a');
	for (const auto& inst : instructions) {
		run_inst(state, inst);
	}
	return state;
}

std::vector<Instruction> load_input(const char* filename)
{
	std::ifstream fs(filename);
	std::vector<Instruction> instructions;
	char inst;
	while (fs.get(inst)) {
		switch (inst)
		{
		case 'p':
		{
			char p1, p2;
			fs.get(p1);
			fs.ignore(1);
			fs.get(p2);
			instructions.emplace_back(Instruction{ Instruction::Op::Partner, p1, p2 });
		}	break;
		case 's':
		{	
			int i;
			fs >> i;
			instructions.emplace_back(Instruction{ Instruction::Op::Spin, (char)i, 0 });
		}	break;
		case 'x':
		{
			int p1, p2;
			fs >> p1;
			fs.ignore(1);
			fs >> p2;
			instructions.emplace_back(Instruction{ Instruction::Op::Exchange, (char)p1, (char)p2});
		}	break;
		default:
			break;
		}
		fs.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	}

	return instructions;
}

/*
	Part 2: I now have to perform this dance 1 billion times. Thats an awful lot. 
	I can probably just let my computer chug at it, but that's a lot of iteration. 
	What I can do is find the permutation done by the first dance, and apply that a 
	billion times? But no, that's not true, because of the partner instructions, which depend on the current state.
	Guess I just have to run it 1'000'000'000 times then.
*/


State part_2(std::vector<Instruction> instructions)
{
	State state;
	std::iota(state.begin(), state.end(), 'a');


	State initial_state = state;
	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);

	constexpr int NUM_ITERATIONS = 1'000'000'000;

	for (int i = 0; i < NUM_ITERATIONS; i++) {

		//state = apply_permutation(state, permutation);
		for (const auto& inst : instructions) {
			run_inst(state, inst);
		}

		if (initial_state == state) {
			//We have a loop from iteration 0 to i. 
			//This means that I can skip a certain number of states. 
			int loop_size = i;
			while (i < NUM_ITERATIONS) i += loop_size + 1;
			i -= loop_size + 1;
		}
	}

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	float elapsed_time = (end.QuadPart - start.QuadPart) / (float)(freq.QuadPart);
	printf("Took %f s\n", elapsed_time);
	return state;
}

int main(int argc, char** argv)
{
	auto input = load_input("day16.txt");
	auto state = part_1(input);
	std::cout << std::string(state.begin(), state.end()) << std::endl;
	

	auto p2 = part_2(input);
	std::cout << std::string(p2.begin(), p2.end()) << std::endl;
	std::getchar();
}