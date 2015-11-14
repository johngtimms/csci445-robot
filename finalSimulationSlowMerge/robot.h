#ifndef Robot_Filter_Particle_h
#define Robot_Particle_h
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define TRIG 5
#define dt .01
#define MIN_ERROR 20
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

/**int rotate(bool pos)
{
	static int angle = 0;
	if(pos)
		setSpeed(fp, 180, 180);
	else
		setSpeed(fp, ZERO - 180 - ZERO, ZERO - 180 - ZERO);
	delay(750);
	setSpeed(fp, 0, 0);
	angle = (90 + angle) % 360;
	return angle;
}

void turnBotABit(bool pos)
{
	if(pos)
		setSpeed(fp, 180, 180);
	else
		setSpeed(fp, ZERO - 180 - ZERO, ZERO - 180 - ZERO);
	delay(50);
	setSpeed(fp, 0, 0);
}

//from 0 to 270 on 90 degree marks
void rotateToAngle(int angleToRotateTo)
{
	int angleCurr = rotate(true);
	while(angleCurr != angleToRotateTo)
		angleCurr = rotate(true);
}

//move one square forward
void forward()
{
	setSpeed(fp, 170, ZERO - 170 - ZERO);
	delay(3000);
	//possibly add in some error correction here like looking around at the walls
	setSpeed(fp, 0, 0);
}*/
// robot movement should be handled with maintaining roughly the same left to right distances throughout the
// run
// to accomplish this, look left and right first
// remember those distances
// movement forward should be within ~30 cm from wall
// turns need to move forward to get that distance to around 20 to account for turning radius
// accomplish movements in steps, slowly compensating for wrong distances
int checkSonar(int dir)
{
	static int prevDir = -1;
	if(prevDir == dir)
	{
		return getCMGood();
	}
	prevDir = dir;
	switch(dir)
	{
		case 0:
			//left
			moveCM(fp, 250);
			delay(750);//maybe
			moveCM(fp, 0);
			return getCMGood();
		case 1:
			moveCM(fp, 50);
			delay(750);//maybe
			moveCM(fp, 250);
			delay(155);
			moveCM(fp, 0);
			return getCMGood();
		case 2:
			moveCM(fp, 50);
			delay(750);//maybe
			moveCM(fp, 0);
			return getCMGood();
	}
	return -1;//error
}

void turn(bool left)
{
	int forward = checkSonar(1);
	do
	{
		setSpeed(fp, 170, ZERO - 170 - ZERO);
		delay(100);
		setSpeed(fp, 0, 0);
	}
	while(forward - getCMGood() > 10);
	if(left)
	{
		setSpeed(fp, 180, 180);
	}
	else
	{
		setSpeed(fp, ZERO - 180 - ZERO, ZERO - 180 - ZERO);
	}
	delay(750);
	setSpeed(fp, 0, 0);
}
/**
* dir - 0 down
* dir - 1 right
* dir - 2 left
* dir - 3 up
*/
void move(int dir, bool turnOnly)
{
	static int currDir = 3;
	static int left = checkSonar(0);
	static int right = checkSonar(2);
	//start of regular logic
	if(currDir != dir)
	{
		//turn logic to get in right direction
		if((dir == 0 && currDir == 3) || (currDir == 0 && dir == 3) || (currDir = 1 && dir == 2) || (currDir == 2 && dir == 1))
		{
			//need to rotate 180
			turn(true);
			turn(true);
		}
		else if(dir == 1)
		{
			//need to go to right?
			if(currDir == 0)
				turn(false);
			else
				turn(true);
		}
		else if(dir == 2)
		{
			if(currDir == 3)
				turn(false);
			else
				turn(true);
		}
		else if(dir == 0)
		{
			if(currDir == 2)
				turn(false);
			else
				turn(true);
		}
		else if(dir == 3)
		{
			if(currDir == 1)
				turn(false);
			else
				turn(true);
		}
	}
	if(turnOnly)
		return;
	currDir = dir;
	//move forward now
	int totalTime = 3000;
	for(int i = 0; i < totalTime; i = i + totalTime/10)
	{
		int newLeft = checkSonar(0);
		if(newLeft - left > 5)
		{
			//more than 5 away, angle a bit left
			setSpeed(fp, 180, 180);//probably left
			delay(50);
		}
		else if(newLeft - left < -5)
		{
			setSpeed(fp, ZERO - 180 - ZERO, ZERO - 180 - ZERO);//probably right
			delay(50);
		}
		setSpeed(fp, 0, 0);
		setSpeed(fp, 170, ZERO - 170 - ZERO);
		delay(totalTime/10);
		setSpeed(fp, 0, 0);
	}
}

void move(int dir)
{
	move(dir, false);
}

/**void moveABit(bool forw)
{
	if(forw)
		setSpeed(fp, 170, ZERO - 170 - ZERO);
	else
		setSpeed(fp, ZERO - 170 - ZERO, 170);
	delay(100);
	//possibly add in some error correction here like looking around at the walls
	setSpeed(fp, 0, 0);
}

int straighten(bool hasLeft, bool hasRight, bool hasStriaght, int blocks, int optDist)
{
	//is wall in front
	if (hasStriaght)
	{
		
		//move to left
		moveCM(fp, 50);
		delay(750);//maybe
		moveCM(fp, 250);
		delay(155);
		moveCM(fp, 0);
		while(true)
		{
			int dist = getCMGood();
			if(dist < optDist - 2 + (60 * blocks))
				moveABit(false);
			else if(dist > optDist + 2 + (60 * blocks))
				moveABit(true);
			else
				break;
		}
	}
	//is wall on left
	else if(hasLeft)
	{
		//move to left
		moveCM(fp, 250);
		delay(750);//maybe
		moveCM(fp, 0);
		while(true)
		{
			int dist = getCMGood();
			if(dist < optDist - 2 + (60 * blocks))
				turnBotABit(true);
			else if(dist > optDist + 2 + (60 * blocks))
				turnBotABit(false);
			else
				break;
		}
	}
	//is wall on right
	else if(hasRight)
	{
		//move to left
		moveCM(fp, 50);
		delay(750);//maybe
		moveCM(fp, 0);
		while(true)
		{
			int dist = getCMGood();
			if(dist < optDist - 2 + (60 * blocks))
				turnBotABit(false);
			else if(dist > optDist + 2 + (60 * blocks))
				turnBotABit(true);
			else
				break;
		}
	}
	return getCMGood();
	//can't do anything otherwise
}

int straighten(bool hasLeft, bool hasRight, bool hasStriaght, int optDist)
{
	return straighten(hasLeft, hasRight, hasStriaght, 0, optDist);
}*/

#endif