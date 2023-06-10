#include "nu32dip.h" // constants, functions for startup and UART
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "ws2812b.h"


void blink(int, int); // blink the LEDs function
void delay(int);

int main(void) {
    NU32DIP_Startup();
    ws2812b_setup();
    char debug[100];
//        sprintf(debug, "newline\r\n");    
//        NU32DIP_WriteUART1(debug);
    
    wsColor LEDarray[4];
    float hues[4] = {0, 45, 90, 135};
    
    while (1) {
        for (int ii = 0; ii < 4; ii++) {
            if (hues[ii] >= 360) {
                hues[ii] = 0;
            }
            LEDarray[ii] = HSBtoRGB(hues[ii], 1.0, 0.5);
            hues[ii] += 1;
        }

        ws2812b_setColor(LEDarray, 4);
        delay(3);
    }
}

// blink the LEDs
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

void delay(int timems) {
    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT() < (timems * 24000));
}