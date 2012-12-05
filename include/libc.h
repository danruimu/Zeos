/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>

int write(int fd, char *buffer, int size);

int read(int fd, char *buffer, int count);

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

int clone(void (*function)(void), void *stack);

void exit();

int getStats(unsigned int pid,struct stats *st);

int setpriority(unsigned int pid,unsigned int priority);

void provoca_PageFault();

int sem_init(int n_sem, unsigned int value);

int sem_wait(int n_sem);

int sem_signal(int n_sem);

int sem_destroy(int n_sem);

void *sbrk(int increment);

#endif  /* __LIBC_H__ */
