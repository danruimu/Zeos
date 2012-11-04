#include <time.h>

unsigned long int ticks = 0;
int idle = 0;

unsigned long int getTicks() {
	return ticks;
}

void incTicks() {
	ticks++;
}