#include "nu32dip.h" // constants, functions for startup and UART
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "ssd1306.h"
#include "mpu6050.h"
#include "i2c_master_noint.h"
#include "font.h"


void blink(int, int); // blink the LEDs function
void delay(int);
void drawchar(unsigned char, unsigned char, unsigned char);

int main(void) {
    NU32DIP_Startup();
    i2c_master_setup();
    init_mpu6050();
    ssd1306_setup();
	
	// char array for the raw data
    unsigned char d[14];
	// floats to store the data
	float ax, ay, az, gx, gy, gz, t;

    char m[100];
    ssd1306_clear();

    unsigned int t1, t2;
    double fps;
    
  while (1) {
      t1 = _CP0_GET_COUNT();
      
      ssd1306_clear();
      burst_read_mpu6050(d);
		// convert data
//        ax = conv_xXL(d);
//        ay = conv_yXL(d);
        az = conv_zXL(d);
//        gx = conv_xG(d);
//        gy = conv_yG(d);
//        gz = conv_zG(d);
//        t = conv_temp(d);
        
        sprintf(m, "%f", az);
        drawstr(m, 0, 12);
        ssd1306_update();
        
        t2 = _CP0_GET_COUNT();
        fps =  1.0 / ((double) (t2-t1) / 24000000.0);
        sprintf(m, "%f", fps);
        drawstr(m, 60, 12);
        ssd1306_update();
        
        
      
  }
}


void drawchar(unsigned char letter, unsigned char x, unsigned char y) {
    for (int ii = 0; ii < 5; ii++) {
        for (int jj = 0; jj < 8; jj++) {
            unsigned char actual = ASCII[letter-32][ii]; 
            ssd1306_drawPixel(x+ii, y+jj, ((actual >> jj) & 0b1));
        } 
    }  
}

void drawstr(char* m, unsigned char x, unsigned char y) {
    int ii = 0;
    while (m[ii] != '\0') {
        drawchar(m[ii], (x + (6*ii)), y);
        ii++;
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