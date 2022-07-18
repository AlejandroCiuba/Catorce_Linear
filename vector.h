/**
 * @file vector.h
 * @author Alejandro Ciuba (alejandrociuba@gmail.com)
 * @brief Vector struct and vector operations based on the vec struct and ops.
 * Modified from array_list.h (arl project).
 * @version 0.1
 * @date 2022-18-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

#define metadata size_t size; bool fixed_length;

/**
 * @brief Appends a new component to the end of the vector.
 * Upsizes by 2N where N = # of components if double_size == true; 
 * sets v NULL if no v. Sets v unchanged if v->fixed_length == true,
 * or type == void.
 *
 * @param vector Vector to be appended to.
 * @param data Data to be appended.
 * @param type Data type (char|int_32_t|int_64_t|float|double|void)
 * @param double_size If true, upsize makes vector twice as big with padded 0s.
 */
#define append(vector, type, data, double_size) ({ \
    if (vector == NULL) \
    })

/**
 * @brief The vector structs containing the following:
 * - type* array: components of the vector, access with v->array[index].
 * - size_t size: Size of array in bytes
 * - bool fixed_length: whether the vector can change in size. Default true.
 *
 * types: vec_char, vec_int_32, vec_int_64, vec_float, vec_double, and vec_void
 */
typedef struct vector_char {

    char* array;
    metadata;
} vec_char;

typedef struct vector_int_32_t {

    int32_t* array;
    metadata;
} vec_int_32;

typedef struct vector_int_64_t {

    int64_t* array;
    metadata;
} vec_int_64;

typedef struct vector_float {

    float* array;
    metadata;
} vec_float;

typedef struct vector_double {

    double* array;
    metadata;
} vec_double;

typedef struct vector_void {

    void* array;
    metadata;
} vec_void;

// ===================== FUNCTIONS =====================

/**
 * @brief Initialize a malloc'd instance of a vector. NO_TYPE
 * gives you a void* array. Cast to your desired vector like
 * regular malloc(). All components start off as 0 in their
 * respective formats.
 *
 * @param init_size The starting size (sizeof(type) * size).
 * @param fixed_length Vector cannot dynamically change size;
 * any operations that will alter its size will instead leave
 * the vector completely unchanged.
 * @return vec* | NULL
 */
void* init_vec(size_t init_size, bool fixed_length);
#endif