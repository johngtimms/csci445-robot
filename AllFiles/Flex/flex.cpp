#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define flexpin 5

void setup() {
	wiringPiSetup();
}

int getTime() {
	int charging_time = 0;

  	pinMode(flexpin, OUTPUT);
  	digitalWrite(flexpin, LOW);
  	delay(1000);
	pinMode(flexpin, INPUT);

        while(digitalRead(flexpin) == LOW)
		charging_time++;

        return charging_time;
}

int main(void) {
        setup();

	while (1) {
        	printf("Charging Time: %d\n", getTime());
		delay(1000);
	}
        
	return 0;
}

