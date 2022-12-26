#include "generic_print.h"

void error(const char *s) {
  fprintf(stderr, "%s", s);
  abort();
}


// Обратите внимание на обратные слэши в конце каждой строчки кроме последней!
// Они экранируют переводы строк, позволяя написать макрос во много строчек.
void int64_t_print(int64_t i) { printf("%" PRId64, i); }
void double_print(double d) { printf("%lf", d); }
void uint64_t_print(uint64_t u) { printf("%" PRIu64, u); }
void newline_print() { puts(""); }