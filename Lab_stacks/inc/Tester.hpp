#ifndef _TESTER_HPP
#define _TESTER_HPP

#include "Common.hpp"

enum Stack_type {
	DA,
	FL,
};

enum Test_type {
	TEST1,
	TEST2,
	TEST3,
	TEST4,
};

int Tester(enum Stack_type stk, enum Test_type test, FILE *output);

#endif
