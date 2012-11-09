#include <string.h>

String init_string(char *b) {
    String res;
    res.buff = b;
    res.pt = 0;
    res.size = strlen(b);
    return res;
}

char getCharAt(String s, unsigned int pos) {
    return s.buff[pos];
}

unsigned int getSize(String s) {
    return s.size;
}

void putAt(String s, unsigned int pos, char c) {
    s.buff[pos] = c;
}

int strlen(char *buff) {
    int res = 0;
    while (buff[res] != 0) res++;
    return res;
}
