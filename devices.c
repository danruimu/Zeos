#include <io.h>
#include <utils.h>
#include <list.h>

#include "sched.h"
#include "keyboard.h"
#define TAM_BUFF	100

// Blocked queue for this device
LIST_HEAD(blocked);
unsigned int posBuffer = 0;
char read_buff[TAM_BUFF];

int llistaTeclatBuida() {
	return list_empty(&blocked);
}

void readChar() {
    if(posBuffer<TAM_BUFF) read_buff[posBuffer++] = llegirImprimirTecla();
    if(!list_empty(&blocked)) {
	struct readStruct *lectorActual;
	lectorActual = list_head_to_lectura(list_first(blocked));
	if(lectorActual->blocsLlegits * TAM_BUFF + posBuffer == lectorActual->tamany) {
		copy_to_user(&read_buff[0], &lectorActual->buffer[lectorActual->blocsLlegits * TAM_BUFF], posBuffer);
		list_del(&lectorActual->PCB->entry);
		encuaReady(lectorActual->PCB);
		return;
	}
	if(posBuffer == TAM_BUFF) {
		copy_to_user(&read_buff[0], &lectorActual->buffer[lectorActual->blocsLlegits * TAM_BUFF], TAM_BUFF);
		posBuffer = 0;
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
        struct task_struct* actual = current();
        list_del(&actual->entry);
	switcher();
	return lector.tamany;
}

