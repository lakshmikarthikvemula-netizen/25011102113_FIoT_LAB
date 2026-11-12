/* Timer0 software PWM, nominal 500Hz, 10% duty increments.
 * 11.0592MHz classic 12-clock 8051. P1.1 active-low LED.
 * Manual reload/ISR latency changes timing; measure with a DSO.
 */
#include <reg52.h>
sbit LED_PWM=P1^1;
volatile unsigned char phase=0, duty=0, rising=1;
volatile unsigned int periods=0;
void timer0_isr(void) interrupt 1 {
 TH0=0xFF; TL0=0x48; /* 184 counts, approximately 200us */
 LED_PWM=(phase<duty)?0:1;
 if(++phase==10) {
  phase=0;
  if(++periods==250) {
   periods=0;
   if(rising) { if(++duty==10) rising=0; }
   else if(--duty==0) rising=1;
  }
 }
}
void main(void) {
 LED_PWM=1; TMOD=(TMOD&0xF0)|1; TH0=0xFF; TL0=0x48;
 ET0=EA=TR0=1; for(;;) {}
}
