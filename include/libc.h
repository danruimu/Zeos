/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>

int write(int fd, char *buffer, int size);

void itoa(int a, char *b);

int atoi(char *str,int *i);

int strlen(char *a);

int getpid();

unsigned int rand(void);

void srand(unsigned int seed);

unsigned long int gettime();

void perror(char *message);

int printz(char *missatge);

int fork();

void exit();

int getStats(unsigned int pid,struct stats *st);

int setpriority(unsigned int pid,unsigned int priority);

void provoca_PageFault();

#endif  /* __LIBC_H__ */
