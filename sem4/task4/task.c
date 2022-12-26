#include <stdio.h>
#include "print_file.h"
#include "print_string.h"
int main() {
    print_string("Enter file name: ");
    char fileName[100];
    int res = scanf("%19s", fileName);
    if (res==EOF) {
        print_string("name too long, sorry");
        return 1;
    }
    print_string("Here's your file: \n");
    print_file(fileName);
    return 0;
}