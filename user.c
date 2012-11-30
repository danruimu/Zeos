#include <libc.h>

char buff[100];
char stack[100];

void pfunct(void) {
	read(0, buff, 10);
	printz("me he desbloqueado\n");
	printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
    clone(pfunct, stack);
    while (1);
    return 0;
}
