#include <libc.h>
char *string;

int __attribute__((__section__(".text.main")))
main(void) {
	sem_init(1, 0);
	int pid = fork();
	if(pid==0) {
		printz("En el nombre del padre, ");
		sem_signal(1);
		exit();
	} else {
		sem_wait(1);
		printz("del hijo, ");
		sem_destroy(1);
		
		sem_init(2,0);
		pid = fork();
		if(pid==0) {
			printz("del esperitu santo...\n");
			sem_signal(2);
			exit();
		} else {
			sem_wait(2);
			printz("Amen\n");
			sem_destroy(2);
		}	
	}
	while(1);
}
