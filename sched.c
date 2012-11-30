/*
 * sched.c - initializes struct for task 0 and a task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <utils.h>
#include <libc.h>
#include <errno.h>

enum schedulling_p politica = RR; // apliquem round robin
struct task_struct *idle_task;
int pidMesNou = 0;



union task_union task[NR_TASKS]
__attribute__((__section__(".data.task")));

LIST_HEAD(freeQueue);

LIST_HEAD(readyQueue);

//#if 0

struct task_struct *list_head_to_task_struct(struct list_head *l) {
    return list_entry(l, struct task_struct, entry);
}
//#endif 

extern struct list_head blocked;

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR(struct task_struct *t) {
    return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT(struct task_struct *t) {
    return (page_table_entry *) (((unsigned int) (t->dir_pages_baseAddr->bits.pbase_addr)) << 12);
}

void cpu_idle(void) {
    __asm__ __volatile__("sti" : : : "memory");
    while (1);
}

void init_idle(void) {
    idle_task = list_head_to_task_struct(list_first(freeQueue));
    list_del(&idle_task->entry);
    idle_task->PID = 0;
    pidMesNou++;
    idle_task->quantum = QUANTUM_NORMAL;
    idle_task->estadistiques.cs = 0;
    idle_task->estadistiques.remaining_quantum = QUANTUM_NORMAL;
    idle_task->estadistiques.tics = 0;
    idle_task->estado = ST_READY;
    idle_task->priority = 0;
    union task_union *unio = (union task_union*)idle_task;
    unio->stack[KERNEL_STACK_SIZE - 1] = (long) &cpu_idle;
    unio->stack[KERNEL_STACK_SIZE - 2] = 0;
    idle_task->kernel_esp = (unsigned int) &(unio->stack[KERNEL_STACK_SIZE - 2]);
}

void init_task1(void) {
    struct list_head* listPCBfree = list_first(freeQueue);
    struct task_struct* PCBtask1 = list_head_to_task_struct(listPCBfree);
    list_del(listPCBfree);
    PCBtask1->PID = 1;
    pidMesNou++;
    PCBtask1->quantum = QUANTUM_NORMAL;
    PCBtask1->estadistiques.remaining_quantum = QUANTUM_NORMAL;
    PCBtask1->estadistiques.tics = 0;
    PCBtask1->estadistiques.cs = 0;
    PCBtask1->estado = ST_RUN;
    PCBtask1->priority = 42;
    pagines_usades[0]++;
    set_user_pages(PCBtask1);
    set_cr3(PCBtask1->dir_pages_baseAddr);
}

void init_sched() {
    int i;
    for (i = 0; i < NR_TASKS; i++) {
        list_add_tail(&(task[i].task.entry), &freeQueue);
    }
}

struct task_struct* current() {
    int ret_value;

    __asm__ __volatile__(
            "movl %%esp, %0"
            : "=g" (ret_value)
            );
    return (struct task_struct*) (ret_value & 0xfffff000);
}

void inline task_switch(union task_union*t) {
    __asm__ __volatile__(
            "movl %%ebp,%0"//guardar al pcb actual el esp
            : "=g"(current()->kernel_esp) : : "memory");
    tss.esp0 = (DWord) &(t->stack[KERNEL_STACK_SIZE]); //updatejar la tss per apuntar al l'stack de t
    if (t->task.dir_pages_baseAddr != current()->dir_pages_baseAddr)
        set_cr3(get_DIR(&t->task)); //canviar l'espai d'adresses
    __asm__ __volatile__(
            "movl %0,%%esp\n\t"//fas que l'stack apunti a la pila del nou proces
            "popl %%ebp\n\t"//restore ebp
            "ret"
            : : "g"(t->task.kernel_esp) : "memory");
}

int nouPid() {
    return pidMesNou++;
}

void encuaReady(struct task_struct *t) {
    list_add_tail(&t->entry, &readyQueue);
}

void updateSchedullingData() {
    struct list_head *i;
    if(current()->PID == 0) return;
    switch (politica) {
        case RR:
            current()->estadistiques.remaining_quantum--;
            break;
        case PRIOR:
            list_for_each(i, &readyQueue)
        {
            list_head_to_task_struct(i)->priority++; //aging
        }
            break;
        case MULTI_LIST:
            break;
        default://FCFS
            break;
    }
}

int checkSchedulling() {
    if(current()->PID == 0){
        if(list_empty(&readyQueue))return 0;
        else return 1;
    }
    switch (politica) {
        case RR:
            if (current()->estadistiques.remaining_quantum == 0) return 1;
            break;
        case PRIOR:
            if (!list_empty(&readyQueue) && list_head_to_task_struct(list_first(readyQueue))->priority > current()->priority)return 1;
            break;
        case MULTI_LIST:
            break;
        default://FCFS
            break;
    }
    return 0;
}

void switcher() {
    struct task_struct *nou = idle_task;
    switch (politica) {
        case MULTI_LIST:
            break;
        default://FCFS & RR & PRIOR
            if (!list_empty(&readyQueue)) {
                nou = list_head_to_task_struct(list_first(readyQueue));
            }
            break;
    }
    nou->estadistiques.cs++;
    list_del(&nou->entry);
    nou->estado = ST_RUN;
    task_switch((union task_union*)nou);
}

void updateQueuesStates() {
    struct list_head *i;
    struct task_struct* tret = current();
    if (tret->PID != 0) {
        switch (politica) {
           case RR:
               list_add_tail(&tret->entry, &readyQueue);
               tret->estadistiques.remaining_quantum = tret->quantum;
               break;
           case PRIOR:
               if (list_empty(&readyQueue))list_add_tail(&tret->entry, &readyQueue);
               else {

		   list_for_each(i, &readyQueue) {
		       struct task_struct *PCB = list_head_to_task_struct(i);
		       if (PCB->priority >= tret->priority) {
			   if (i->next == &readyQueue) {
		               list_add(&tret->entry, i);
                           }
                       } else {
                           list_add_tail(&tret->entry, i);
                           break;
                       }
                   }
               }
               break;
           case MULTI_LIST:
               break;
           default://FCFS
               list_add_tail(&tret->entry, &readyQueue);
               break;
        }
        tret->estado = ST_READY;
    }
}

