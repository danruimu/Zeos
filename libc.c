/*
 * libc.c 
 */

#include <libc.h>

#include <types.h>

unsigned long int next = 1;

int errno;

char* errors[] = {"Operation not permitted", "No such file of directory", "No such process", "Interrupted system call", "I/O error", "", "", "", "Bad file number", "", "", "Out of memory", "Permission denied", "Bad address", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Function not implemented", "", "", "Eres tonto chaval!"}; //tamaño 41

void itoa(int a, char *b) {
    int i, i1;
    char c;

    if (a == 0) {
        b[0] = '0';
        b[1] = 0;
        return;
    }

    i = 0;
    while (a > 0) {
        b[i] = (a % 10) + '0';
        a = a / 10;
        i++;
    }

    for (i1 = 0; i1 < i / 2; i1++) {
        c = b[i1];
        b[i1] = b[i - i1 - 1];
        b[i - i1 - 1] = c;
    }
    b[i] = 0;
}

int strlen(char *a) {
    int i;

    i = 0;

    while (a[i] != 0) i++;

    return i;
}

int write(int fd, char *buffer, int size) {
    int res;
    __asm__ __volatile__(
            "movl %1,%%ebx\n\t"
            "movl %2,%%ecx\n\t"
            "movl %3,%%edx\n\t"
            "movl $4,%%eax\n\t"
            "int $0x80\n\t"
            "movl %%eax,%0"
            : "=g" (res)
            : "g" (fd), "g"(buffer), "g" (size)
            : "ax", "bx", "cx", "dx");
    if (res >= 0)return res;
    else {
        errno = res * -1;
        return -1;
    }
}

unsigned long int gettime() {
    unsigned long int res;
    __asm__ __volatile__(
            "movl $10,%%eax\n\t"
            "int $0x80\n\t"
            "movl %%eax,%0"
            : "=g" (res)
            );
    if (res >= 0)return res;
    else {
        errno = res * -1;
        return -1;
    }
}

int getpid(void) {
    unsigned long int res;
    __asm__ __volatile__(
            "movl $20,%%eax\n\t"
            "int $0x80\n\t"
            "movl %%eax,%0"
            : "=g" (res)
            );
    if (res >= 0)return res;
    else {
        errno = res * -1;
        return -1;
    }
}

void perror(char *message) {
    if (message != NULL && message[0] != '\0')
        printz(message);
    printz(errors[errno - 1]);
    printz("\n");
}

unsigned int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int) (next / 65536) % 32768;
}

void srand(unsigned int seed) {
    next = seed;
}

int printz(char *missatge) {
    return write(1, missatge, strlen(missatge) + 1);
}

int atoi(char *str, int *i) {
    int aux = 0;
    int size = 0;
    if (str == NULL || i == NULL)return 0;
    *i = 1;
    if (str[0] == '-') {
        *i = -1;
        str++;
        size++;
    }
    while (*str) {
        if (*str < '0' || *str > '9') {
            *i = 0;
            return -1;
        }
        aux = (aux << 3)+(aux << 1)+(*str - '0');
        str++;
        size++;
    }
    *i = (*i) * aux;
    return size;
}

int fork() {
    int res;
    __asm__ __volatile__(
            "movl $2,%%eax\n\t"
            "int $0x80\n\t"
            "movl %%eax,%0"
            : "=g" (res)
            );
    if (res >= 0)return res;
    else {
        errno = res * -1;
        return -1;
    }
}

int getStats(unsigned int pid, struct stats *st) {
    int res;
    __asm__ __volatile__(
            "movl %1,%%ebx\n\t"
            "movl %2,%%ecx\n\t"
            "movl $35,%%eax\n\t"
            "int $0x80\n\t"
            "movl %%eax,%0"
            : "=g" (res)
            : "g" (pid), "g"(st)
            : "ax", "bx", "cx");
    if (res >= 0)return res;
    else {
        errno = res * -1;
        return -1;
    }
}

int setpriority(unsigned int pid, unsigned int priority) {
    int res;
    __asm__ __volatile__(
            "movl %1,%%ebx\n\t"
            "movl %2,%%ecx\n\t"
            "movl $40,%%eax\n\t"//es el 97 en linux pero como que no llegamos XD
            "int $0x80\n\t"
            "movl %%eax,%0"
            : "=g" (res)
            : "g" (pid), "g"(priority)
            : "ax", "bx", "cx");
    if (res >= 0)return res;
    else {
        errno = res * -1;
        return -1;
    }
}

void exit(void) {
    __asm__ __volatile__(
            "movl $1, %eax\n\t"
            "int $0x80");
}

void provoca_PageFault() {
    __asm__ __volatile__("int $0xE");
}