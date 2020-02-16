#include "test.h"
#include "huffman.h"
#include "autotest.h"
#include <fstream>
#include <string>
#include <stdio.h>

bool compare_files(const std::string &file1, const std::string &file2) {
	std::ifstream in1(file1, std::ios::binary);
	std::ifstream in2(file2, std::ios::binary);
	char tmp1, tmp2;
	bool result = 0;
	while (true) {
		in1.read(reinterpret_cast<char*>(&tmp1), sizeof(char));
		in2.read(reinterpret_cast<char*>(&tmp2), sizeof(char));
		if (!in1 || !in2) {
			result = (!in1 && !in2 ? true : false);
			break;
		}
		if (tmp1 == tmp2)
			continue;
		result = (tmp1 < tmp2);
		break;
	}
	in1.close();
	in2.close();
	return result;
}

const std::string FILE_NAME = "tmpfile.txt";
const std::string ENCODED_FILE_NAME = "encoded_file.txt";
const std::string NEW_FILE_NAME = "new_tmpfile.txt";

void HuffmanTest::test_end_to_end_min() {
	std::ofstream out(FILE_NAME, std::ios::binary);

	out << "mem";

	out.close();
	HuffmanArchiver archiver;
	archiver.encode_file(FILE_NAME, ENCODED_FILE_NAME);
	HuffmanExtracter extracter;
	extracter.decode_file(ENCODED_FILE_NAME, NEW_FILE_NAME);
	ASSERT(compare_files(FILE_NAME, NEW_FILE_NAME));
	remove(FILE_NAME.c_str());
	remove(ENCODED_FILE_NAME.c_str());	
	remove(NEW_FILE_NAME.c_str());
}

void HuffmanTest::test_end_to_end_1() {
	std::ofstream out(FILE_NAME, std::ios::binary);

	for (int i = 0; i < 100; ++i) {
		out << char(rand() % 256);
	}

	out.close();
	HuffmanArchiver archiver;
	archiver.encode_file(FILE_NAME, ENCODED_FILE_NAME);
	HuffmanExtracter extracter;
	extracter.decode_file(ENCODED_FILE_NAME, NEW_FILE_NAME);
	ASSERT(compare_files(FILE_NAME, NEW_FILE_NAME));
	remove(FILE_NAME.c_str());
	remove(ENCODED_FILE_NAME.c_str());
	remove(NEW_FILE_NAME.c_str());
}


void HuffmanTest::test_end_to_end_2() {
	std::ofstream out(FILE_NAME, std::ios::binary);

	for (int i = 0; i < 10000; ++i) {
		out << char(rand() % 256);
	}

	out.close();
	HuffmanArchiver archiver;
	archiver.encode_file(FILE_NAME, ENCODED_FILE_NAME);
	HuffmanExtracter extracter;
	extracter.decode_file(ENCODED_FILE_NAME, NEW_FILE_NAME);
	ASSERT(compare_files(FILE_NAME, NEW_FILE_NAME));
	remove(FILE_NAME.c_str());
	remove(ENCODED_FILE_NAME.c_str());	
	remove(NEW_FILE_NAME.c_str());
}

void HuffmanTest::test_end_to_end_big() {
	std::ofstream out(FILE_NAME, std::ios::binary);

	for (int i = 0; i < 1000000; ++i) {
		out << char(rand() % 256);
	}

	out.close();
	HuffmanArchiver archiver;
	archiver.encode_file(FILE_NAME, ENCODED_FILE_NAME);
	HuffmanExtracter extracter;
	extracter.decode_file(ENCODED_FILE_NAME, NEW_FILE_NAME);
	ASSERT(compare_files(FILE_NAME, NEW_FILE_NAME));
	remove(FILE_NAME.c_str());
	remove(ENCODED_FILE_NAME.c_str());	
	remove(NEW_FILE_NAME.c_str());
}

void HuffmanTest::run_all_tests() {
	test_end_to_end_min();
	test_end_to_end_1();
	test_end_to_end_2();
	test_end_to_end_big();
}

int main() {
	HuffmanTest tests;
	tests.run_all_tests();
	tests.show_final_result();
	return 0;
}