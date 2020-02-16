#pragma once

#include "huffman.h"
#include "autotest.h"

#define ASSERT(expr) check(expr);

class HuffmanTest: public Autotest {
public:
	void test_end_to_end_min();
	void test_end_to_end_1();
	void test_end_to_end_2();
	void test_end_to_end_big();

	void run_all_tests();
};