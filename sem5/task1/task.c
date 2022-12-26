#include <stdio.h>
#define print_var(x) printf(#x " is %d", x )
#define CONST 25
int main() {
    int x = 3;
    print_var(x);
    print_var(19);
    print_var(CONST);
    return 0;
}