#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define 	SDI	0
#define		RCLK    1
#define		SRCLK	2

unsigned long long int LED[61] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,
0x8000,0x10000,0x20000,0x40000,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000,
0x100000000,0x200000000,0x400000000,0x800000000,0x1000000000,0x2000000000,0x4000000000,0x8000000000,0x10000000000,0x20000000000,0x40000000000,0x80000000000,
0x100000000000,0x200000000000,0x400000000000,0x800000000000,0x1000000000000,0x2000000000000,0x4000000000000,0x8000000000000,
0x10000000000000,0x20000000000000,0x40000000000000,0x80000000000000,0x100000000000000,0x200000000000000,0x400000000000000,0x800000000000000,0x1000000000000000};

unsigned long long int LED32[32] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000,0x10000,0x20000,0x40000,0x80000,
0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000};

void pulse(int pin)
{
	digitalWrite(pin, 1);
	delay(0.001);
	digitalWrite(pin, 0);
}

void SIPO(unsigned long long int byte)
{
	int i;

	for(i=0;i<61;i++){
		//digitalWrite(SDI, ((byte & (0x800000000000000 >> i)) > 0));
		//digitalWrite(SDI, 0x1000000000000000 & (byte << i));
		//digitalWrite(SDI, 0x80000000 & (byte << i));
		digitalWrite(SDI, ((byte & (0x1000000000000000 >> i)) > 0));
		pulse(SRCLK);
	}
}

void init(void)
{
	pinMode(SDI, OUTPUT);
	pinMode(RCLK, OUTPUT);
	pinMode(SRCLK, OUTPUT);

	digitalWrite(SDI, 0);
	digitalWrite(RCLK, 0);
	digitalWrite(SRCLK, 0);
}

int main(void)
{
	int i;

	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}

	init();

	while(1){
		for(i=0;i<61;i++){
			SIPO(LED[i]);
			pulse(RCLK);
			delay(100);
		}
		//delay(500);
/*
		for(i=0;i<3;i++){
			SIPO(0xff);
			pulse(RCLK);
			delay(100);
			SIPO(0x00);
			pulse(RCLK);
			delay(100);
		}
		delay(500);
*/
	}

	return 0;
}


