/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <stats.h>
#include <entry.h>

#define LECTURA 0
#define ESCRIPTURA 1
#define MIDA_INTERNA 100

extern struct list_head freeQueue;
extern struct list_head readyQueue;

int check_fd(int fd, int permissions) {
    if (fd != 1) return -EBADF;
    if (permissions != ESCRIPTURA) return -EACCES;
    return 0;
}

int sys_ni_syscall() {
    return -ENOSYS;
}

void ret_from_fork() {
    __asm__ __volatile__("popl %eax\n\t"
            "xorl %eax,%eax");
}

int sys_fork() {
    if (list_empty(&freeQueue))return -ENOMEM; //error out of memory?¿?
    struct list_head* listPCBfree = list_first(freeQueue);
    union task_union* fill = (union task_union*)list_head_to_task_struct(listPCBfree);
    union task_union* pare = (union task_union*) current();
    list_del(listPCBfree);
    int frames[NUM_PAG_DATA];
    int i;
    for (i = 0; i < NUM_PAG_DATA; i++) {//busca frames lliures
        frames[i] = alloc_frame();
        if (frames[i] < 0) {
            while (i >= 0)free_frame(frames[i--]);
            return -ENOMEM; // out of memory
        }
    }
    page_table_entry* PTf = get_PT(&fill->task);
    page_table_entry* PTp = get_PT(&pare->task);
    copy_data((void *) pare, (void *) fill, sizeof (union task_union)); //copia el pare al fill
    int allocata = allocate_page_dir(fill);
    if (allocata != 0)return allocata;
    for (i = PAG_LOG_INIT_CODE_P0; i < PAG_LOG_INIT_CODE_P0 + NUM_PAG_CODE; i++) {
        set_ss_pag(PTf, i, get_frame(PTp, i)); //s'allocaten al fill les pagines de codi del pare
    }
    for (i = PAG_LOG_INIT_DATA_P0; i < PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA; i++) {//s'allocaten les pagines noves al fill com a data+stack i es desallocaten del pare
        set_ss_pag(PTf, i, frames[i - PAG_LOG_INIT_DATA_P0]);
    }
    for (i = PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA; i < PAG_LOG_INIT_DATA_P0 + 2 * NUM_PAG_DATA; i++) {//copiar al fill tot el data+stack del pare allocatant cada pagina, copiant i desallocatant-la
        set_ss_pag(PTp, i, frames[i - (PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA)]);
        copy_data((void*) ((i - NUM_PAG_DATA) * PAGE_SIZE), (void*) ((i) * PAGE_SIZE), PAGE_SIZE); //als nous frames que ha trobats hi copia el seu data+stack
        del_ss_pag(PTp, i);
    }
    set_cr3(get_DIR(&pare->task)); //flush TLB
    unsigned int Pid = nouPid();
    fill->task.PID = Pid;
    fill->task.quantum = QUANTUM_NORMAL;
    fill->task.estadistiques.remaining_quantum = QUANTUM_NORMAL;
    fill->task.estadistiques.tics = 0;
    fill->task.estadistiques.cs = 0;
    fill->task.estado = ST_READY;
    unsigned long *ebp;
    __asm__ __volatile__("movl %%ebp,%0"
            : "=g"(ebp)); //obtenim el punter al ebp del pare
    int desp = ((unsigned long*) ebp - &pare->stack[0]); //calculem quantes celes de mem hi ha entre l'inici i el esp
    fill->stack[desp] = (unsigned long) (((unsigned long *) fill->stack[desp] - &pare->stack[0]) + &fill->stack[0]); //modifiquem el ebp del fill amb el del pare
    fill->stack[desp - 1] = (unsigned long) &ret_from_fork; //posem una posicio per la pila amunt per on retornarà el fill
    fill->stack[desp - 2] = (unsigned long) &fill->stack[desp]; //posem dos posicions per la pila amunt el ebp del pare
    fill->task.kernel_esp = (unsigned int) &fill->stack[desp - 2]; //diem que el kernel_esp del fill sigui la posició del ebp del pare
    list_add_tail(&fill->task.entry, &readyQueue);
    return Pid;
}

void sys_exit() {
    union task_union* act = (union task_union*) current();
    act->task.estado = ST_ZOMBIE;
    free_user_pages(&act->task);
    list_add_tail(&act->task.entry, &freeQueue);
    switcher();
}

