/**
 * @file test.c
 * @author Alejandro Ciuba (alejandrociuba@gmail.com)
 * @brief Test cases for the vector struct and its associated functions
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright Copyright (c) 2022
 *
 */

 // TO TEST
#include "vector.h"

// REQUIRED STANDARDS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>

// Error-check
typedef enum { FAILED, PASSED } test;

// HELPER FUNCTIONS & GLOBAL
size_t data_size = 0x00;
type data_type = NO_TYPE;
int init_size = -1;
time_t seed = 0x00;

bool comp_data(const void* arg1, const void* arg2, type data_type);
bool parse_args(int argc, char* argv[]);

// PREFIXTURES
vec* prefixtures(type data_type, int init_size, bool fixed_length);
void run_test_case(test(*test_case[])(vec*), int size);

// TEAR DOWN
void teardown(vec* v);

// TESTS FOR VECTOR_H
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

int main(int argc, char* argv[]) {

    // GET TESTING ARGUMENTS
    if ((argc != 5 && argc != 7) || !parse_args(argc, argv)) {

        printf("\nINVALID FORMAT:\n\t./test -d [DATA_TYPE|0-6] -n [INIT_CAPACITY|0-MAX_INT] opt. -s [SEED]\n");
        exit(-1);
    }

    // TEST CASE I: VECTOR_H
    printf("TEST CASE I: VECTOR_H\n");

    int tc1_size = 11;
    test(*test_case_1[])(vec*) = { test_init_vec, test_append, test_replace,
                                    test_contains, test_get_deep, test_get_shallow,
                                    test_get_data_size, test_byte_size, test_upsize,
                                    test_downsize, test_free_vec };

    run_test_case(test_case_1, tc1_size);

    printf("ALL TEST CASES PASSED, EXIT 0!\n");
    return 0;
}

// TEST CASE I: VECTOR_H
test test_init_vec(vec* v) {

    if (v == NULL) return FAILED;

    vec* test_v = init_vec(v->size, v->data_type, v->fixed_length);
    if (test_v == NULL) goto FAILED_TEST_INIT_VEC;

    if (test_v->size != v->size) goto FAILED_TEST_INIT_VEC;
    if (test_v->array == v->array) goto FAILED_TEST_INIT_VEC;
    if (test_v->data_type != v->data_type) goto FAILED_TEST_INIT_VEC;
    if (test_v->fixed_length != v->fixed_length) goto FAILED_TEST_INIT_VEC;

    // NO_TYPE passes
    if (data_type == NO_TYPE) {

        teardown(test_v);
        return PASSED;
    }

    for (int i = 0; i < v->size; i++)
        if (!comp_data(test_v->array + i * data_size, v->array + i * data_size, data_type)) goto FAILED_TEST_INIT_VEC;

    teardown(test_v);
    return PASSED;

FAILED_TEST_INIT_VEC:
    teardown(test_v);
    return FAILED;
}

test test_append(vec* v) {

    if (v == NULL) return FAILED;

    // NO_TYPE passes
    if (data_type == NO_TYPE) return PASSED;

    vec* test_v = init_vec(v->size, data_type, v->fixed_length);
    if (test_v == NULL) return FAILED;

    // Init random seed
    srand(time(NULL));

    // Fill v with random values
    for (int i = 0; i < v->size; i++) {

        int64_t val64 = (int64_t)(rand() % init_size);
        memcpy(v->array + i * data_size, &val64, data_size);
    }

    // Test without upsize
    for (int i = 0; i < v->size; i++)
        if ((test_v = append(v->array + i * data_size, test_v, false)) == NULL) goto FAILED_TEST_APPEND;

    for (int i = 0; i < init_size; i++) {
        if (!comp_data(test_v->array + (i + init_size) * data_size, v->array + i * data_size, data_type)) goto FAILED_TEST_APPEND;
        char zero_test = 0x00 >> data_size;
        if (strncmp(test_v->array + i * data_size, &zero_test, data_size)) goto FAILED_TEST_APPEND;
    }

    teardown(test_v);
    return PASSED;

FAILED_TEST_APPEND:
    teardown(test_v);
    return FAILED;
}

test test_replace(vec* v) {

    if (v == NULL) return FAILED;

    // NO_TYPE passes
    if (data_type == NO_TYPE) return PASSED;

    vec* test_v = init_vec(v->size, data_type, v->fixed_length);
    if (test_v == NULL) return FAILED;

    // Init random seed
    srand(time(NULL));
    int64_t comp_arr[v->size];

    // Fill test_v with random values
    for (int i = 0; i < v->size; i++) {

        int64_t val64 = (int64_t)(rand() % init_size);
        if (!replace(&val64, i, test_v)) goto FAILED_TEST_REPLACE;

        // Add to comp_arr
        comp_arr[i] = val64;
    }

    // Now check
    for (int i = 0; i < v->size; i++) {
        if (strncmp(test_v->array + i * data_size, (char*)&comp_arr[i], data_size)) goto FAILED_TEST_REPLACE;
    }

    return PASSED;

FAILED_TEST_REPLACE:
    teardown(test_v);
    return FAILED;
}

