#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

enum class Operation {
	Inc,
	Dec,
};

enum Condition {
	LT = 0,
	GT,
	EQ,
	NEQ,
	LEQ,
	GEQ
};

struct Instruction {
	std::string reg;
	Operation op;
	int amount;
	std::string condition_reg;
	Condition condition;
	int condition_amount;
};

using SystemState = std::unordered_map<std::string, int>;

std::vector<Instruction> load_file(const char* filename)
{
	std::vector<Instruction> result;
	std::ifstream fs(filename);
	for (std::string line; std::getline(fs, line); ) {
		std::istringstream ss(line);
		std::string name;
		std::string op;
		int amount;
		std::string if_string;
		std::string comp_name;
		std::string comp;
		int condition_amount;
		ss >> name >> op >> amount >> if_string >> comp_name >> comp >> condition_amount;

		Instruction i;
		i.reg = name;
		if (op.compare("dec") == 0) {
			i.op = Operation::Dec;
		}
		else {
			i.op = Operation::Inc;
		}
		i.amount = amount;
		i.condition_amount = condition_amount;
		i.condition_reg = comp_name;

		if (comp.compare("<") == 0) {
			i.condition = Condition::LT;
		}
		else if (comp.compare(">") == 0) {
			i.condition = Condition::GT;
		}
		else if (comp.compare(">=") == 0) {
			i.condition = Condition::GEQ;
		}
		else if (comp.compare("<=") == 0) {
			i.condition = Condition::LEQ;
		}
		else if (comp.compare("==") == 0) {
			i.condition = Condition::EQ;
		}
		else if (comp.compare("!=") == 0) {
			i.condition = Condition::NEQ;
		}
		result.emplace_back(std::move(i));
	}

	return result;
};

void run_instruction(const Instruction& inst, SystemState& state)
{
	using ConditionCheck = bool(*)(int, int);
	static const ConditionCheck condition_checks[] = {
		[](int a, int b) {return a < b; },
		[](int a, int b) {return a > b; },
		[](int a, int b) {return a == b; },
		[](int a, int b) {return a != b; },
		[](int a, int b) {return a <= b; },
		[](int a, int b) {return a >= b; }
	};

	auto test_fn = condition_checks[inst.condition];
	if (test_fn(state[inst.condition_reg], inst.condition_amount)) {
		switch (inst.op)
		{
		case Operation::Dec:
			state[inst.reg] -= inst.amount;
			break;
		case Operation::Inc:
			state[inst.reg] += inst.amount;
			break;
		default:
			break;
		}
	}
}

SystemState run_code(const std::vector<Instruction>& instructions) {
	SystemState state;

	for (const auto& instruction : instructions) {
		run_instruction(instruction, state);
	}

	return state;
};

int part_1(const std::vector<Instruction>& instructions) {
	auto state = run_code(instructions);
	
	auto max = std::max_element(state.begin(), state.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
		return a.second < b.second;
	});

	return max->second;
}

int part_2(const std::vector<Instruction>& instructions) 
{
	SystemState state;
	int max = std::numeric_limits<int>::min();
	for (const auto& inst : instructions) {
		run_instruction(inst, state);
		auto new_max = std::max_element(state.begin(), state.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
			return a.second < b.second;
		});
		//Could be a lot more efficient - only need to check if the modified register is the new maximum, 
		//or if the "maximum" register was modified, do a complete check. I've kept it simple, but slower.
		//That said, it's still instantaneous on this dataset...
		max = std::max<int>({ max, new_max->second });
	}

	return max;
}

int main(int argc, char** argv)
{
	auto instructions = load_file("day8.txt");
	std::cout << part_1(instructions) << std::endl;
	std::cout <<part_2(instructions) << std::endl;
	std::getchar();
}