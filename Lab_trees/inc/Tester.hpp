#ifndef _TESTER_HPP
#define _TESTER_HPP

#include "Common.hpp"

enum Test_type {
	NAIVE,
	NAIVE_SRTD,
	AVL,
	AVL_SRTD,
	TREAP,
};

int Tester(enum Test_type type, FILE *__restrict insert_output, FILE *__restrict erase_output);

#endif
