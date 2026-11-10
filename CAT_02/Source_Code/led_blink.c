#include <reg51.h>
#include <stdio.h>

sbit LED = P1^0;

void delay(void) {
    unsigned int i, j;
    for (i = 0; i < 500; i++) {
        for (j = 0; j < 120; j++) {
        }
    }
}

void main(void) {
    while (1) {
        LED = 0;
        delay();
        
        LED = 1;
        delay();
    }
}