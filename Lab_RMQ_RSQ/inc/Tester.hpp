#ifndef _TESTER_HPP
#define _TESTER_HPP

#include "Common.hpp"

enum Test_type {
	TEST1,
	TEST2,
};

int Tester(enum Test_type test, FILE *output);

#endif
