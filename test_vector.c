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
void run_test_case(test (*test_case[])(vec*), int size, vec* main_v);

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
    if((argc != 5 && argc != 7) || !parse_args(argc, argv)) {

        printf("\nINVALID FORMAT:\n\t./test -d [DATA_TYPE|0-6] -n [INIT_CAPACITY|0-MAX_INT] opt. -s [SEED]\n");
        exit(-1);
    }

    // PREFIXTURE
    vec* v = prefixtures(data_type, init_size, false);

    // TEST CASE I: VECTOR_H
    printf("TEST CASE I: VECTOR_H\n");

    int tc1_size = 11;
    test (*test_case_1[])(vec*) = { test_init_vec, test_append, test_replace,
                                        test_contains, test_get_deep, test_get_shallow,
                                        test_get_data_size, test_byte_size, test_upsize,
                                        test_downsize, test_free_vec };

    run_test_case(test_case_1, tc1_size, v);

    // TEARDOWN
    teardown(v);

    printf("ALL TEST CASES PASSED, EXIT 0!\n");
    return 0;
}

// TEST CASE I: VECTOR_H
test test_init_vec(vec* v) {

    if(v == NULL) return FAILED;

    vec* test_v = init_vec(v->size, v->data_type, v->fixed_length);
    if(test_v == NULL) goto FAILED_TEST_INIT_VEC;

    if(test_v->size != v->size) goto FAILED_TEST_INIT_VEC;
    if(test_v->array == v->array) goto FAILED_TEST_INIT_VEC;
    if(test_v->data_type != v->data_type) goto FAILED_TEST_INIT_VEC;
    if(test_v->fixed_length != v->fixed_length) goto FAILED_TEST_INIT_VEC;

    for(int i = 0; i < v->size; i++) 
        if(!comp_data(test_v->array + i * data_size, v->array + i * data_size, data_type)) goto FAILED_TEST_INIT_VEC;

    teardown(test_v);
    return PASSED;

    FAILED_TEST_INIT_VEC:
        teardown(test_v);
        return FAILED;
}

test test_append(vec* v) {
    
    if(v == NULL) return FAILED;

    vec* test_v = init_vec(v->size, data_type, v->fixed_length);

    // Init random seed
    srand(time(NULL));
    
    teardown(test_v);
    return PASSED;
}

test test_replace(vec* v) {
    return PASSED;
}

test test_contains(vec* v) {
    return PASSED;
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
    vec* v = (vec*) malloc(sizeof(vec));
    if (v == NULL) return NULL;

    // Init the size, data_type, and fixed_length metadata
    v->size = init_size;
    v->data_type = data_type;
    v->fixed_length = fixed_length;

    // Init the vector array and GLOBAL data_size
    switch (data_type) {

        case NO_TYPE:

            return v;

        case CHAR:;

            char* arrc = (char*) malloc(sizeof(char) * init_size);
            if (arrc == NULL) { 

                free(v); 
                return NULL; 
            }

            v->array = arrc;

            for (int i = 0; i < init_size; i++) 
                ((char*) v->array)[i] = 0;

            data_size = sizeof(char);

            return v;

        case INT32:;

            int32_t* arr32 = (int32_t*) malloc(sizeof(int32_t) * init_size);
            if (arr32 == NULL) { 

                free(v); 
                return NULL; 
            }

            v->array = arr32;

            for (int i = 0; i < init_size; i++) 
                ((int32_t*) v->array)[i] = 0;

            data_size = sizeof(int32_t);

            return v;

        case INT64:;

            int64_t* arr64 = (int64_t*) malloc(sizeof(int64_t) * init_size);
            if (arr64 == NULL) { 

                free(v); 
                return NULL; 
            }

            v->array = arr64;

            for (int i = 0; i < init_size; i++) 
                ((int64_t*) v->array)[i] = 0;

            data_size = sizeof(int64_t);

            return v;

        case FLOAT32:;

            float* arrf = (float*) malloc(sizeof(float) * init_size);
            if (arrf == NULL) { 

                free(v); 
                return NULL; 
            }

            v->array = arrf;

            for (int i = 0; i < init_size; i++) 
                ((float*) v->array)[i] = 0.0f;

            data_size = sizeof(float);

            return v;

        case DOUBLE:;

            double* arrd = (double*) malloc(sizeof(double) * init_size);
            if (arrd == NULL) { 

                free(v); 
                return NULL; 
            }

            v->array = arrd;

            for (int i = 0; i < init_size; i++) 
                ((double*) v->array)[i] = 0.0f;

            data_size = sizeof(double);

            return v;

        default:

            free(v);
            return NULL;
    }
}

void run_test_case(test (*test_case[])(vec*), int size, vec* main_v) {

    if(test_case == NULL) {
        printf("NO TEST CASE GIVEN, EXITING!\n");
        return; 
    }

    int i = 0;
    for (i = 0; i < size; i++)
        if ((*test_case[i])(main_v) == FAILED) goto FAIL;

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

    switch(data_type) {
        case NO_TYPE:
            return false;
        
        case CHAR:
            return *(char*)(arg1) == *(char*)(arg2);

        case INT32:
            return *(int32_t*)(arg1) == *(int32_t*)(arg2);
        
        case INT64:
            return *(int64_t*)(arg1) == *(int64_t*)(arg2);
        
        case FLOAT32:
            return *(float*)(arg1) == *(float*)(arg2);
        
        case DOUBLE:
            return *(double*)(arg1) == *(double*)(arg2);
        
        default:
            return false;
    } 
}

bool parse_args(int argc, char* argv[]) {
    // GET ARGUMENTS
    int opt = -1;
    seed = time(NULL); // If no seed is established
    while((opt = getopt(argc, argv, "d:n:s:")) != -1) {

        switch(opt) { 

            case 'd':;

                int check = atoi(optarg);
                if(check > 5 || check < 0) return false;

                data_type = (type) check;

                break;

            case 'n': 

                init_size = atoi(optarg);

                if(init_size < 0) return false;

                break;

            case 's':

                seed = (time_t) atoi(optarg);
                break;

            default:
                return false;
        } 
    }

    // All args were correctly parsed
    return true;
}