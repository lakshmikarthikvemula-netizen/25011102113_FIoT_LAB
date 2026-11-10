#include <reg52.h>  

void main(void) {
    char ch;

 
    SCON = 0x50;    
    TMOD = 0x20;    
    TH1  = 0xFD;   
    TR1  = 1;       

    while (1) {
        while (RI == 0);
        ch = SBUF;    
        RI = 0;          

        SBUF = ch;       
        while (TI == 0); 
        TI = 0;
    }
}
