#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define TRIG 5
#define dt .01
#define MIN_ERROR 10
#define MIN -3
#define MAX 3
#define Kp .1
#define Kd .01
#define Ki .005
#define ZERO 153

void setup() {
	wiringPiSetup();
}

int getCM() {
  	pinMode(TRIG, OUTPUT);
  	digitalWrite(TRIG, LOW);
  	delay(30);
  	digitalWrite(TRIG, HIGH);
  	delay(50);
  	digitalWrite(TRIG, LOW);
	pinMode(TRIG, INPUT);

        //Wait for echo start
        while(digitalRead(TRIG) == LOW);

        //Wait for echo end
        long startTime = micros();
        while(digitalRead(TRIG) == HIGH);
        long travelTime = micros() - startTime;

        //Get distance in cm
        int distance = travelTime / 58;

        return distance;
}

void setSpeed(FILE *fp, int left, int right)
{
	fprintf(fp, "0=%i\n", left); //Servo#0, Counter Clockwise
	fprintf(fp, "1=%i\n", right); //Clockwise
	fflush(fp);
}

void doPID(FILE *fp)
{
	static int leftSpeed = ZERO;
	static int rightSpeed = ZERO;
	static float last_error = 0;
	static float integral = 0;
	int distance = getCM();
	while(distance > 80)
	{
		setSpeed(fp, 0, 0);
		printf("too far %d\n", distance);
		distance = getCM();
	}
	static int goodDistance = distance;
	float error = distance - goodDistance;
	float deriv = (error - last_error)/dt;
	if(abs(error) > MIN_ERROR)
	{
		integral = integral + error * dt;
	}
	float output = Kp * error + Ki * integral + Kd * deriv;
	if(output < MIN)
	{
		output = MIN;
	}
	else if (output > MAX)
	{
		output = MAX;
	}
	printf("Current output %f\n", output);
	//negative means go faster right
	//positive means go faster left
	leftSpeed = 190 + (55 * output/3);
	rightSpeed = ZERO - (190 - ZERO) + (55 * output/3);
	printf("left=%i right=%i", leftSpeed, rightSpeed);
	setSpeed(fp, leftSpeed, rightSpeed);
	last_error = error;
}

int main(void) {
        setup();

	FILE *fp;
	fp = fopen("/dev/servoblaster", "w");
	if (fp == NULL) {
		printf("Error opening file\n");
		exit(0);
	}
	
	while(1)
	{
		doPID(fp);
		delay(dt * 1000);
	}
        
	return 0;
}