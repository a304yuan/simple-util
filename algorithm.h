#ifndef SIMPLE_ALGORITHM_H
#define SIMPLE_ALGORITHM_H

#include <stdlib.h>

/**
 * Define the function to compare two elements, if the former is greater than
 * the latter it returns 1, otherwise returns -1. If they are equal, then returns
 * 0.
 */
typedef int (*cmp_fun)(const void *, const void *);

/**
 * Generic searching and sorting algorithms
 */
enum sorting_order {
	sort_ascending = 1,
	sort_descending = -1
};

extern void swap(void * a, void * b, size_t elem_size);

extern void * linear_search(void * arr, size_t n, size_t elem_size, const void * val, cmp_fun cmp);

extern void * binary_search(void * arr, size_t n, size_t elem_size, const void * val, cmp_fun cmp);

/* Insert sort */
extern void insert_sort(void * arr, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order);

/* Shell's sort */
extern void shell_sort(void * arr, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order);

/* Bubble sort */
extern void bubble_sort(void * arr, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order);

/* Quick sort */
extern void quick_sort(void * arr, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order);

/* Heap sort */
extern void max_heapify(void * arr, size_t elem_size, size_t n, size_t idx, cmp_fun cmp);
extern void build_max_heap(void * arr, size_t elem_size, size_t n, cmp_fun cmp);
extern void min_heapify(void * arr, size_t elem_size, size_t n, size_t idx, cmp_fun cmp);
extern void build_min_heap(void * arr, size_t elem_size, size_t n, cmp_fun cmp);
extern void heap_sort(void * arr, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order);

/* Merge sort */
extern void merge_sort(void * arr, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order);

#endif
