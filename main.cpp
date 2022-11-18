#include "mbed.h"
DigitalOut out1(D7),out2(D6),
out3(D5),out4(D4);
PwmOut ena(D9),enb(D3);
Ticker movement;
char x,gx,y,gy;
int i =0;
void counter_clock();
void normal_clock();
void Move();
enum Clock {twelve,six,nine,three};
Clock face;
int main() {
    out1=out2=out3=out4=0;
    ena=enb=.1;
    ena.period(.5);
    enb.period(.5);
    gx=2;
    gy=2;
    x=0;
    y=0;
    face = twelve;
    movement.attach(&Move, 1000ms);
    while (1) {
    }
}

void Move(){
    if (gx != x || gy != y) {
    if(face == twelve){
    if (y < gy){out1=out3=1;out2=out4=0;y++;}
    else if (y>gy){out2=out4=1;out1=out3=0;y--;}
    else if (x < gx)movement.attach(&normal_clock,666ms);
    else if (x > gx)movement.attach(&counter_clock,666ms);}

    else if(face == six){
    if (y < gy){out2=out4=1;out1=out3=0;y++;}
    else if (y>gy){out1=out3=1;out2=out4=0;y--;}
    else if (x < gx)movement.attach(&counter_clock,666ms);
    else if (x > gx)movement.attach(&normal_clock,666ms);}

    else if(face==three){
    if (x < gx){out1=out3=1;out2=out4=0;x++;}
    else if (x>gx){out2=out4=1;out1=out3=0;x--;}
    else if (y < gy)movement.attach(&counter_clock,666ms);
    else if (y > gy)movement.attach(&normal_clock,666ms);}
   
    else if(face==nine){
    if (x < gx){out2=out4=1;out1=out3=0;x++;}
    else if (x>gx){out1=out3=1;out2=out4=0;x--;}
    else if (y < gy)movement.attach(&normal_clock,666ms);
    else if (y > gy)movement.attach(&counter_clock,666ms);}
    }
    else if(gx == x && gy == y){
    out1=out2=out3=out4=0;
     movement.detach();}
}
void counter_clock(){
    out1=out4=1;
    out2=out3=0;    
    if (i==0){
        switch (face) {
        case twelve: face = nine;break;
        case six: face = three;break;
        case nine: face = six;break;
        case three: face = twelve;break;
        }
        i++;
    }
    else{
        i = 0;
        movement.attach(&Move,1000ms);
    }
}
void normal_clock(){
    out2=out3=1;
    out1=out4=0;
    if (i==0){
        switch (face) {
        case twelve: face = three;break;
        case six: face = nine;break;
        case nine: face = twelve;break;
        case three: face = six;break;
        }
        i++;
    }
    else {
        i = 0;
        movement.attach(&Move,1000ms);
    }   
}