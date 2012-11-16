#include <string.h>

String init_string(char *b) {
    int i = 0;
    while (b[i] != 0) i++;
    String res;
    res.buff = b;
    res.pt = 0;
    res.size = i;
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
