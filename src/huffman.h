#pragma once
#include <stdint.h>
#include <vector>
#include <fstream>
#include <array>

class HuffTree {
private:
	class Node {
	public:
		int left, right, pr;
		Node(int l, int r);
		Node();
	};

	std::array<Node, 511> tree;

public:
	HuffTree();
	std::vector<bool> path_in_tree(uint8_t pos);
	void build(std::array<int, 256> &char_num);
	void read(std::ifstream &in);
	void write(std::ofstream &out);
	int move_to(int node, bool bit);
};

class Encoder {
public:
	uint8_t num_of_bytes;
	uint8_t number;
	void add(std::vector<bool> &a, std::ofstream &out);
	void write(std::ofstream &out);
	Encoder();
};

class HuffmanArchiver {
private:
	HuffTree tree;

	std::array<int, 256> char_num;
	void count(const std::string &input_file);

	std::array<std::vector<bool>, 256> precalc;
	void count_precalc();

	int get_result_size();

public:
	HuffmanArchiver();
	void encode_file(const std::string &input_file, const std::string &output_file);
};

class Decoder {
public:
	int node;
	uint32_t result_size;
	uint32_t current_bit;
	void decode_byte(HuffTree &tree, uint8_t tmp, std::ofstream &out, uint32_t size);
	Decoder();
};

class HuffmanExtracter {
private:
	HuffTree tree;

public:
	HuffmanExtracter();
	void decode_file(const std::string &input_file, const std::string &output_file);
};