#include <libc.h>

char buff[100];
char stack[100];
char *string;

void pfunct(void) {
	read(0, buff, 10);
	printz("me he desbloqueado\n");
	printz(buff);
	exit();
}

int __attribute__((__section__(".text.main")))
main(void) {
//    string = " ";
//    sem_init(1, 0);
    int pid = fork();
//    sem_wait(1);
    itoa(pid, string);
    printz("pid = ");
    printz(string);
    printz("\n");
//    clone(pfunct, stack);
    while (1);
    return 0;
}
