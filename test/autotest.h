#pragma once

#include <iostream>

class Autotest {
protected:
	static int failedNum;
	static int totalNum;
public:
	static void check(bool expr);
	static void show_final_result();
	virtual void run_all_tests() = 0;
};