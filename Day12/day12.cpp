#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Node
{
	std::vector<int> neighbours;
	bool visited;
};

int part_1(std::vector<Node> graph)
{
	std::vector<int> neighbours;
	neighbours.push_back(0);
	int num_connected = 0;
	while (!neighbours.empty()) {
		auto node = neighbours.back();
		neighbours.pop_back();
		
		if (!graph[node].visited) {
			num_connected++;
			graph[node].visited = true;
			for (auto n : graph[node].neighbours) {
				neighbours.push_back(n);
			}
		}
	}
	return num_connected;
}

int part_2(std::vector<Node> graph)
{
	//In part 1, we found 1 group. Now, just loop that procedure until we have no more nodes that haven't been visited.
	int num_groups = 0;
	for (auto& node : graph) {
		if (node.visited) continue;
		node.visited = true;
		num_groups++;
		std::vector<int> neighbours(node.neighbours);
		while (!neighbours.empty()) {
			auto n = neighbours.back();
			neighbours.pop_back();
			if (!graph[n].visited) {
				graph[n].visited = true;
				neighbours.insert(neighbours.end(), graph[n].neighbours.begin(), graph[n].neighbours.end());
			}
		}
	}
	return num_groups;

}

std::vector<Node> load_graph(const char* filename)
{
	std::ifstream fs(filename);
	std::vector<Node> graph;
	for (std::string line; std::getline(fs, line);) {
		std::istringstream ss(line);
		ss.ignore(std::numeric_limits<std::streamsize>::max(), '>');
		Node n;
		n.visited = false;
		while (ss.good()) {
			int neighbour;
			ss >> neighbour;
			ss.ignore(2);
			n.neighbours.push_back(neighbour);
			
		}

		graph.emplace_back(std::move(n));
	}
	return graph;
}



int main(int argc, char** argv)
{
	auto graph = load_graph("day12.txt");
	std::cout << part_1(graph) << std::endl;
	std::cout << part_2(graph) << std::endl;
	std::getchar();
}