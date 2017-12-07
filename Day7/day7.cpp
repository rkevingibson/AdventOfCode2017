#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

struct Node {
	std::string name;
	int weight;
	std::vector<Node*> children;
	
};

std::vector<Node> load_file(const char* filename)
{
	std::ifstream fs(filename, std::ios::in);
	std::vector<Node> result;
	std::vector<std::string> children_strings;

	std::unordered_map<std::string, size_t> name_to_position;

	for (std::string line; std::getline(fs, line); ) {
		auto label_end = line.find(' ');
		auto weight_start = line.find('(', label_end);
		auto weight_end = line.find(')', weight_start);

		Node node;
		node.name = line.substr(0, label_end);
		node.weight = std::stoi(line.substr(weight_start+1, weight_end - weight_start));
		
		name_to_position[node.name] = result.size();

		result.emplace_back(std::move(node));
		if (weight_end == line.size() - 1) {
			children_strings.push_back("");
		}
		else {
			children_strings.push_back(line.substr(line.find('>', weight_end) + 2));
		}
	}
	
	for (size_t i = 0; i < result.size(); i++) {
		if (children_strings[i].size() != 0) {
			size_t comma, prev_comma = 0;


			do {
				comma = children_strings[i].find(',', prev_comma);
				auto child_name =children_strings[i].substr(prev_comma, comma-prev_comma);
				auto child_index = name_to_position.at(child_name);
				result[i].children.push_back(&result[child_index]);
				prev_comma = comma + 2;
			} while(comma != std::string::npos);
		}
	}

	return result;
}

const Node* part_1(const std::vector<Node>& programs)
{
	//Need to find a node which -has no children and is not a child of any other node.
	std::vector<bool> is_child;
	is_child.resize(programs.size(), false);
	for (const auto& program : programs) {
		for (auto child : program.children) {
			ptrdiff_t index = child - programs.data();
			is_child[index] = true;
		}
	}

	for (int i=0; i < programs.size(); ++i) {
		if (!is_child[i] && !programs[i].children.empty()) {
			return &(programs[i]);
		}
	}
}

struct Result {

};

int get_graph_weight(const Node* graph)
{
	std::vector<int> child_weights;
	for (auto& child : graph->children) {
		child_weights.push_back(get_graph_weight(child));
	}

	if(child_weights.empty()) return graph->weight;

	//Check if all the child weights are the same. 
	//If they aren't, the odd-child weight out is our bad node.
	//What's the easiest way to check this? If we have a bunch of children, all will be equal but one.
	//Easiest - search until != previous and != to next. 
	//Will have special cases for first and last, otherwise good.
	//Then, once I know the odd one out, I need the graph weight of it and its neighbours
	//This is a horrific abuse of exceptions, but eh.
	{
		for (auto it = child_weights.begin() + 1; it != child_weights.end() - 1; ++it) {
			if (*it != *(it - 1) && *it != *(it + 1)) {
				//*it is the odd one out.
				//Should I throw an exception? Seems like an easy way to check.
				auto bad_node = graph->children[std::distance(child_weights.begin(), it)];
				auto bad_weight = *it;
				auto proper_weight = *(it-1);
				throw bad_node->weight + (proper_weight - bad_weight);
			}
		}

		if (child_weights.size() > 1 && child_weights[0] != child_weights[1]) {
			auto bad_node = graph->children[0];
			auto bad_weight = child_weights[0];
			auto proper_weight = child_weights[1];
			throw bad_node->weight + (proper_weight - bad_weight);
		}
		else if (child_weights.size() > 1 && child_weights.back() != child_weights[0]) {
			auto bad_node =graph->children.back();
			auto bad_weight = child_weights.back();
			auto proper_weight = child_weights[0];
			throw bad_node->weight + (proper_weight - bad_weight);
		}
	}
	return std::accumulate(child_weights.begin(), child_weights.end(), 0) + graph->weight;
	
}

int part_2(const Node* graph)
{
	//Do a depth-first search. 
	int weight = 0;
	try {
		weight = get_graph_weight(graph);
	}
	catch (int n) {
		weight = n;
	}

	return weight;
}


int main(int argc, char** argv)
{
	auto graph = load_file("day7.txt");
	auto root = part_1(graph);
	std::cout << "Graph root: " << root->name << std::endl;
	std::cout << "Bad weight: " << part_2(root) << std::endl;

	std::getchar();

}