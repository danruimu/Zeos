#include <libc.h>
char *string;

int __attribute__((__section__(".text.main")))
main(void) {
    string = (char*)sbrk(5000);
    sbrk(-4000);
    string[0] = 'c';
    string[1] = 'a';
    string[2] = 'c';
    string[3] = 'a';
    string[4] = '\0';
    printz(string);
    while (1);
    return 0;
}
