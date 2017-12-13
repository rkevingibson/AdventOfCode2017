#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>

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

int part_1(const std::vector<unsigned char>& lengths, int list_size)
{
	std::vector<uint8_t> list(list_size);
	std::iota(list.begin(), list.end(), 0);
	run_shuffle(lengths, list);

	return list[0] * list[1];
}

std::string part_2(const std::string& string, int list_size)
{
	std::vector<uint8_t> list(list_size);
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
	std::vector<uint8_t> hash(list_size / 16);
	while (i < list_size) {
		for (int j = 0; j < 16; j++, i++) {
			hash[i / 16] ^= list[i];
		}
	}

	std::ostringstream ss;
	for (auto h : hash) {
		ss << std::hex << (int)h;
	}
	return ss.str();
}


int main(int argc, char** argv)
{
	std::vector<unsigned char> lengths = { 187,254,0,81,169,219,1,190,19,102,255,56,46,32,2,216 };
	//std::string input = "1,2,3";
	std::string input = "187,254,0,81,169,219,1,190,19,102,255,56,46,32,2,216";
	std::cout << part_1(lengths, 256) << std::endl;
	std::cout << part_2(input, 256);
	std::getchar();
}