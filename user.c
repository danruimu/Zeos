#include <libc.h>

int pid;

char buff[5];
char stack[100];

void caca(void) {
	printz(buff);
	read(0, buff, 5);
	printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
    clone(caca, stack);
    pid = read(0, buff, 5);
    if(!pid) printz("devuelve 0\n");
    else {
	    atoi(buff, &pid);
	    printz("devuelve ");
	    printz(buff);
	    printz("\n");
    }
    while (1);
    return 0;
}
