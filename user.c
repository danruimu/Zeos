#include <libc.h>

int pid;

char *buff = "porqueria_varia";
char stack[100];

void caca(void) {
	printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
    pid = clone(caca, &stack);
    while (1);
    return 0;
}
