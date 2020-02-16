#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iostream>
#include <stdint.h>
#include "huffman.h"
#include <numeric>

const int ROOT = 510;
const int ADDITIONAL_BITS = 542;

HuffTree::Node::Node(int l, int r) : left(l), right(r), pr(-1) {}
HuffTree::Node::Node() : Node(-1, -1) {}

HuffTree::HuffTree() {
	tree.fill(Node());
}

std::vector<bool> HuffTree::path_in_tree(uint8_t pos) {
	int cur = pos;
	std::vector<bool> tmp;
	while (cur != ROOT) {
		int pr = tree[cur].pr;
		if (tree[pr].left == cur) tmp.push_back(0);
		else tmp.push_back(1);
		cur = pr;
	}
	std::reverse(tmp.begin(), tmp.end());
	return tmp;
}

void HuffTree::build(std::array<int, 256> &char_num) {
	std::priority_queue<std::pair<int, int>> trees;
	for (size_t i = 0; i < int(char_num.size()); ++i) {
		trees.push(std::make_pair(-char_num[i], i));
	}
	int num = 256;
	while (trees.size() > 1) {
		int kl = trees.top().first;
		int l = trees.top().second;
		trees.pop();

		int kr = trees.top().first;
		int r = trees.top().second;
		trees.pop();
		if (r < l) std::swap(l, r);

		tree[num] = Node(l, r);
		tree[l].pr = num;
		tree[r].pr = num;
		trees.push(std::make_pair(kl + kr, num++));
	}
}

void HuffTree::read(std::ifstream &in) {
	for (int i = 0; i < ROOT; ++i) {
		uint8_t tmp_ = 0;
		in.read(reinterpret_cast<char*>(&tmp_), sizeof(uint8_t));
		if (in.rdstate() != std::ifstream::goodbit)
			throw std::invalid_argument("Error: Invalid format of file!");
		int tmp = int(tmp_) + 256;
		tree[i].pr = tmp;
		if (tree[tmp].left == -1) tree[tmp].left = i;
		else {
			if (tree[tmp].right != -1)
				throw std::invalid_argument("Error: Invalid format of file!");
			tree[tmp].right = i;
		}
	}
}

void HuffTree::write(std::ofstream &out) {
	for (int i = 0; i < ROOT; ++i) {
		int8_t tmp = tree[i].pr - 256;
		out.write(reinterpret_cast<char*>(&tmp), sizeof(uint8_t));
	}
}

int HuffTree::move_to(int node, bool bit) {
	if (bit) return tree[node].right;
	else return tree[node].left;
}

Encoder::Encoder() : num_of_bytes(0), number(0) {}

void Encoder::write(std::ofstream &out) {
	if (num_of_bytes) {
		out.write(reinterpret_cast<char*>(&number), sizeof(uint8_t));
		num_of_bytes = 0;
		number = 0;
	}
}

void Encoder::add(std::vector<bool> &a, std::ofstream &out) {
	for (auto byte : a) {
		number |= (uint8_t(byte) << (7 - num_of_bytes));
		num_of_bytes++;
		if (num_of_bytes == 8) write(out);
	}
}

void HuffmanArchiver::count(const std::string &input_file) {
	std::ifstream in(input_file, std::ios::binary);
	uint8_t tmp = 0;
	while (in.read(reinterpret_cast<char*>(&tmp), sizeof(uint8_t))) {
		char_num[tmp]++;
	}
	in.close();
}

void HuffmanArchiver::count_precalc() {
	for (size_t i = 0; i < precalc.size(); ++i) {
		precalc[i] = tree.path_in_tree(i);
	}
}

int32_t HuffmanArchiver::get_result_size() {
	uint32_t size = 0;
	for (size_t i = 0; i < 256; ++i) {
		size += char_num[i] * precalc[i].size();
	}
	return size;
}

HuffmanArchiver::HuffmanArchiver(): tree() {
	char_num.fill(0);
	precalc.fill(std::vector<bool>());
}

void HuffmanArchiver::encode_file(const std::string &input_file, const std::string &output_file) {
	count(input_file);
	tree.build(char_num);
	count_precalc();
	std::ifstream in(input_file, std::ios::binary);
	std::ofstream out(output_file, std::ios::binary);

	uint32_t size = get_result_size();
	out.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
	
	tree.write(out);
	uint8_t tmp = 0;
	Encoder writer;
	while (in.read(reinterpret_cast<char*>(&tmp), sizeof(uint8_t))) {
		writer.add(precalc[tmp], out);
	}
	writer.write(out);

	in.close();
	out.close();

	std::cout << std::accumulate(char_num.begin(), char_num.end(), 0) << '\n';
	std::cout << (size + 7) / 8 << '\n';
	std::cout << ADDITIONAL_BITS << '\n';
}

Decoder::Decoder() : node(ROOT), result_size(0), current_bit(0) {}

void Decoder::decode_byte(HuffTree &tree, uint8_t tmp, std::ofstream &out, uint32_t size) {
	for (size_t i = 0; i < 8 && current_bit < size; ++i, ++current_bit) {
		node = tree.move_to(node, bool(tmp & (1 << (7 - i))));
		if (node < 256) {
			int8_t node_ = node;
			out.write(reinterpret_cast<char*>(&node_), sizeof(int8_t));
			result_size++;
			node = ROOT;
		}
	}
	if (current_bit == size && node != ROOT) {
		throw std::invalid_argument("Error: Wrong format of file");
	}
}

HuffmanExtracter::HuffmanExtracter() : tree() {}

void HuffmanExtracter::decode_file(const std::string &input_file, const std::string &output_file) {
	std::ifstream in(input_file, std::ios::binary);
	std::ofstream out(output_file, std::ios::binary);

	uint32_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
	if (in.rdstate() != std::ifstream::goodbit) {
		in.close();
		out.close();
		throw std::invalid_argument("Error: Wrong format of file");
	}
	try {
		tree.read(in);
	}
	catch (...) {
		in.close();
		out.close();
		throw;
	}

	uint8_t tmp = 0;
	Decoder decoder;
	try {
		while (in.read(reinterpret_cast<char*>(&tmp), sizeof(uint8_t))) {
			decoder.decode_byte(tree, tmp, out, size);
		}
	}
	catch (...) {
		in.close();
		out.close();
		throw;
	}
	in.close();
	out.close();
	if (decoder.current_bit != size) {
		throw std::invalid_argument("Error: Wrong format of file");
	}

	std::cout << (size + 7) / 8 << '\n';
	std::cout << decoder.result_size << '\n';
	std::cout << ADDITIONAL_BITS << '\n';
}