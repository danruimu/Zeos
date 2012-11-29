#include <keyboard.h>
#include <types.h>
#include <io.h>
#include <libc.h>

char char_map[] = {
    '\0', '\0', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '\'', '¡', '\0', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '`', '+', '\n', '\0', 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ñ',
    '\0', 'º', '\0', 'ç', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '-', '\0', '*',
    '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '\0', '\0', '\0', '<', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0'
};

char llegirImprimirTecla() {
    Byte b = inb(PORT_IN_KEY);
    unsigned int c = (unsigned int) b & 0x7F;
    if (b == ESC) {
        cls();
    }
    else if (b & 0x80) {
        if (c < (sizeof(char_map)/sizeof(char)) && char_map[c] != '\0') printc(char_map[c]);
        else {
            char *buff = "     ";
            itoa(c,buff);
            printk_xy(0, 0, buff);
        }
    }
    return char_map[c];
}
