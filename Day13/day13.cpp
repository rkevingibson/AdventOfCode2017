#include <cstdint>
#include <sstream>
#include <iostream>
#include <vector>

struct Layer {
	uint32_t depth;
	uint32_t range;
};

uint32_t part_1(const std::vector<Layer>& firewall)
{
	uint32_t severity = 0;
	for (const auto& layer : firewall) {
		if (layer.depth % (2 * layer.range - 2) == 0) {
			severity += layer.depth*layer.range;
		}
	}

	return severity;
}

uint32_t part_2(const std::vector<Layer>& firewall)
{
	uint32_t delay=0; 
	while (true) {
		uint32_t severity = 0;
		for (const auto& layer : firewall) {
			if (((layer.depth + delay) % (2 * layer.range - 2)) == 0) {
				severity++;
			}
		}
		if (severity == 0) {
			break;
		}
		delay++;
	}
	return delay;
}

std::vector<Layer> load_input()
{
	const char* input = R"%(0: 3
		1 : 2
		2 : 4
		4 : 4
		6 : 5
		8 : 6
		10 : 6
		12 : 6
		14 : 6
		16 : 8
		18 : 8
		20 : 8
		22 : 8
		24 : 10
		26 : 8
		28 : 8
		30 : 12
		32 : 14
		34 : 12
		36 : 10
		38 : 12
		40 : 12
		42 : 9
		44 : 12
		46 : 12
		48 : 12
		50 : 12
		52 : 14
		54 : 14
		56 : 14
		58 : 12
		60 : 14
		62 : 14
		64 : 12
		66 : 14
		70 : 14
		72 : 14
		74 : 14
		76 : 14
		80 : 18
		88 : 20
		90 : 14
		98 : 17)%";

	const char * test = R"%(0: 3
		1 : 2
		4 : 4
		6 : 4)%";

	std::stringstream ss(input);
	std::vector<Layer> result;
	while (ss.good()) {
		uint32_t depth, range;
		ss >> depth;
		ss.ignore(std::numeric_limits<std::streamsize>::max(), ':');
		ss >> range;
		result.emplace_back(Layer{depth, range});
	}
	return result;
}


int main(int argc, char** argv)
{
	auto input = load_input();
	std::cout << part_1(input) << std::endl;
	std::cout << part_2(input) << std::endl;
	std::getchar();
}