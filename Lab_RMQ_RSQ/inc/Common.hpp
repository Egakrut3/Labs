#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <assert.h>
#include <errno.h>
#include "My_std.hpp"
#include <stdio.h>
#include <stdlib.h>



typedef char unsigned byte_t;



#if defined(__STDC_VERSION__)

#define TYPEOF(expr)		typeof(expr)
#define TYPEOF_UNQUAL(expr)	typeof_unqual(expr)

#else

#include <type_traits>

#define TYPEOF(expr)		std::remove_reference_t<decltype((expr))>
#define TYPEOF_UNQUAL(expr)	std::remove_cv_t<TYPEOF(expr)>

#endif



#if defined(NDEBUG)

#define ON_DEBUG(...)

#else

#define ON_DEBUG(...) __VA_ARGS__

#endif

#define PRINT_LINE()							\
do {									\
	fprintf(stderr, __FILE__ ":%d: %s\n", __LINE__, __func__);	\
} while (false)



#define LEAVE(err)	\
do {			\
	FINAL_CODE	\
	return err;	\
} while (false)

#define CHECK_PROC(proc, ...)							\
do {										\
	int _cur_err_val = (proc)(__VA_ARGS__);					\
	if (!_cur_err_val) { break; }						\
	ON_DEBUG(								\
		fprintf(stderr, "Error with code %d found\n", _cur_err_val);	\
		PRINT_LINE();							\
		fputs(#proc " failed\n", stderr);				\
	)									\
	LEAVE(_cur_err_val);							\
} while (false)



#define ALLOC_ANY_ELEM(dst, size)		CHECK_PROC(my_malloc, &(dst), size)
#define CALLOC_ANY_ARR(dst, n, size)		CHECK_PROC(my_calloc, &(dst), n, size)
#define REALLOC_ANY_ARR(dst, new_n, size)	CHECK_PROC(my_reallocarray, &(dst), (dst), new_n, size)
#define FREE_ANY_ELEM(elem, size)		free_sized((elem), size)
#define FREE_ANY_ARR(arr, n, size)		free_sized((arr), size * n)

#define ALLOC_ELEM(dst)				CHECK_PROC(my_malloc, (void *__restrict *__restrict)&(dst), sizeof(*(dst)))
#define CALLOC_ARR(dst, n)			CHECK_PROC(my_calloc, (void *__restrict *__restrict)&(dst), n, sizeof(*(dst)))
#define REALLOC_ARR(dst, new_n)			CHECK_PROC(my_reallocarray, (void *__restrict *__restrict)&(dst), (dst), new_n, sizeof(*(dst)))
#define FREE_ELEM(elem)				FREE_ANY_ELEM((elem), sizeof(*(elem)))
#define FREE_ARR(arr, n)			FREE_ANY_ARR((arr), n, sizeof(*(arr)))



#define NEW(type, dst, ...)						\
do {									\
	ALLOC_ELEM(dst);						\
	CHECK_PROC(type ## _ctor, dst __VA_OPT__(,) __VA_ARGS__);	\
} while (false)
#define DELETE_UNCHECKED(type, dst)	\
do {					\
	type ## _dtor(dst);		\
	FREE_ELEM(dst);			\
} while (false)
#define DELETE_CHECKED(type, dst)	\
do {					\
	CHECK_PROC(type ## _dtor, dst);	\
	FREE_ELEM(dst);			\
} while (false)

#endif
