#include "nu32dip.h"
#include "spi.h"
#include <stdio.h>
#include <math.h>

void blink(int, int); // blink the LEDs function
void calc_sine(void);
void calc_tri(void);

unsigned int sine_graph[100];
unsigned int tri_graph[100];


int main(void) {
    NU32DIP_Startup();
    initSPI();
    unsigned int y;    
    unsigned short message1, message2;
    char debug[100];
    
    calc_sine();
    calc_tri();
    
    while(1) {
        for (int ii = 0; ii < 100; ii++) {
            sprintf(debug, "%d\r\n", sine_graph[ii]);    
            NU32DIP_WriteUART1(debug);
            
            message1 = (0b0111 << 12 | sine_graph[ii] << 2);
            message2 = (0b1111 << 12 | tri_graph[ii] << 2);
//            unsigned short test1  = 0b01111111;
//            unsigned short test2 = 0b11111100;
            LATBbits.LATB10 = 0;
            spi_io(message1 >> 8);
            spi_io(message1);
//            spi_io(test1);
//            spi_io(test2);
            LATBbits.LATB10 = 1;
//                    
            LATBbits.LATB10 = 0;
            spi_io(message2 >> 8);
            spi_io(message2);
            LATBbits.LATB10 = 1;    
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < 240000);
        }
        
    
    
    
    }
    
    
}



void blink(int iterations, int time_ms){
	int i;
	unsigned int t;
	for (i=0; i< iterations; i++){
		NU32DIP_GREEN = 0; // on
		NU32DIP_YELLOW = 1; // off
		t = _CP0_GET_COUNT(); // should really check for overflow here
		// the core timer ticks at half the SYSCLK, so 24000000 times per second
		// so each millisecond is 24000 ticks
		// wait half in each delay
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
		
		NU32DIP_GREEN = 1; // off
		NU32DIP_YELLOW = 0; // on
		t = _CP0_GET_COUNT(); // should really check for overflow here
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
	}
}

void calc_sine(void) {
    for (int ii = 0; ii < 100; ii++) {
        sine_graph[ii] = (unsigned  int) ((float)1023/2) + ((float)(1023/2) * sin(4 * (M_PI/100) * ii));
    }
}

void calc_tri(void) {
    for (int ii = 0; ii < 100; ii++) {
        if (ii < 50) {
            tri_graph[ii] = 2 * 1023 * ((double) ii / 100);
        } else {
            tri_graph[ii] = 2046 - (2046 * ((double) ii / 100));
        }
    }
}