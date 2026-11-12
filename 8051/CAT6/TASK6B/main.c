/* Interrupt-driven 8051 UART echo. 11.0592 MHz, 9600-8-N-1.
 * New implementation replacing the reference polling-only example.
 * TX=P3.1, RX=P3.0. RX overflow drops newest bytes; inspect rx_dropped.
 */
#include <reg52.h>
volatile unsigned char rx_head=0, rx_tail=0, rx_dropped=0;
volatile unsigned char rx_buffer[32];
volatile bit tx_ready=1;
void serial_isr(void) interrupt 4 {
 unsigned char next, value;
 if(RI) {
  value=SBUF; RI=0; next=(rx_head+1)&31;
  if(next!=rx_tail) { rx_buffer[rx_head]=value; rx_head=next; }
  else if(rx_dropped!=255) ++rx_dropped;
 }
 if(TI) { TI=0; tx_ready=1; }
}
void uart_tx(unsigned char value) {
 while(!tx_ready) {}
 tx_ready=0; SBUF=value;
}
void main(void) {
 unsigned char value;
 PCON &= 0x7F; TMOD=(TMOD&0x0F)|0x20;
 TH1=TL1=0xFD; SCON=0x50; RI=TI=0; TR1=1; ES=EA=1;
 for(;;) {
  if(rx_head!=rx_tail) {
   value=rx_buffer[rx_tail]; rx_tail=(rx_tail+1)&31; uart_tx(value);
  }
 }
}
