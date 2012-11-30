#ifndef STRING_H
#define	STRING_H

typedef struct str
{
    char *buff;
    unsigned int pt;
    unsigned int size;
}String;

String init_string(char *b);
char getCharAt(String s,unsigned int pos);
unsigned int getSize(String s);
void putAt(String s,unsigned int pos, char c);




#endif	/* STRING_H */