int sys_write(int fd, char *buffer, int size) {
    int sizeorg = size;
    char buffer_sys[MIDA_INTERNA];
    int i = 0;
    int check = check_fd(fd, ESCRIPTURA);
    if (check < 0) return check;
    if (buffer == NULL) return -EFAULT;
    if (size < 0) return -EINVAL;
    if (access_ok(VERIFY_READ, buffer, size) == 0)return -EFAULT;
    while (size > MIDA_INTERNA) {
        if (copy_from_user(&buffer[i], buffer_sys, MIDA_INTERNA) < 0) return -ENOMEM;
        if (sys_write_console(buffer_sys, MIDA_INTERNA) != MIDA_INTERNA)return -EIO;
        size -= MIDA_INTERNA;
        i += MIDA_INTERNA;
    }
    if (copy_from_user(&buffer[i], buffer_sys, size) < 0) return -ENOMEM;
    if (sys_write_console(buffer_sys, size) != size) return -EIO;
    return sizeorg;
}

int sys_read(int fd, char *buffer, int count) {
    if (!count) return 0;
    else if (count < 0) return -EINVAL;
    if (check_fd(fd, LECTURA) < 0) return -EBADF;
    if (buffer == NULL) return -EFAULT;
}

int sys_clone(void (*function)(void), void *stack) {
    if (list_empty(&freeQueue))return -ENOMEM; //error out of memory?¿?
    struct list_head* listPCBfree = list_first(freeQueue);
    union task_union* nou = (union task_union*)list_head_to_task_struct(listPCBfree);
    union task_union* actual = (union task_union*) current();
    list_del(listPCBfree);
    copy_data((void *) actual, (void *) nou, sizeof (union task_union));
    ocupa_page_dir(nou);
    nou->task.PID = nouPid();
    nou->task.estadistiques.cs = 0;
    nou->task.estadistiques.remaining_quantum = nou->task.quantum;
    nou->task.estadistiques.tics = 0;
    nou->task.estado = ST_READY;
    unsigned long *ebp;
    __asm__ __volatile__("movl %%ebp,%0"
            : "=g"(ebp)); //obtenim el punter al ebp del actual
    int desp = ((unsigned long*) ebp - &actual->stack[0]); //calculem quantes celes de mem hi ha entre l'inici i el esp
    nou->stack[desp] = ebp;
    nou->stack[desp + 1] = (unsigned long) &ret_from_clone; //posem una posicio per la pila amunt per on retornarà el fill
    nou->stack[desp - 2] = (unsigned long) &stack; //posem dos posicions per la pila amunt el ebp del pare
    nou->stack[desp - 1] = (unsigned long) &function;
    nou->task.kernel_esp = (unsigned int) &nou->stack[desp]; //diem que el kernel_esp del fill sigui la posició del ebp del pare
    list_add_tail(&nou->task.entry, &readyQueue);
    return 0;
}

int sys_gettime() {
    return getTicks();
}

int sys_getpid() {
    return current()->PID;
}

int sys_getstats(unsigned int pid, struct stats *userSt) {
    if (userSt == NULL) return -EFAULT;
    if (access_ok(VERIFY_READ, userSt, sizeof (struct stats)) == 0)return -EFAULT;
    if (current()->PID == pid) {
        copy_to_user(&current()->estadistiques, userSt, sizeof (struct stats));
        return 0;
    }
    if (list_empty(&readyQueue)) return -ESRCH;
    struct list_head *entry = readyQueue.next;
    while (entry != NULL) {
        struct task_struct *PCB = list_head_to_task_struct(entry);
        if (PCB->PID == pid) {
            copy_to_user(&PCB->estadistiques, userSt, sizeof (struct stats));
            return 0;
        }
        entry = entry->next;
    }
    return -ESRCH;
}

int sys_setpriority(unsigned int pid, unsigned int priority) {
    if (current()->PID == pid) {
        current()->priority = priority;
        return 0;
    }
    if (list_empty(&readyQueue)) return -ESRCH;
    struct list_head *entry = readyQueue.next;
    while (entry != NULL) {
        struct task_struct *PCB = list_head_to_task_struct(entry);
        if (PCB->PID == pid) {
            PCB->priority = priority;
            return 0;
        }
        entry = entry->next;
    }
    return -ESRCH;
}
