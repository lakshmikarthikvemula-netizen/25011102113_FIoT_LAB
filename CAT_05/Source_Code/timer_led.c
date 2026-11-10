#include<reg51.h>

sbit LED = P1^3;
int count = 0;

void main(){

	TMOD = 0x01;
	TH0 = 0xEA;
	TL0 = 0x66;
	TR0 = 1; //initialize timer
		
	while(1){
			
		if(TF0 == 1){
			TH0 = 0xEA;
			TL0 = 0x66;
			TF0 = 0;
			count++;
			if(count>=133){
				LED=!LED;
				count=0;
			}
		}
	}
}