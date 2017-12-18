#include <iostream>
#include <vector>

constexpr uint64_t INITIAL_A = 516;
constexpr uint64_t INITIAL_B = 190;

uint64_t generator_a(uint64_t prev_value)
{
	return (prev_value * 16807) % 2147483647;
}

uint64_t generator_b(uint64_t prev_value)
{
	return (prev_value * 48271) % 2147483647;
}

uint32_t part_1()
{
	uint32_t count = 0;
	uint64_t a = INITIAL_A;
	uint64_t b = INITIAL_B;
	for (uint32_t i = 0; i < 40'000'000; ++i) {
		a = generator_a(a);
		b = generator_b(b);
		count += ((a & 0xFFFF) == (b & 0xFFFF));
	}

	return count;
}

uint32_t part_2()
{
	uint32_t count = 0;
	uint64_t a = INITIAL_A;
	uint64_t b = INITIAL_B;
	for (uint32_t i = 0; i < 5'000'000; ++i) {
		do {
			a = generator_a(a);
		} while(a % 4 != 0);
		do {
			b = generator_b(b);
		} while(b % 8 != 0);
		count += ((a & 0xFFFF) == (b & 0xFFFF));
	}

	return count;
}

int main(int argc, char** argv)
{
	std::cout << part_1() << std::endl;
	std::cout << part_2() << std::endl;
	std::getchar();
}