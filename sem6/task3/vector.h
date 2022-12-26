#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

struct vector;

struct maybe_int64_t {
    bool valid;
    int64_t value;
};

struct vector* vector_new_with_capacity(size_t initial_capacity);
struct vector* vector_new();
void vector_destroy(struct vector* vector);
size_t vector_length(struct vector* vector);
size_t vector_capacity(struct vector* vector);
struct maybe_int64_t vector_get(struct vector* vector, size_t index);
bool vector_set(struct vector* vector, size_t index, int64_t value);
void vector_push(struct vector* vector, int64_t value);
void vector_add(struct vector* initial, struct vector* add);
void vector_change_size(struct vector* vector, size_t size);
void vector_print(struct vector* vector, FILE* file);

#endif