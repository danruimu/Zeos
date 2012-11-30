#include <libc.h>

int *pid;

char buff[100];
char stack[100];

void pfunct(void) {
        //buff[0] = 'h';
	printz("pfunct\n");
	//read(0, buff, 5);
	//printz("no se bloquea\n");
	//printz(buff);
	itoa(*pid,buff);
	printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
	int a = 3;
	pid = &a;
    clone(pfunct, stack);
    
    printz("Main\n");
    while (1);
    return 0;
}
