# 1 "entry.S"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "entry.S"




# 1 "include/asm.h" 1
# 6 "entry.S" 2
# 1 "include/segment.h" 1
# 7 "entry.S" 2
# 72 "entry.S"
.globl clock_handler; .type clock_handler, @function; .align 0; clock_handler:;
pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %edx; pushl %ecx; pushl %ebx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es;
movb $0x20, %al ; outb %al, $0x20 ;;
call clock_irs;
popl %ebx; popl %ecx; popl %edx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs;;
iret;

.globl keyboard_handler; .type keyboard_handler, @function; .align 0; keyboard_handler:;
pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %edx; pushl %ecx; pushl %ebx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es;
movb $0x20, %al ; outb %al, $0x20 ;;
call keyboard_irs;
popl %ebx; popl %ecx; popl %edx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs;;
iret;

.globl pageFault_handler; .type pageFault_handler, @function; .align 0; pageFault_handler:;
pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %edx; pushl %ecx; pushl %ebx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es;
call pageFault_exception;
popl %ebx; popl %ecx; popl %edx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs;;
iret;

.globl ret_from_clone; .type ret_from_clone, @function; .align 0; ret_from_clone:
movl -12(%esp), %ebx;
movl -16(%esp), %ecx;
movl $0, 0x18(%esp);
movl %ebx, 0x2c(%esp);
movl %ecx, 0x38(%esp);
popl %ebx; popl %ecx; popl %edx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs;;
iret;
