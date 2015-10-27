#ifndef Robot_Filter_Particle_h
#define Robot_Particle_h
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define TRIG 5
#define dt .01
#define MIN_ERROR 10
#define ZERO 153
#define  RADIUS 150 //need to set this

using namespace std;

FILE *fp;

void setup()
{
	wiringPiSetup();
	fp = fopen("/dev/servoblaster", "w");
	if (fp == NULL) {
		printf("Error opening file\n");
		exit(0);
	}
}

void moveCM(FILE *fp, int speed)
{
	fprintf(fp, "2=%i\n", speed); //Servo#2
	fflush(fp);
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

int getCMGood()
{
	int reading1 = getCM();
	int reading2 = getCM();
	//printf("2 reads done\n");
	if(abs(reading1 - reading2) > MIN_ERROR)
	{
		printf("reading got weird, trying again\n");
		return getCMGood();
	}
	return reading1;
}

void setSpeed(FILE *fp, int left, int right)
{
	fprintf(fp, "0=%i\n", left); //Servo#0
	fprintf(fp, "1=%i\n", right);
	fflush(fp);
}

void rotate()
{
	setSpeed(fp, 180, 180);
	delay(690);
	setSpeed(fp, 0, 0);
}

void forward()
{
	setSpeed(fp, 170, ZERO - (180 - ZERO));
	delay(250);
	setSpeed(fp, 0, 0);
}

#endif