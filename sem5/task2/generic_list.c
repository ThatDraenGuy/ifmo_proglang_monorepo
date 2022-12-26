#include <stdint.h>
#include "generic_print.h"

#define DEFINE_LIST(type)                                               \
  struct list_##type {                                                  \
    type value;                                                         \
    struct list_##type* next;                                           \
  };                                                                    \
  struct list_##type* node_##type##_create( type value ) {              \
    struct list_##type* node = malloc(sizeof(struct list_##type));      \
    node->value = value;                                                \
    return node;                                                        \
  }                                                                     \
  void list_##type##_add_front( struct list_##type** old, int64_t value ) {\
    struct list_##type* new = node_##type##_create(value);              \
    new->next = *old;                                                   \
    *old = new;                                                         \
  }                                                                     \
  struct list_##type* list_##type##_last( struct list_##type * list ) { \
    struct list_##type* last = list;                                    \
    while (list) {                                                      \
        last = list;                                                    \
        list = list->next;                                              \
    }                                                                   \
    return last;                                                        \
  }                                                                     \
  void list_##type##_push(struct list_##type** old, type value) {       \
    if (*old) {                                                         \
        struct list_##type* last = list_##type##_last(*old);            \
        struct list_##type* new = node_##type##_create(value);          \
        last->next = new;                                               \
        return;                                                         \
    }                                                                   \
    list_##type##_add_front(old, value);                                \
  }                                                                     \
  void list_##type##_print(struct list_##type* list) {                  \
    while(list) {                                                       \
        print(list->value);                                             \
        printf("\n");                                                   \
        list = list->next;                                              \
    }                                                                   \
  }                                                                     \

DEFINE_LIST(int64_t)
DEFINE_LIST(double)
DEFINE_LIST(uint64_t)



#define list_push(list, x)                                          \
    _Generic((list),                                                \
        struct list_int64_t**   :   list_int64_t_push,     \
        struct list_double**  :   list_double_push,      \
        struct list_uint64_t**  :   list_uint64_t_push)(list, x)    \
    
#define list_print(list)                                        \
    _Generic((list),                                            \
        struct list_int64_t*    :   list_int64_t_print,   \
        struct list_double*   :   list_double_print,    \
        struct list_uint64_t*   :  list_uint64_t_print)(list)   \

void task3() {
    struct list_int64_t* intList = NULL;
    list_push(&intList, 5);
    list_push(&intList, 5);
    list_push(&intList, 5);
    list_print(intList);

    struct list_double* doubleList = NULL;
    list_push(&doubleList, 2.0);
    list_push(&doubleList, 2.0);
    list_print(doubleList);

    struct list_uint64_t* uintList = NULL;
    list_push(&uintList, 1);
    list_push(&uintList, 1);
    list_push(&uintList, 1);
    list_push(&uintList, 1);
    list_push(&uintList, 1);
    list_push(&uintList, 1);
    list_push(&uintList, 1);
    list_print(uintList);
}


int main() {
    // struct list_int64_t* intList = node_int64_t_create(5);
    struct list_int64_t* intList = NULL;
    list_int64_t_push(&intList, 7);
    list_int64_t_push(&intList, 3);
    list_int64_t_print(intList);
    
    struct list_double* doubleList = node_double_create(2.0);
    list_double_push(&doubleList, 1.0);
    list_double_print(doubleList);

    struct list_uint64_t* uintList = node_uint64_t_create(1);
    list_uint64_t_push(&uintList, 1);
    list_uint64_t_push(&uintList, 1);
    list_uint64_t_push(&uintList, 1);
    list_uint64_t_push(&uintList, 1);
    list_uint64_t_push(&uintList, 1);
    list_uint64_t_print(uintList);

    newline_print();
    task3();
}