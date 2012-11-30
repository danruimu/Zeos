#include <io.h>
#include <utils.h>
#include <list.h>

#include "sched.h"
#include "keyboard.h"
#define TAM_BUFF	100

// Blocked queue for this device
LIST_HEAD(blocked);
unsigned int finBuffer = 0;
unsigned int iniBuffer = 0;
char read_buff[TAM_BUFF];

int llistaTeclatBuida() {
	return list_empty(&blocked);
}

void readChar() {
    if(finBuffer<TAM_BUFF && finBuffer > 0 && iniBuffer == 0) {  //caso buffer inicial o completo desde 0 a TAM_BUFF-1
        printk("xivato 1\n");
        read_buff[finBuffer++] = llegirImprimirTecla();
    } else if(finBuffer < iniBuffer) {          //caso ya tenemos el buffer circular
        printk("xivato 2\n");
        read_buff[finBuffer++] = llegirImprimirTecla();
    } else if(!finBuffer && !iniBuffer) {       //primera tecla pulsada en general
        printk("xivato 3\n");
        read_buff[finBuffer++] = llegirImprimirTecla();
    }

    if(!list_empty(&blocked)) {
        printk("xivato 4\n");
        struct readStruct *lectorActual;
        lectorActual = list_head_to_lectura(list_first(blocked));
        if(finBuffer > iniBuffer) {
            printk("xivato 5\n");
            char *buff = " ";
            itoa(lectorActual->tamany, buff);
            printk("\n\n\n\n lectorActual->tamany = ");
            printk(buff);
            printk("\n\n\n\n");
            if((lectorActual->tamany) <= TAM_BUFF) {
                printk("xivato 6\n");
                if(lectorActual->tamany == (finBuffer - iniBuffer + 1) ) {
                    printk("xivato 7\n");
                    copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, finBuffer-iniBuffer + 1);
                    iniBuffer = finBuffer = 0;
                    list_del(&lectorActual->PCB->entry);
                    encuaReady(lectorActual->PCB);
                }
            } else {
                printk("xivato 8\n");
                if(TAM_BUFF == (finBuffer - iniBuffer + 1) ) {
                    printk("xivato 9\n");
                    copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, finBuffer-iniBuffer + 1);
                    iniBuffer = finBuffer = 0;
                    lectorActual->blocsLlegits++;
                    lectorActual->tamany -= TAM_BUFF;
                }
            }
        } else if (iniBuffer > finBuffer) {
            printk("xivato 10\n");
            if(lectorActual->tamany <= ((TAM_BUFF - iniBuffer +1) + finBuffer + 1) ) {
                printk("xivato 11\n");
                copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, TAM_BUFF - iniBuffer + 1);
                copy_to_user(&read_buff[0], &lectorActual->buffer[TAM_BUFF - iniBuffer + 2], finBuffer+1);
                iniBuffer = finBuffer = 0;
                list_del(&lectorActual->PCB->entry);
                encuaReady(lectorActual->PCB);
            } else {
                printk("xivato 12\n");
                copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, TAM_BUFF - iniBuffer + 1);
                copy_to_user(&read_buff[0], &lectorActual->buffer[TAM_BUFF - iniBuffer + 2], finBuffer+1);
                iniBuffer = finBuffer = 0;
                lectorActual->blocsLlegits++;
                lectorActual->tamany -= TAM_BUFF;
            }
        }
    }
}

int sys_write_console(char *buffer,int size)
{
  int i;
  for (i=0; i<size; i++)
    printc(buffer[i]);
  
  return size;
}

int sys_read_console(char *buffer, int size) {
	struct readStruct lector;
	lector.PCB = current();
	lector.buffer = buffer;
	lector.tamany = size;
	lector.blocsLlegits = 0;
	list_add_tail(&lector.PCB->entry, &blocked);
	switcher();
	return lector.tamany;
}

