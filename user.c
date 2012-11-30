#include <libc.h>

int pid = -20;

char buff[100];
char stack[100];

void pfunct(void) {
	int res;
        buff[0] = 'h';
	printz("pfunct\n");
	read(0, buff, 5);
	printz("no se bloquea\n");
	printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
    clone(pfunct, stack);
    printz("Main\n");
    while (1);
    return 0;
}
