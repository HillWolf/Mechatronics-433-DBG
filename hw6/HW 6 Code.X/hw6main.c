#include "nu32dip.h" // constants, functions for startup and UART
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "i2c_master_noint.h"


void blink(int, int); // blink the LEDs function
void gp7on(void);
void gp7off(void);
int gp0read(void);
void delay(int);

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    i2c_master_setup();
    i2c_master_start();
    unsigned char chip_address = 0b01000000;
    i2c_master_send(chip_address);
    unsigned char chip_setup = 0x00;
    i2c_master_send(chip_setup);
    unsigned char pin_setup = 0b01111111;
    i2c_master_send(pin_setup);
    i2c_master_stop;
    
    char debug[100];
    
    //init the chip, GP0 is input, gp7 is output
    
  while (1) {
      sprintf(debug, "%d\r\n", gp0read());    
      NU32DIP_WriteUART1(debug);

      if (gp0read()) {
          gp7on();
      } else {
          gp7off();
      }
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
		
void gp7on(void) {
    i2c_master_start();
    unsigned char chip_address = 0b01000000;
    i2c_master_send(chip_address);
    unsigned char reg_address = 0x0A;
    i2c_master_send(reg_address);
    unsigned char reg_status = 0b10000000;
    i2c_master_send(reg_status);
    i2c_master_stop();
}

void gp7off(void) {
    i2c_master_start();
    unsigned char chip_address = 0b01000000;
    i2c_master_send(chip_address);
    unsigned char reg_address = 0x0A;
    i2c_master_send(reg_address);
    unsigned char reg_status = 0b00000000;
    i2c_master_send(reg_status);
    i2c_master_stop();    
}

int gp0read(void) {    
    i2c_master_start();
    //send address with write bit
    //send register you want to read from.
    //restart
    //send address with read bit
        //unsigned char r = recv
    //send the ack bit
    //send the stop bit
    //return (r&0b00000001);
    unsigned char chip_address = 0b01000000;
    i2c_master_send(chip_address);
    unsigned char reg_address = 0x09;
    i2c_master_send(reg_address);
    i2c_master_restart();
    unsigned char read_address = 0b01000001;
    i2c_master_send(read_address);
    unsigned char message = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    int result = message & 0b00000001;
    return result;
}