/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>
#include "list.h"
#define NUM_ESPAIS_TAB 4
/** Screen functions **/
/**********************/

struct readStruct
{
	struct task_struct *PCB;
	char *buffer;
	int tamany;
	int blocsLlegits;
};

struct readStruct* list_head_to_lectura(struct list_head* entry);
int llegintDeTeclat();
void llegeixChar();
Byte inb (unsigned short port);
void printc(char c);
void printc_xy(Byte x, Byte y, char c);
void printk(char *string);
void printk_xy(Byte x, Byte y, char *string);
void cls();

#endif  /* __IO_H__ */
