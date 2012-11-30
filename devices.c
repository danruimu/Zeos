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
    if(finBuffer<TAM_BUFF && iniBuffer == 0) {  //caso buffer inicial o completo desde 0 a TAM_BUFF-1
        read_buff[finBuffer++] = llegirImprimirTecla();
    } else if(finBuffer < iniBuffer-1) {          //caso ya tenemos el buffer circular
        read_buff[finBuffer++] = llegirImprimirTecla();
    } else if(!finBuffer && !iniBuffer) {       //primera tecla pulsada en general
        read_buff[finBuffer++] = llegirImprimirTecla();
    }
    //si finBuffer==TAM_BUFF y iniBuffer==0 o finBuffer==iniBuffer!=0 tenemos el buffer
    //completo y no tenemos que leer
    if(!list_empty(&blocked)) {
        struct readStruct *lectorActual;
        lectorActual = list_head_to_lectura(list_first(blocked));
        if(finBuffer > iniBuffer) {
            if(lectorActual->tamany <= TAM_BUFF) {
                if(lectorActual->tamany == (finBuffer - iniBuffer + 1) ) {
                    copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, finBuffer-iniBuffer + 1);
                    iniBuffer = finBuffer = 0;
                    list_del(&lectorActual->PCB->entry);
                    encuaReady(lectorActual->PCB);
                }
            } else {
                if(TAM_BUFF == (finBuffer - iniBuffer + 1) ) {
                    copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, finBuffer-iniBuffer + 1);
                    iniBuffer = finBuffer = 0;
                    lectorActual->blocsLlegits++;
                    lectorActual->tamany -= TAM_BUFF;
                }
            }
        } else if (iniBuffer > finBuffer) {
            if(lectorActual->tamany <= ((TAM_BUFF - iniBuffer +1) + finBuffer + 1) ) {
                copy_to_user(&read_buff[iniBuffer], lectorActual->buffer, TAM_BUFF - iniBuffer + 1);
                copy_to_user(&read_buff[0], &lectorActual->buffer[TAM_BUFF - iniBuffer + 2], finBuffer+1);
                iniBuffer = finBuffer = 0;
                list_del(&lectorActual->PCB->entry);
                encuaReady(lectorActual->PCB);
            } else {
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

