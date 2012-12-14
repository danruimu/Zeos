#include <libc.h>
#define LA_RESPUESTA_A_LA_VIDA_EL_UNIVERSO_Y_TODO_LO_DEMAS	42
#define HERRAMIENTA_QUE_LO_ARREGLA_TODO	1

char *flag;

int __attribute__((__section__(".text.main")))
main(void) {
    clean_screen();
    flag = (char*) sbrk(100);
    flag[0] = LA_RESPUESTA_A_LA_VIDA_EL_UNIVERSO_Y_TODO_LO_DEMAS;
    flag[1] = LA_RESPUESTA_A_LA_VIDA_EL_UNIVERSO_Y_TODO_LO_DEMAS+HERRAMIENTA_QUE_LO_ARREGLA_TODO;

    sem_init(1, 0);
    int pid = fork();
    if(pid==0) {
            if(flag[0]==LA_RESPUESTA_A_LA_VIDA_EL_UNIVERSO_Y_TODO_LO_DEMAS) printz("En el nombre del padre, ");
            sem_signal(1);
            sem_init(2,0);
            pid = fork();
            if(pid==0) {
                    if(flag[1] == LA_RESPUESTA_A_LA_VIDA_EL_UNIVERSO_Y_TODO_LO_DEMAS+HERRAMIENTA_QUE_LO_ARREGLA_TODO) printz("del esperitu santo...\n");
                    sem_signal(2);
                    exit();
            } else {
                    sem_wait(2);
                    printz("Amen\n");
                    sem_destroy(2);
            }	
            exit();
    } else {
            sem_wait(1);
            printz("del hijo, ");
            sem_destroy(1);

    }
    while(1);
}
