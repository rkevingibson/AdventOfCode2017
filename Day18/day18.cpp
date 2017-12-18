#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

enum class Operation {
	Snd,
	Set,
	Add,
	Mul,
	Mod,
	Rcv,
	Jgz
};

struct Instruction {
	Operation op;
	int64_t r1;
	int64_t r2;
	bool r1_is_register{ true };
	bool r2_is_register{ true };
};

int32_t part_1(const std::vector<Instruction>& instructions)
{
	int32_t prev_freq = 0;
	bool freq_recovered = false;
	std::unordered_map<int64_t, int64_t> registers;
#define VALUE(x) ((inst->x##_is_register) ? registers[inst->x] : inst->x)

	for (auto inst = instructions.begin(); !freq_recovered && inst < instructions.end() && inst >= instructions.begin(); ++inst) {
		switch (inst->op)
		{
		case Operation::Snd:
			prev_freq = VALUE(r1);
			break;
		case Operation::Set:
			assert(inst->r1_is_register);
			registers[inst->r1] = VALUE(r2);
			break;
		case Operation::Add:
			assert(inst->r1_is_register);
			registers[inst->r1] += VALUE(r2);
			break;
		case Operation::Mul:
			assert(inst->r1_is_register);
			registers[inst->r1] *= VALUE(r2);
			break;
		case Operation::Mod:
			assert(inst->r1_is_register);
			{
				int64_t x = VALUE(r1);
				int64_t y = VALUE(r2);
				registers[inst->r1] = x % y;
			}
			break;
		case Operation::Rcv:
			if (VALUE(r1) != 0) {
				freq_recovered = true;
			}
			break;
		case Operation::Jgz:
			if (VALUE(r1) > 0) {
				inst += VALUE(r2);
				--inst;
			}
			break;
		default:
			break;
		}
	}
#undef VALUE
	return prev_freq;
}

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
		if (op.compare("snd") == 0) {
			inst.op = Operation::Snd;
		}
		else if (op.compare("set") == 0) {
			inst.op = Operation::Set;
		}
		else if (op.compare("add") == 0) {
			inst.op = Operation::Add;
		}
		else if (op.compare("mul") == 0) {
			inst.op = Operation::Mul;
		}
		else if (op.compare("mod") == 0) {
			inst.op = Operation::Mod;
		}
		else if (op.compare("rcv") == 0) {
			inst.op = Operation::Rcv;
		}
		else if (op.compare("jgz") == 0) {
			inst.op = Operation::Jgz;
		}


		std::string r1_str;
		ss >> r1_str;
		if (r1_str.size() == 1 && r1_str[0] >= 'a' && r1_str[0] <= 'z') {
			inst.r1 = r1_str[0];
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
				inst.r2 = r2_str[0];
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


int32_t part_2(const std::vector<Instruction>& instructions)
{
	//Rather than worry about multithreading, I'm just going to run one "thread", and switch whenever a thread is waiting for an entry to the queue.
	//This is simple enough, and I won't have to worry about contention or any of that other stuff.



	std::vector<int64_t> queues[2];
	auto send_count = 0;
	std::unordered_map<int64_t, int64_t> registers[2];
	registers[0]['p'] = 0;
	registers[1]['p'] = 1;

	int current_id = 0;
	int other_id = 1;

	bool thread_finished[2] = { false, false };
	bool thread_waiting[2] = { false , false };

#define VALUE(x) ((pc->x##_is_register) ? registers[current_id][pc->x] : pc->x)

	
	auto pc = instructions.begin();
	auto other_pc = instructions.begin();


	while(!thread_finished[0] && !thread_finished[1]) {


		switch (pc->op)
		{
		case Operation::Snd:
			//Send a value to the other queue.
		{
			if (current_id == 1) {
				send_count++;
			}
			queues[other_id].push_back(VALUE(r1));
		}
		break;
		case Operation::Rcv:
		{
			if (queues[current_id].empty()) {
				//Switch "threads"
				if (thread_waiting[other_id] && queues[other_id].empty()) {
					//We're going to be waiting forever. Deadlock!
					thread_finished[current_id] = true;
					continue;

				}

				thread_waiting[current_id] = true;
				std::swap(current_id, other_id);
				std::swap(pc, other_pc);
				continue;
			}

			registers[current_id][pc->r1] = queues[current_id].front();
			queues[current_id].erase(queues[current_id].begin());
		}
		break;


		case Operation::Set:
			assert(pc->r1_is_register);
			registers[current_id][pc->r1] = VALUE(r2);
			break;
		case Operation::Add:
			assert(pc->r1_is_register);
			registers[current_id][pc->r1] += VALUE(r2);
			break;
		case Operation::Mul:
			assert(pc->r1_is_register);
			registers[current_id][pc->r1] *= VALUE(r2);
			break;
		case Operation::Mod:
			assert(pc->r1_is_register);
			{
				int64_t x = VALUE(r1);
				int64_t y = VALUE(r2);
				registers[current_id][pc->r1] = x % y;
			}
			break;
		case Operation::Jgz:
			if (VALUE(r1) > 0) {
				pc += VALUE(r2);
				--pc;
			}
			break;
		default:
			break;
		}
	
		++pc;
		if (pc == instructions.end()) {
			thread_finished[current_id] = true;
		}
	}

#undef VALUE



	return send_count;
}


int main(int argc, char** argv)
{
	auto instructions = load_input("day18.txt");
	std::vector<Instruction> test_instructions = {
		Instruction{Operation::Snd, 1, 1, false, false},
		Instruction{ Operation::Snd, 2, 2, false, false },
		Instruction{ Operation::Snd, 'p', 'a', true, true},
		Instruction{ Operation::Rcv, 'a', 5, true, false },
		Instruction{ Operation::Rcv, 'b', 0, true, false },
		Instruction{ Operation::Rcv, 'c', 0, true, false },
		Instruction{ Operation::Rcv, 'd', 1, true, false }
	};

	std::cout << part_1(instructions) << std::endl;
	std::cout << part_2(instructions) << std::endl;
	std::getchar();
}