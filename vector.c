/**
 * Vector for general purposes
 * Running this on Valgrind generates
 * XX
 * Modified from my arl project.
 * @author Alejandro Ciuba
 */

#include "vector.h"
#include <stdlib.h>
#include <string.h>

 // ===================== FUNCTIONS =====================

 /**
  * @brief Initialize a malloc'd instance of a vector. NO_TYPE
  * initializes all to NULL and you can do whatever you want for
  * that; however, no other functions will work with the vector
  * besides contains (only with a custom comparator).
  *
  * @param init_capacity The starting number of components.
  * @param data_type Data type for this vector: NO_TYPE, CHAR,
  * INT32, INT64, FLOAT32, and DOUBLE.
  * @param fixed_length Vector cannot dynamically change size;
  * any operations that will alter its size will instead throw
  * an error.
  * @return vec* | NULL
  */
vec* init_vec(int init_size, type data_type, bool fixed_length) {

    if (init_size == 0) return NULL;

    // Init vec struct
    vec* v = (vec*)malloc(sizeof(vec));
    if (v == NULL) return NULL;

    // Init the size, data_type, and fixed_length metadata
    v->size = init_size;
    v->data_type = data_type;
    v->fixed_length = fixed_length;

    // Init the vector array
    void* arrv = malloc(get_data_size(data_type) * init_size);
    if (arrv == NULL) {

        free(v);
        return NULL;
    }

    v->array = arrv;
    return v;
}

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
vec* append(const void* data, vec* v, bool double_size) {

    if (data == NULL || v == NULL) return v;
    if (v->fixed_length || v->data_type == NO_TYPE) return v;

    // If double_size, upsize
    // Else, just insert
    type data_type = v->data_type;
    int og_size = v->size;
    size_t data_size = get_data_size(v->data_type);

    if (double_size) {

        v = upsize(v);

        if (v == NULL) return NULL;

        // Insert
        memcpy(v->array + og_size * data_size, data, data_size);

        return v;

    } else {

        // Initialize new vector
        vec* new_v = init_vec(og_size + 1, data_type, v->fixed_length);
        if (new_v == NULL) return v;

        // Copy data and insert
        // Insert
        for (int i = 0; i < og_size; i++)
            memcpy(new_v->array + i * data_size, v->array + i * data_size, data_size);

        memcpy(new_v->array + og_size * data_size, data, data_size);

        free_vec(v);

        return new_v;
    }
}

/**
 * @brief Replace the value at a given vector position.
 * If you have v->fixed_length == true and index == size, or
 * if index >= size or data == NULL. it will return false.
 * Otherwise, it will return true.
 *
 * @param data Data to replace current data. Is not type-checked.
 * @param index Index at which to replace.
 * @param v Vector to have component replaced.
 * @return bool
 */
bool replace(const void* data, int index, vec* v) {

    if (data == NULL || v == NULL) return false;
    if (index >= v->size || v->data_type == NO_TYPE) return false;

    // Insert
    size_t data_size = get_data_size(v->data_type);

    memcpy(v->array + index * data_size, data, data_size);
    return true;
}

/**
 * @brief Checks if a vector contains a value. Can take a user supplied
 * function. If the function is left NULL, uses default function. NO_TYPE
 * and comparator == NULL returns false by default.
 *
 * @param data Data to look for. Is not type-checked. Returns false if
 * data == NULL.
 * @param v Vector to look at. Returns false if v == NULL.
 * @param comparator Either NULL for default function or custom function
 * such that: 1 if arg1 > arg2, -1 if arg1 < arg2, and 0 if arg1 == arg2.
 * @return bool
 */
bool contains(const void* data, vec* v, char (*comparator)(const void* arg1, const void* arg2)) {

    if (data == NULL || v == NULL) return false;
    if (v->data_type == NO_TYPE && comparator == NULL) return false;

    // Use for comparator 
    size_t data_size = get_data_size(v->data_type);

    for (int i = 0; i < v->size; i++) {

        // Check if there's a comp function; otherwise, do default comp
        // for a given data_type
        if (comparator != NULL) {
            if (comparator(data, v->array + i * data_size) == 0) return true;
        } else if (strncmp(v->array + i * data_size, (char*)data, data_size) == 0) return true;
    }

    return false;
}

/**
 * @brief Get the deep-copy of the component at index position.
 * REMEMBER TO FREE!!! REMEMBER TO CAST WHEN RECEIVING!!!
 *
 * @param index Component to be gotten. If index > v->size, returns NULL.
 * @param v Vector. If NULL returns NULL.
 * @return void* (Cast to type) | NULL
 */
void* get_deep(int index, vec* v) {

    if (v == NULL) return v;
    if (index > v->size) return NULL;

    size_t data_size = get_data_size(v->data_type);

    void* data_cpy = malloc(data_size);
    if (data_cpy == NULL) return NULL;

    return (void*)memcpy(data_cpy, v->array + index * data_size, data_size);
}

/**
 * @brief Gets the appropriate size_t for v->data_type.
 *
 * @param data_type v->data_type.
 * @return size_t
 */
size_t get_data_size(type data_type) {

    switch (data_type) {

        case NO_TYPE:
            return sizeof(void);

        case CHAR:
            return sizeof(char);

        case INT32:
            return sizeof(int32_t);

        case INT64:
            return sizeof(int64_t);

        case FLOAT32:
            return sizeof(float);

        case DOUBLE:
            return sizeof(double);

        default:
            return 0x00;
    }
}

/**
 * @brief Get the size of the array components (in bytes).
 * Based on initialized components (aka v->size). Returns 0
 * if v == NULL or v->array == NULL.
 *
 * @param v Vector.
 * @return size_t
 */
size_t byte_size(vec* v) {

    if (v == NULL) return 0;
    if (v->array == NULL) return 0;

    return get_data_size(v->data_type) * v->size;
}

/**
 * @brief Upsizes a given vector if v->fixed_length == false.
 * Otherwise, returns v unchanged.
 *
 * @param v Vector to be upsized.
 * @return vec* | NULL
 */
vec* upsize(vec* v) {

    if (v == NULL) return NULL;
    if (v->fixed_length) return v;

    // Initialize new vector
    vec* new_v = init_vec(v->size * 2, v->data_type, v->fixed_length);
    if (new_v == NULL) return v;

    // Copy all the data
    int size = v->size;
    for (int i = 0; i < size; i++)
        if (!replace((v->array + i * size), i, new_v)) return v;

    // Free old v and return new_v
    free_vec(v);

    return new_v;
}

/**
 * @brief Downsizes a given vector by (int)(v->size / 2) if v->fixed_length == false.
 * Otherwise, returns v unchanged.
 *
 * @param v Vector to be downsized.
 * @return vec* | NULL
 */
vec* downsize(vec* v) {

    if (v == NULL) return NULL;
    if (v->fixed_length) return v;

    // Initialize new vector
    vec* new_v = init_vec(v->size / 2, v->data_type, v->fixed_length);
    if (new_v == NULL) return v;

    //Copy all the data
    int size = new_v->size;
    for (int i = 0; i < size; i++)
        if (!replace((v->array + i * size), i, new_v)) return v;

    // Free old v and return new_v
    free_vec(v);

    return new_v;
}

/**
 * @brief Frees the given vector and all its components.
 *
 * @param v Vector to be freed
 */
void free_vec(vec* v) {

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