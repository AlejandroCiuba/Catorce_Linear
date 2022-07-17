/**
 * @file test.c
 * @author Alejandro Ciuba (alejandrociuba@gmail.com)
 * @brief Small package containing vectors, matrices and their linear algebra operations
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Error-check
typedef enum { FAILED, PASSED} test;

// TESTS FOR VECTOR_H
test prefixtures(vec* v);
test test_init_vec(vec* v);
test test_append(vec* v);
test test_replace(vec* v);
test test_contains(vec* v);
test test_get_deep(vec* v);
test test_get_shallow(vec* v);
test test_get_data_size(vec* v);
test test_byte_size(vec* v);
test test_upsize(vec* v);
test test_downsize(vec* v);
test test_free_vec(vec* v);

int main(/*int argc, char* argv[]*/) {
    return 0;
}