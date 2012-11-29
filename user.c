#include <libc.h>

int pid;

char *buff = "h";
char stack[100];

void pfunct(void) {
	int res;
	buff = "c";
	pid = getpid();
	atoi(buff, &pid);
	printz(buff);
	//res = read(0, buff, 5);
	//printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
    clone(pfunct, stack);
    buff = "l";
    printz(buff);
    while (1);
    return 0;
}
