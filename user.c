#include <libc.h>

int pid = -20;

char buff[5];
char stack[100];

void pfunct(void) {
	int res;
	printz(buff);
	res = read(0, buff, 5);
	if(res > 0) printz(buff);
	else perror("");
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
    //clone(pfunct, stack);
    char *buffer = "p";
    pid = getpid();
    atoi(buffer, &pid);
    printz(buff);
    while (1);
    return 0;
}
