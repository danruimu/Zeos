#include <libc.h>

int pid;

int __attribute__((__section__(".text.main")))
main(void) {
    char *buff = "porqueria_varia";
    pid = 1;
    while (1) {
        pid = fork();
        if (pid == -1) perror("");
        if (pid == 0) {
            itoa(getpid(), buff);
            printz("Hijo PID --> ");
            printz(buff);
            printz("\n");
        } else {
            itoa(getpid(), buff);
            printz("Padre PID --> ");
            printz(buff);
            printz("\n");
        }
        if (pid != 0) {
            itoa(getpid(), buff);
            printz("Soy ");
            printz(buff);
            printz(" y hago el exit\n");
            exit();
            printz("Error, aqui no deberia haber llegado\n\n\n\n");
        }
        
        //provoca_PageFault();
        
    }
    
    while (1);
    return 0;
}
