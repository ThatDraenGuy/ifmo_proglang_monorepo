#ifndef GENERIC_PRINT_H
#define GENERIC_PRINT_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char *s);

#define _print(type, x) type##_print(x)

// Обратите внимание на обратные слэши в конце каждой строчки кроме последней!
// Они экранируют переводы строк, позволяя написать макрос во много строчек.
#define print(x)                                                        \
  _Generic((x),                                                         \
           int64_t : int64_t_print(x),                                  \
           uint64_t : uint64_t_print(x),                                  \
           double : double_print(x),                                    \
           default : error("Unsupported operation"))

void int64_t_print(int64_t i);
void double_print(double d);
void uint64_t_print(uint64_t u);
void newline_print();

#endif