test test_contains(vec* v) {

    if (v == NULL) return FAILED;

    // NO_TYPE passes
    if (data_type == NO_TYPE) return PASSED;

    vec* test_v = init_vec(v->size, data_type, v->fixed_length);
    if (test_v == NULL) return FAILED;

    // Init random seed
    srand(time(NULL));
    int64_t comp_arr[v->size];

    // Fill test_v with random values
    for (int i = 0; i < v->size; i++) {

        int64_t val64 = (int64_t)(rand() % init_size);
        if (!replace(&val64, i, test_v)) goto FAILED_TEST_CONTAINS;

        // Add to comp_arr
        comp_arr[i] = val64;
    }

    // Now check
    for (int i = 0; i < v->size; i++) {
        if (!contains((void*)(comp_arr + i), test_v, NULL)) goto FAILED_TEST_CONTAINS;
    }

    return PASSED;


FAILED_TEST_CONTAINS:
    teardown(test_v);
    return FAILED;
}

test test_get_deep(vec* v) {
    return PASSED;
}

test test_get_shallow(vec* v) {
    return PASSED;
}

test test_get_data_size(vec* v) {
    return PASSED;
}

test test_byte_size(vec* v) {
    return PASSED;
}

test test_upsize(vec* v) {
    return PASSED;
}

test test_downsize(vec* v) {
    return PASSED;
}

test test_free_vec(vec* v) {
    return PASSED;
}

// PREFIXTURES
vec* prefixtures(type data_type, int init_size, bool fixed_length) {

    // Manually create a vector
    if (init_size == 0) return NULL;

    // Init vec struct
    vec* v = (vec*)malloc(sizeof(vec));
    if (v == NULL) return NULL;

    // Init the size, data_type, and fixed_length metadata
    v->size = init_size;
    v->data_type = data_type;
    v->fixed_length = fixed_length;

    // Init the vector array and GLOBAL data_size
    switch (data_type) {

        case NO_TYPE:;

            void* arrv = malloc(sizeof(void) * init_size);
            if (arrv == NULL) {

                free(v);
                return NULL;
            }

            v->array = arrv;
            return v;

        case CHAR:;

            char* arrc = (char*)malloc(sizeof(char) * init_size);
            if (arrc == NULL) {

                free(v);
                return NULL;
            }

            v->array = arrc;

            for (int i = 0; i < init_size; i++)
                ((char*)v->array)[i] = 0;

            data_size = sizeof(char);

            return v;

        case INT32:;

            int32_t* arr32 = (int32_t*)malloc(sizeof(int32_t) * init_size);
            if (arr32 == NULL) {

                free(v);
                return NULL;
            }

            v->array = arr32;

            for (int i = 0; i < init_size; i++)
                ((int32_t*)v->array)[i] = 0;

            data_size = sizeof(int32_t);

            return v;

        case INT64:;

            int64_t* arr64 = (int64_t*)malloc(sizeof(int64_t) * init_size);
            if (arr64 == NULL) {

                free(v);
                return NULL;
            }

            v->array = arr64;

            for (int i = 0; i < init_size; i++)
                ((int64_t*)v->array)[i] = 0;

            data_size = sizeof(int64_t);

            return v;

        case FLOAT32:;

            float* arrf = (float*)malloc(sizeof(float) * init_size);
            if (arrf == NULL) {

                free(v);
                return NULL;
            }

            v->array = arrf;

            for (int i = 0; i < init_size; i++)
                ((float*)v->array)[i] = 0.0f;

            data_size = sizeof(float);

            return v;

        case DOUBLE:;

            double* arrd = (double*)malloc(sizeof(double) * init_size);
            if (arrd == NULL) {

                free(v);
                return NULL;
            }

            v->array = arrd;

            for (int i = 0; i < init_size; i++)
                ((double*)v->array)[i] = 0.0f;

            data_size = sizeof(double);

            return v;

        default:

            free(v);
            return NULL;
    }
}

void run_test_case(test(*test_case[])(vec*), int size) {

    if (test_case == NULL) {
        printf("NO TEST CASE GIVEN, EXITING!\n");
        return;
    }

    vec* main_v = NULL;

    int i = 0;
    for (i = 0; i < size; i++) {

        // PREFIXTURE
        vec* main_v = prefixtures(data_type, init_size, false);

        if ((*test_case[i])(main_v) == FAILED) goto FAIL;

        // TEARDOWN
        teardown(main_v);
    }

    return;

    // FAILED TEST
FAIL:
    teardown(main_v);
    printf("\nTEST %d FAILED, EXIT -1!\n", i);
    exit(-1);
}

// TEAR DOWN
void teardown(vec* v) {

    if (v == NULL) return;
    else if (v->array == NULL) {

        free(v);
        return;
    }

    // Free the void pointer array
    free(v->array);

    // Finally, free the v pointer
    free(v);
}

// HELPER FUNCTIONS
bool comp_data(const void* arg1, const void* arg2, type data_type) {
    return (strncmp((char*)arg1, (char*)arg2, data_size) == 0);
}

bool parse_args(int argc, char* argv[]) {
    // GET ARGUMENTS
    int opt = -1;
    seed = time(NULL); // If no seed is established
    while ((opt = getopt(argc, argv, "d:n:s:")) != -1) {

        switch (opt) {

            case 'd':;

                int check = atoi(optarg);
                if (check > 5 || check < 0) return false;

                data_type = (type)check;

                break;

            case 'n':

                init_size = atoi(optarg);

                if (init_size < 0) return false;

                break;

            case 's':

                seed = (time_t)atoi(optarg);
                break;

            default:
                return false;
        }
    }

    // All args were correctly parsed
    return true;
}
