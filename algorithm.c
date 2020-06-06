#include <stdbool.h>
#include "algorithm.h"

void swap(void *a, void *b, size_t elem_size) {
    char t[elem_size];
    memcpy(t, a, elem_size);
    memcpy(a, b, elem_size);
    memcpy(b, t, elem_size);
}

void *linear_search(void *arr, size_t n, size_t elem_size, const void *val, cmp_fun cmp){
    char *walk = arr;

    for (size_t i = 0; i < n; i++) {
        if (cmp(walk, val) == 0)
            return walk;
        else
            walk += elem_size;
    }

    return NULL;
}

void *binary_search(void *arr, size_t n, size_t elem_size, const void *val, cmp_fun cmp){
    size_t left = 0, right = n - 1, mid = (left + right) / 2;

    while (left <= right) {
        int cmp_val = cmp(arr + elem_size * mid, val);

        if (cmp_val == 0)
            return arr + elem_size * mid;
        else if (cmp_val > 0)
            left = mid + 1;
        else
            right = mid - 1;

        mid = (left + right) / 2;
    }

    return NULL;
}

void insert_sort(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order){
    for (size_t i = 1; i < n; i++)
        for (size_t j = i; j > 0; j--)
            if (order * cmp((char *)a + j * elem_size, (char *)a + (j - 1) * elem_size) < 0)
                swap((char *)a + j * elem_size, (char *)a + (j - 1) * elem_size, elem_size);
            else
                break;
}

void shell_sort(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order){
    for (size_t step = n >> 1; step >= 1; step >>= 1)
        for (size_t i = step; i < n; i += step)
            for (size_t j = i; j > 0; j -= step)
                if (order * cmp((char *)a + j * elem_size, (char *)a + (j - 1) * elem_size) < 0)
                    swap((char *)a + j * elem_size, (char *)a + (j - 1) * elem_size, elem_size);
                else
                    break;
}

void bubble_sort(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order){
    bool exchanged = true;
    size_t pos = n;

    for (size_t i = 0; i < n - 1 && exchanged; i++) {
        exchanged = false;
        for (size_t j = 0; j < pos - 1; j++)
            if (order * cmp((char *)a + j * elem_size, (char *)a + (j + 1) * elem_size) > 0) {
                swap((char *)a + j * elem_size, (char *)a + (j + 1) * elem_size, elem_size);
                exchanged = true;
            }
        --pos;
    }
}

void quick_sort(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order) {
    if (n <= 1)
        return;

    struct range {
        size_t start;
        size_t end;
    };
    struct range *rng = malloc(sizeof(struct range) * n);
    struct range cur_rng;
    size_t p = 0;
    rng[p].start = 0;
    rng[p].end = n - 1;
    ++p;
    size_t l, r, pivot;

    while (p) {
        cur_rng = rng[--p];
        l = cur_rng.start;
        r = cur_rng.end - 1;
        pivot = cur_rng.end;

        if (l > r)
            continue;

        while (l < r) {
            while (l < r && order * cmp((char *)a + l * elem_size, (char *)a + pivot * elem_size) <= 0)
                ++l;
            while (l < r && order * cmp((char *)a + r * elem_size, (char *)a + pivot * elem_size) > 0)
                --r;
            swap((char *)a + l * elem_size, (char *)a + r * elem_size, elem_size);
        }
        if (order * cmp((char *)a + l * elem_size, (char *)a + pivot * elem_size) > 0)
            swap((char *)a + l * elem_size, (char *)a + pivot * elem_size, elem_size);
        else
            ++l;

        if (l > 1) {
            rng[p].start = cur_rng.start;
            rng[p].end = l - 1;
            ++p;
        }
        rng[p].start = l + 1;
        rng[p].end = cur_rng.end;
        ++p;
    }
}

void heapify(void *a, size_t elem_size, size_t n, size_t idx, cmp_fun cmp, enum sorting_order order) {
    size_t l, r, m;

    while (1) {
        l = idx * 2 + 1;
        r = l + 1;
        m = idx;

        if (l < n && order * cmp((char *)a + l * elem_size, (char *)a + m * elem_size) > 0)
            m = l;
        if (r < n && order * cmp((char *)a + r * elem_size, (char *)a + m * elem_size) > 0)
            m = r;

        if (m != idx) {
            swap((char *)a + idx * elem_size, (char *)a + m * elem_size, elem_size);
            idx = m;
        }
        else
            break;
    }
}

void build_heap(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order) {
    for (size_t i = (n - 2) / 2; i >= 0; i--) {
        heapify(a, elem_size, n, i, cmp, order);
        if (i == 0)
            break;
    }
}

void heap_sort(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order) {
    build_heap(a, elem_size, n, cmp, order);

    for (size_t i = n - 1; i >= 1; i--) {
        swap(a, (char *)a + i * elem_size, elem_size);
        heapify(a, elem_size, i, 0, cmp, order);
    }
}

void max_heapify(void *a, size_t elem_size, size_t n, size_t idx, cmp_fun cmp) {
    heapify(a, elem_size, n, idx, cmp, sort_ascending);
}

void min_heapify(void *a, size_t elem_size, size_t n, size_t idx, cmp_fun cmp) {
    heapify(a, elem_size, n, idx, cmp, sort_descending);
}

void build_max_heap(void *a, size_t elem_size, size_t n, cmp_fun cmp) {
    build_heap(a, elem_size, n, cmp, sort_ascending);
}

void build_min_heap(void *a, size_t elem_size, size_t n, cmp_fun cmp) {
    build_heap(a, elem_size, n, cmp, sort_descending);
}

void merge_sort(void *a, size_t elem_size, size_t n, cmp_fun cmp, enum sorting_order order) {
    void *src = a;
    void *b = malloc(elem_size * n);
    size_t seg, start;

    for (seg = 1; seg < n; seg <<= 1) {
        for (start = 0; start < n; start += seg + seg) {
            size_t low = start;
            size_t mid = start + seg < n ? start + seg : n;
            size_t high = start + seg + seg < n ? start + seg + seg : n;
            size_t k = low;
            size_t start1 = low, end1 = mid;
            size_t start2 = mid, end2 = high;
            while (start1 < end1 && start2 < end2) {
                if (order * cmp((char *)src + start1 * elem_size, (char *)src + start2 * elem_size) < 0)
                    memcpy((char *)b + k++ * elem_size, (char *)src + start1++ * elem_size, elem_size);
                else
                    memcpy((char *)b + k++ * elem_size, (char *)src + start2++ * elem_size, elem_size);
            }
            while (start1 < end1)
                memcpy((char *)b + k++ * elem_size, (char *)src + start1++ * elem_size, elem_size);
            while (start2 < end2)
                memcpy((char *)b + k++ * elem_size, (char *)src + start2++ * elem_size, elem_size);
        }
        void *temp = src;
        src = b;
        b = temp;
    }
    if (a != src) {
        memcpy(a, src, elem_size * n);
        b = src;
    }
    free(b);
}
