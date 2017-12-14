#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <intrin.h>

template<typename T>
class ring_iterator
{
public:
	using self_type = ring_iterator;
	using value_type = T;
	using reference = std::add_lvalue_reference_t<value_type>;
	using pointer = std::add_pointer_t<value_type>;
	using const_pointer = std::add_const_t<pointer>;
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
private:
	pointer first{ nullptr };
	pointer last{ nullptr };
	pointer cur{ nullptr };
public:


	ring_iterator() = default;
	ring_iterator(pointer _cur, pointer _first, pointer _last) : first{ _first }, last{ _last }, cur{ _cur } {}

	reference operator*() const
	{
		return *cur;
	}

	pointer operator->() const noexcept
	{
		return cur;
	}

	std::add_lvalue_reference_t<self_type> operator++()
	{
		cur = cur + 1 == last ? first : cur + 1;
		return *this;

	}
	std::add_lvalue_reference_t<self_type> operator++(int) {
		auto i = *this;
		cur = cur + 1 == last ? first : cur + 1;
		return i;
	}

	std::add_lvalue_reference_t<self_type> operator--() {
		cur = (cur == first) ? last - 1 : cur - 1;
		return *this;
	}

	std::add_lvalue_reference_t<self_type> operator--(int) {

		auto i = *this;
		cur = (cur == first) ? last - 1 : cur - 1;
		return i;
	}

	friend bool operator==(const ring_iterator& a, const ring_iterator& b)
	{
		return a.cur == b.cur && a.first == b.first && a.last == b.last;
	}
	friend bool operator!=(const ring_iterator& a, const ring_iterator& b)
	{
		return !(a == b);
	}


};

void run_shuffle(const std::vector<unsigned char>& lengths, std::vector<uint8_t>& list, int* final_pos = nullptr, int* final_skip_size = nullptr)
{
	int list_size = list.size();
	int skip_size = final_skip_size ? *final_skip_size : 0;
	int pos = final_pos ? *final_pos : 0;
	uint8_t *start = list.data();
	uint8_t *end = list.data() + list_size;
	for (auto length : lengths) {
		int prev_pos = pos;

		std::reverse(
			ring_iterator<uint8_t>(&list[pos], start, end),
			ring_iterator<uint8_t>(&list[(pos + length) % list_size], start, end));
		pos = (prev_pos + length + skip_size) % list_size;
		skip_size++;

	}
	if (final_pos) *final_pos = pos;
	if (final_skip_size) *final_skip_size = skip_size;
}

std::array<uint8_t, 16> knot_hash(const std::string& string)
{
	std::vector<uint8_t> list(256);
	std::iota(list.begin(), list.end(), 0);

	std::vector<unsigned char> lengths(string.size());
	std::transform(string.begin(), string.end(), lengths.begin(), [](const unsigned char c) { return c; });
	lengths.insert(lengths.end(), { 17,31,73,47,23 });


	int pos = 0;
	int skip_size = 0;
	for (int i = 0; i < 64; i++) {
		run_shuffle(lengths, list, &pos, &skip_size);
	}
	//Now, use xor to compress the data.
	int i = 0;
	std::array<uint8_t, 16> hash;
	for (auto& h : hash) h = 0;
	while (i < 256) {
		for (int j = 0; j < 16; j++, i++) {
			hash[i / 16] ^= list[i];
		}
	}

	return hash;
}

uint32_t count_bits(const std::array<uint8_t, 16>& hash)
{
	uint32_t const *data = (const uint32_t*)hash.data();
	int size = 16 / 4;
	uint32_t count = 0;
	for (int i = 0; i < size; ++i) {
		count += __popcnt(data[i]);
	}
	return count;
}

uint32_t part_1(const std::string& key)
{
	uint32_t sum = 0;
	for (int i = 0; i < 128; ++i) {
		std::string word = key + '-' + std::to_string(i);
		auto hash = knot_hash(word);
		sum += count_bits(hash);
	}
	return sum;
}

uint32_t part_2(const std::string& key)
{
	/*
		Need to count regions. This was one of my microsoft interview questions. It's kind of crappy to do, but
		the easiest algorithm is a greedy approach. I'll just implement that.
		For simplicity, I turn our hashes into a 128x128 image vector.
	*/
	uint32_t sum = 0;
	std::vector<uint8_t> image(128*128);
	int index = 0;
	for (int i = 0; i < 128; ++i) {
		std::string word = key + '-' + std::to_string(i);
		auto hash = knot_hash(word);
		//Need to convert each byte into 8 bytes, based on which bits are set. 
		for (auto h : hash) {
			for (int8_t b = 7; b >= 0; --b) {
				image[index] = (h >> b) & 1;
				++index;
			}
		}
	}
	//Now we should have a 128*128 image array. 
	index = 0;
	int num_regions = 0;
	auto up = [](int index) -> int {
		if (index < 128) return -1;
		return index - 128;
	};
	auto down = [](int index) -> int {
		if (index + 128 > 128 * 128) return -1;
		return index + 128;
	};
	auto left = [](int index) -> int {
		if (index % 128 == 0) return -1;
		return index - 1;
	};
	auto right = [](int index)->int {
		if ((index + 1) % 128 == 0) return -1;
		return index + 1;
	};

	while (index < 128 * 128) {
		if (image[index] != 0) {
			//We've found a new region. Grow out from here.
			num_regions++;
			std::vector<int> stack;
			//Push the 4 neighbours onto the stack, and then explore.
			//NOTE: THis is pretty innefficient. Would be better to check if indices are valid before pushing on to the stack,
			//To reduce the number of repetitive checks. Still, this is correct.
			stack.push_back(index);
			while (!stack.empty()) {
				auto neighbour = stack.back();
				stack.pop_back();
				if (neighbour != -1 && image[neighbour] != 0) {
					image[neighbour] = 0;
					stack.push_back(up(neighbour));
					stack.push_back(down(neighbour));
					stack.push_back(left(neighbour));
					stack.push_back(right(neighbour));
				}
			}			
		}
		index++;
	}

	return num_regions;
}

int main(int argc, char** argv)
{
	std::cout << part_1("ffayrhll") << std::endl;
	std::cout << part_2("ffayrhll");
	std::getchar();
}