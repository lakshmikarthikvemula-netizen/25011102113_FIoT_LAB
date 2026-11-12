/* Timer-driven two-coil full-step motor controller, ULN2003 P2.0-P2.3.
 * P3.2 chooses direction; P3.3 selects 10ms/100ms step interval (active-low).
 * 11.0592MHz; approximately 1ms timer tick. Motor requires its own supply.
 */
#include <reg52.h>
sbit DIR=P3^2;
sbit FAST=P3^3;
const unsigned char sequence[4]={9,12,6,3};
volatile unsigned char tick=0,index=0;
void timer0_isr(void) interrupt 1 {
 TH0=0xFC; TL0=0x66;
 if(++tick>=(FAST?100:10)) {
  tick=0;index=(index+(DIR?1:3))&3;
  P2=(P2&0xF0)|sequence[index];
 }
}
void main(void) {
 DIR=FAST=1; P2 &= 0xF0; TMOD=(TMOD&0xF0)|1;
 TH0=0xFC;TL0=0x66; ET0=EA=TR0=1; for(;;) {}
}
