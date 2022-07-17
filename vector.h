/**
 * @file vector.h
 * @author Alejandro Ciuba (alejandrociuba@gmail.com)
 * @brief Vector struct and vector operations based on the vec struct and ops.
 * Modified from array_list.h (arl project).
 * @version 0.1
 * @date 2022-06-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef enum { NO_TYPE, CHAR, INT32, INT64, FLOAT32, DOUBLE } type;

/**
 * @brief The vector struct containing the following:
 * - void** array: components of the vector, access with v->array[index].
 * - int size: Number of components.
 * - int capacityL How many components it could store until upsize()
 * should be called. Starts the same as size but changes if
 * v->fixed_length == false.
 * - type data_type: Data type stored in this vector, only this type.
 * - bool fixed_length: Whether or not you can change the size of the
 * vector.
 *
 */
typedef struct vector {

    // An array to any data type listed
    void** array;
    int size;
    type data_type;
    bool fixed_length;
} vec;

// ===================== FUNCTIONS =====================

/**
 * @brief Initialize a malloc'd instance of a vector. NO_TYPE
 * initializes all to NULL and you can do whatever you want for
 * that; however, no other functions will work with the vector.
 * Size should always be equal to capacity iff v->fixed_length == true.
 *
 * @param init_capacity The starting number of components.
 * @param data_type Data type for this vector: NO_TYPE, CHAR,
 * INT32, INT64, FLOAT32, and DOUBLE.
 * @param fixed_length Vector cannot dynamically change size;
 * any operations that will alter its size will instead throw
 * an error.
 * @return vec* | NULL
 */
vec* init_vec(int init_capacity, type data_type, bool fixed_length);

/**
 * @brief Appends a new component to the end of the vector.
 * Upsizes by 2N where N = # of components if double_size == true; returns NULL if no v. Returns v unchanged
 * if v->fixed_length == true or data == NULL.
 *
 * @param data Data to be appended. DOES NOT CHECK TYPE.
 * @param v Vector to be appended to.
 * @param double_size If true, upsize makes vector twice as big with padded 0s.
 * @return vec* | NULL
 */
vec* append(const void* data, vec* v, bool double_size);

/**
 * @brief Replace the value at a given vector position.
 * If you have v->fixed_length == true and index == size,
 * it will return NULL. Otherwise, it will return v unchanged
 * if index >= size or data == NULL.
 *
 * @param data Data to replace current data. Is not type-checked.
 * @param index Index at which to replace.
 * @param v Vector to have component replaced.
 * @return vec* | NULL
 */
vec* replace(const void* data, int index, vec* v);

/**
 * @brief Checks if a vector contains a value. Can take a user supplied
 * function. If the function is left NULL, uses default function.
 *
 * @param data Data to look for. Is not type-checked. Returns false if
 * data == NULL.
 * @param v Vector to look at. Returns false if v == NULL.
 * @param comparator Either NULL for default function or custom function
 * such that: 1 if arg1 > arg2, -1 if arg1 < arg2, and 0 if arg1 == arg2.
 * @return bool
 */
bool contains(const void* data, vec* v, char (*comparator)(const void* arg1, const void* arg2));

/**
 * @brief Get the deep-copy of the component at index position.
 * REMEMBER TO FREE!!! REMEMBER TO CAST WHEN RECEIVING!!!
 *
 * @param index Component to be gotten. If index > v->size, returns NULL.
 * @param v Vector. If NULL returns NULL.
 * @return void* (Cast to type) | NULL
 */
void* get_deep(int index, vec* v);

/**
 * @brief Get the shallow-copy of the component at index position.
 * REMEMBER, NO UNWARRANTED CHANGES!!!
 *
 * @param index Component to be gotten. If index > v->size, returns NULL.
 * @param v Vector. If NULL returns NULL.
 * @return void* (Cast to type) | NULL
 */
void* get_shallow(int index, vec* v);

/**
 * @brief Gets the appropriate size_t for v->data_type.
 *
 * @param data_type v->data_type.
 * @return size_t
 */
size_t get_data_size(type data_type);

/**
 * @brief Get the size of the array components (in bytes).
 * Based on initialized components (aka v->size). Returns 0
 * if v == NULL or v->array == NULL.
 *
 * @param v Vector.
 * @return size_t
 */
size_t byte_size(vec* v);

/**
 * @brief Upsizes a given vector if v->fixed_length == false.
 * Otherwise, returns v unchanged.
 *
 * @param v Vector to be upsized.
 * @return vec* | NULL
 */
vec* upsize(vec* v);

/**
 * @brief Downsizes a given vector if v->fixed_length == false.
 * Otherwise, returns v unchanged.
 *
 * @param v Vector to be downsized.
 * @return vec* | NULL
 */
vec* downsize(vec* v);

/**
 * @brief Frees the given vector and all its components.
 *
 * @param v Vector to be freed
 */
void free_vec(vec* v);

#endif