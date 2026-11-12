/* AT89S52 Timer2 hardware clock-out, P1.0/T2, 50% duty.
 * Fout=11059200/(4*(65536-RCAP2)); nominal rates 100,500,1000,2000,5000Hz.
 * P3.2 cycles rates on a press (active-low). Actual crystal/edges unverified.
 */
#include <reg52.h>
sfr LAB_T2MOD=0xC9;
sbit RATE_BUTTON=P3^2;
const unsigned int reload[5]={37888,60006,62771,64154,64983};
void set_rate(unsigned char index) {
 TR2=0; RCAP2H=TH2=reload[index]>>8; RCAP2L=TL2=reload[index]&255;
 T2CON=0; LAB_T2MOD=0x02; TR2=1;
}
void main(void) {
 unsigned char index=0; unsigned int stable=0; bit latched=0;
 RATE_BUTTON=1; set_rate(index);
 for(;;) {
  if(!RATE_BUTTON) {
   if(stable<30000) ++stable;
   else if(!latched) { latched=1; index=(index+1)%5; set_rate(index); }
  } else {stable=0;latched=0;}
 }
}
