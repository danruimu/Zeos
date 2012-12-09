/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>
#include <stats.h>

#define NR_TASKS      10
#define KERNEL_STACK_SIZE	1024
#define QUANTUM_NORMAL          42
#define SEM_VALUE_MAX           42

enum schedulling_p {FCFS, RR,PRIOR,MULTI_LIST};
enum state_t { ST_RUN, ST_READY, ST_BLOCKED, ST_ZOMBIE };

struct task_struct {
  int PID;		/* Process ID */
  page_table_entry * dir_pages_baseAddr;
  struct list_head entry;
  unsigned int kernel_esp;
  struct stats estadistiques;
  unsigned int quantum; 
  unsigned int priority;
  enum state_t estado;
  char *buffer;
  int tamany;
  int blocsLlegits;
  unsigned char sem_usats[SEM_VALUE_MAX];
  unsigned long *heap_break;
};

union task_union {
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

struct sem {
  int counter;
  unsigned short used;
  int propietari;
  struct list_head blockedQueue;
};

struct sem semaphores[SEM_VALUE_MAX];

extern union task_union task[NR_TASKS]; /* Vector de tasques */
extern struct task_struct *idle_task;

#define KERNEL_ESP       (DWord) &task[1].stack[KERNEL_STACK_SIZE]

/* Inicialitza les dades del proces inicial */
void init_task1(void);

void init_idle(void);

void init_sched(void);

struct task_struct * current();

void inline task_switch(union task_union*t);

struct task_struct *list_head_to_task_struct(struct list_head *l);

page_table_entry * get_PT (struct task_struct *t) ;

page_table_entry * get_DIR (struct task_struct *t) ;


void encuaReady(struct task_struct *t);
void updateSchedullingData();
int checkSchedulling();
void switcher();
void updateQueuesStates();
int nouPid();
#endif  /* __SCHED_H__ */
