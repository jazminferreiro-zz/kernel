#include <stdio.h>

static void printf_sizeof_buf(char buf[256]) {
    printf("sizeof buf = %zu\n", sizeof buf);
}

int main(void) {
    char buf[256];
    printf("sizeof buf = %zu\n", sizeof buf);
    printf_sizeof_buf(buf);
}