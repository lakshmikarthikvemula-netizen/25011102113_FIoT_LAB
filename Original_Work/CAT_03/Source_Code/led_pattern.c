#include <stdio.h>
#include <reg51.h>

sbit in = P2^7;
sbit out = P2^3;

sbit led1 = P1^0;
sbit led2 = P1^1;
sbit led3 = P1^2;
sbit led4 = P1^3;

int mode = 0;
int counter = 0;

void delay()
{
    int i, j;
    for(i = 0; i < 120; i++)
        for(j = 0; j < 120; j++);
}

void mode1()
{
    led1 = 1; led2 = 0; led3 = 0; led4 = 0;
    delay();
    led1 = 0; led2 = 1; led3 = 0; led4 = 0;
    delay();
    led1 = 0; led2 = 0; led3 = 1; led4 = 0;
    delay();
    led1 = 0; led2 = 0; led3 = 0; led4 = 1;
    delay();
}

void mode2()
{
    if(counter == 0)      { led1=0; led2=0; led3=0; led4=0; }
    else if(counter == 1) { led1=1; led2=0; led3=0; led4=0; }
    else if(counter == 2) { led1=0; led2=1; led3=0; led4=0; }
    else if(counter == 3) { led1=1; led2=1; led3=0; led4=0; }
    else if(counter == 4) { led1=0; led2=0; led3=1; led4=0; }
    else if(counter == 5) { led1=1; led2=0; led3=1; led4=0; }
    else if(counter == 6) { led1=0; led2=1; led3=1; led4=0; }
    else if(counter == 7) { led1=1; led2=1; led3=1; led4=0; }
    else if(counter == 8) { led1=0; led2=0; led3=0; led4=1; }
    else if(counter == 9) { led1=1; led2=0; led3=0; led4=1; }
    else if(counter == 10){ led1=0; led2=1; led3=0; led4=1; }
    else if(counter == 11){ led1=1; led2=1; led3=0; led4=1; }
    else if(counter == 12){ led1=0; led2=0; led3=1; led4=1; }
    else if(counter == 13){ led1=1; led2=0; led3=1; led4=1; }
    else if(counter == 14){ led1=0; led2=1; led3=1; led4=1; }
    else if(counter == 15){ led1=1; led2=1; led3=1; led4=1; }

    delay();

    counter = counter + 1;
    if(counter > 15)
    {
        counter = 0;
    }
}

void mode3()
{
    led1 = 1; led2 = 0; led3 = 1; led4 = 0;
    delay();
    led1 = 0; led2 = 1; led3 = 0; led4 = 1;
    delay();
}

void mode4()
{
    led1 = 1; led2 = 1; led3 = 1; led4 = 1;
    delay();
    led1 = 0; led2 = 0; led3 = 0; led4 = 0;
    delay();
}

void check()
{
    if(in == 0)
    {
        delay();
        if(in == 0)
        {
            mode = (mode + 1) % 5;
            counter = 0;
            while(in == 0) {}
            delay();
        }
    }
}

void run()
{
    if(mode == 0)
    {
        led1 = 0; led2 = 0; led3 = 0; led4 = 0;
    }
    else if(mode == 1)
    {
        mode1();
    }
    else if(mode == 2)
    {
        mode2();
    }
    else if(mode == 3)
    {
        mode3();
    }
    else if(mode == 4)
    {
        mode4();
    }
}

void main()
{
    out = 0;
    mode = 0;
    counter = 0;

    while(1)
    {
        check();
        run();
    }
}