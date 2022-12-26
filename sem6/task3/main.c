
#include "vector.h"

int main() {
    struct vector* vector = vector_new();
    vector_push(vector, 2);
    vector_push(vector, 3);
    vector_push(vector, 4);
    vector_push(vector, 1);
    vector_push(vector, 2);
    vector_push(vector, 3);
    vector_push(vector, 4);
    vector_push(vector, 1);
    vector_push(vector, 2);
    vector_push(vector, 3);
    vector_push(vector, 4);
    vector_push(vector, 1);

    vector_print(vector, stdout);

    fprintf(stdout, "\n");
    vector_change_size(vector, 5);
    vector_print(vector, stdout);

    fprintf(stdout, "\n");
    vector_add(vector, vector);
    vector_print(vector, stdout);

    fprintf(stdout, "\n");
    vector_set(vector, 0, 5);
    vector_print(vector, stdout);

    fprintf(stdout, "\n");
    int64_t value = vector_get(vector, 0).value;
    fprintf(stdout, "%"PRId64, value);

    fprintf(stdout, "\n");
    vector_set(vector, 9, 5);
    vector_print(vector, stdout);

    fprintf(stdout, "\n");
    fprintf(stdout, "length: %"PRIu64"; capacity: %"PRIu64, vector_length(vector), vector_capacity(vector));

    fprintf(stdout, "\n");
    vector_push(vector, 0);
    fprintf(stdout, "length: %"PRIu64"; capacity: %"PRIu64, vector_length(vector), vector_capacity(vector));

    fprintf(stdout, "\n");
    vector_change_size(vector, 30);
    fprintf(stdout, "length: %"PRIu64"; capacity: %"PRIu64, vector_length(vector), vector_capacity(vector));
    fprintf(stdout, "\n");

    vector_change_size(vector, 0);
    vector_push(vector, 55);
    fprintf(stdout, "length: %"PRIu64"; capacity: %"PRIu64, vector_length(vector), vector_capacity(vector));
    fprintf(stdout, "\n");
    vector_print(vector, stdout);
    fprintf(stdout, "\n");
    vector_change_size(vector, 0);
    vector_add(vector, vector);
    vector_print(vector, stdout);
    fprintf(stdout, "length: %"PRIu64"; capacity: %"PRIu64, vector_length(vector), vector_capacity(vector));

    fprintf(stdout, "\n");
    vector_destroy(vector);
}