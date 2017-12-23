#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

enum class Operation {
	Set, 
	Sub,
	Mul,
	Jnz
};

struct Instruction {
	Operation op;
	int64_t r1;
	int64_t r2;
	bool r1_is_register{ true };
	bool r2_is_register{ true };
};

std::vector<Instruction> load_input(const char* input)
{
	std::ifstream fs(input);
	std::string line;
	std::vector<Instruction> result;
	while (std::getline(fs, line)) {
		std::istringstream ss(line);
		Instruction inst;
		std::string op;
		ss >> op;
		if (op.compare("set") == 0) {
			inst.op = Operation::Set;
		}
		if (op.compare("sub") == 0) {
			inst.op = Operation::Sub;
		}
		else if (op.compare("mul") == 0) {
			inst.op = Operation::Mul;
		}
		else if (op.compare("jnz") == 0) {
			inst.op = Operation::Jnz;
		}


		std::string r1_str;
		ss >> r1_str;
		if (r1_str.size() == 1 && r1_str[0] >= 'a' && r1_str[0] <= 'z') {
			inst.r1 = r1_str[0] - 'a';
			inst.r1_is_register = true;
		}
		else {
			inst.r1 = std::stoi(r1_str);
			inst.r1_is_register = false;
		}
		if (ss.good()) {
			std::string r2_str;
			ss >> r2_str;
			if (r2_str.size() == 1 && r2_str[0] >= 'a' && r2_str[0] <= 'z') {
				inst.r2 = r2_str[0] - 'a';
				inst.r2_is_register = true;
			}
			else {
				inst.r2 = std::stoi(r2_str);
				inst.r2_is_register = false;
			}
		}

		result.emplace_back(std::move(inst));
	}

	return result;
}

int32_t part_1(const std::vector<Instruction>& instructions)
{
	int32_t mul_count = 0;
	int64_t registers[8] = {0};
#define VALUE(x) ((inst->x##_is_register) ? registers[inst->x] : inst->x)

	for (auto inst = instructions.begin(); inst < instructions.end() && inst >= instructions.begin(); ++inst) {
		switch (inst->op)
		{
		case Operation::Set:
			registers[inst->r1] = VALUE(r2);
			break;
		case Operation::Sub:
			registers[inst->r1] -= VALUE(r2);
			break;
		case Operation::Mul:
			registers[inst->r1] *= VALUE(r2);
			mul_count++;
			break;
		case Operation::Jnz:
			if (VALUE(r1) != 0) {
				inst += VALUE(r2);
				--inst;
			}
			break;
		default:
			break;
		}
	}
#undef VALUE
	return mul_count;
}

int64_t part_2(const std::vector<Instruction>& instructions)
{
	//Just translated the assembly into c directly.
	int32_t mul_count = 0;
	int64_t a, b, c, d, e, f, g, h;
	a = 1;
	b = 93;
	c = b;
	d = 0;
	e = 0;
	f = 0;
	g = 0;
	h = 0;
	if (a != 0) {
		b *= 100;
		b += 100000;
		c = b;
		c += 17000;
	}

	do {
		f = 1;
		d = 2;
		do {
			if (b % d == 0) {
				f = 0;
				++h;
				break;
			}
			++d;
		} while (d != b);

		g = b;
		g -= c;
		b += 17;
	} while (g != 0);

	return h;
}


int main(int, char**)
{
	auto instructions = load_input("day23.txt");
	//std::cout << part_1(instructions) << std::endl;
	printf("%d\n", part_2(instructions));
	std::getchar();
}