
#include <inttypes.h>

int main() {
    int8_t x = -4;
    int8_t y = 2;
    if (x > 0)
        if (y > 0) {
            print("yes");
        }
        else {
            print("no");
        }
}