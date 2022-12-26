#include "vector.h"

#define VECTOR_INITIAL_CAPACITY 10

struct vector {
    size_t capacity;
    size_t count;
    int64_t* array;
};

struct maybe_int64_t none_int64_t = {.valid = false};

struct maybe_int64_t some_int64_t(int64_t value) {
    return (struct maybe_int64_t) {.valid = true, .value = value};
}

struct vector* vector_new_with_capacity(size_t initial_capacity) {
    struct vector* vector = malloc(sizeof(struct vector));
    int64_t* array = malloc(initial_capacity*sizeof(int64_t));
    vector->array = array;
    vector->capacity = initial_capacity;
    vector->count = 0;
    return vector;
}

struct vector* vector_new() {
    return vector_new_with_capacity(VECTOR_INITIAL_CAPACITY);
}

void vector_destroy(struct vector* vector) {
    free(vector->array);
    free(vector);
}

size_t vector_length(struct vector* vector) {
    return vector->count;
}
size_t vector_capacity(struct vector* vector) {
    return vector->capacity;
}

struct maybe_int64_t vector_get(struct vector* vector, size_t index) {
    if (index >= vector->count) return none_int64_t;
    return some_int64_t(vector->array[index]);
}

bool vector_set(struct vector* vector, size_t index, int64_t value) {
    if (index >= vector->count) return false;
    vector->array[index] = value;
    return true;
}
static void vector_set_capacity(struct vector* vector, size_t amount) {
    vector->array = realloc(vector->array, sizeof(int64_t)*(amount));
    vector->capacity = amount;
}

static void vector_extend_by_amount(struct vector* vector, size_t amount) {
    vector_set_capacity(vector, amount + vector->capacity);
}
static void vector_extend(struct vector* vector) {
    if (vector->capacity<VECTOR_INITIAL_CAPACITY) {
        vector_extend_by_amount(vector, VECTOR_INITIAL_CAPACITY);
    } else vector_extend_by_amount(vector, vector->capacity);
}

void vector_push(struct vector* vector, int64_t value) {
    if (vector->count==vector->capacity) {
        vector_extend(vector);
    }
    vector->array[vector->count] = value;
    vector->count++;
}

void vector_add(struct vector* initial, struct vector* add) {
    size_t newCount = initial->count + add->count;
    if (newCount >= initial->capacity) {
        vector_extend_by_amount(initial, newCount - initial->capacity);
    }
    for (size_t i = 0; i < add->count; i++) {
        initial->array[initial->count+i] = add->array[i];
    }
    initial->count = newCount;
}

void vector_change_size(struct vector* vector, size_t size) {
    vector_set_capacity(vector, size);
    if (vector->count > size) vector->count = size;
}

void vector_print(struct vector* vector, FILE* file) {
    for (size_t i = 0; i < vector->count; i++) {
        fprintf(file, "%"PRId64 " ", vector->array[i]);
    }
}