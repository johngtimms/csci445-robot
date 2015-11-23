#ifndef Robot_Filter_Particle_h
#define Robot_Filter_Particle_h
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>

#define TRIG 5
#define dt .01
#define MIN_ERROR 20
#define ZERO 153
#define  RADIUS 150 //need to set this
#define UP 1
#define DOWN 3
#define LEFT 2
#define RIGHT 0
#define DISABLE_MOVE false

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
	return (reading1 + reading2) / 2;
}

void setSpeed(FILE *fp, int left, int right)
{
	fprintf(fp, "0=%i\n", left); //Servo#0
	fprintf(fp, "1=%i\n", right);
	fflush(fp);
}

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
	cout << "turn called\n";
	if(DISABLE_MOVE)
		return;
	if(!left)
	{
		setSpeed(fp, 180, 180);
	}
	else
	{
		setSpeed(fp, ZERO - (180 - ZERO), ZERO - (180 - ZERO));
	}
	delay(700);
	setSpeed(fp, 0, 0);
}

void move(int dir, bool turnOnly)
{
	cout << "move called\n";
	if(DISABLE_MOVE)
		return;
	static int currDir = UP;
	static int left = checkSonar(0);
	static int right = checkSonar(2);
	//start of regular logic
	if(currDir != dir)
	{
		//turn logic to get in right direction
		if(((dir == DOWN) && (currDir == UP) || (currDir == DOWN) && (dir == UP)) ||
			((currDir == LEFT) && (dir == RIGHT)) || ((currDir == RIGHT) && (dir == LEFT)))
		{
			//need to rotate 180
			turn(true);
			turn(true);
		}
		else if(dir == LEFT)
		{
			//need to go to right?
			if(currDir == UP)
				turn(false);
			else
				turn(true);
		}
		else if(dir == RIGHT)
		{
			if(currDir == UP)
				turn(true);
			else
				turn(false);
		}
		else if(dir == DOWN)
		{
			if(currDir == RIGHT)
				turn(true);
			else
				turn(false);
		}
		else if(dir == UP)
		{
			if(currDir == RIGHT)
				turn(false);
			else
				turn(true);
		}
	}
	currDir = dir;
	if(turnOnly)
		return;
	//move forward now
	int totalTime = 2000;
	for(int i = 0; i < totalTime; i = i + totalTime/10)
	{
		int possibleLeft = !goLeft ? 100 : checkSonar(0);
		int newLeft = possibleLeft % 60;
		bool goLeft = true;
		//int totalErrorPos =  + 10 * (possibleLeft/60);
		if(possibleLeft > 60)
		{
			int possibleRight = checkSonar(2);
			if(possibleRight < 60)
			{
				possibleRight = possibleRight % 60;
				goLeft = false;
				if(possibleRight > 32)
				{
					//more than 5 away, angle a bit left
					setSpeed(fp, 180, 180);
					delay(40);
					setSpeed(fp, 0, 0);
				}
				else if(possibleRight < 23)
				{
					//probably right
					setSpeed(fp, ZERO - (180 - ZERO), ZERO - (180 - ZERO));//probably left
					delay(40);
					setSpeed(fp, 0, 0);
				}
			}
		}
		if(goLeft)
			if(newLeft > 32)
			{
				//more than 5 away, angle a bit left
				setSpeed(fp, ZERO - 180 - ZERO, ZERO - 180 - ZERO);//probably left
				delay(40);
				setSpeed(fp, 0, 0);
			}
			else if(newLeft < 23)
			{
				//probably right
				setSpeed(fp, 180, 180);
				delay(40);
				setSpeed(fp, 0, 0);
			}
		
		if((i/(totalTime/10)) > 5)
		{
			int forwardSon = checkSonar(1);
			if(forwardSon < 15)
			{
				setSpeed(fp, 0, 0);
				break;
			}
		}
		setSpeed(fp, 0, 0);
		delay(10);
		setSpeed(fp, 170, ZERO - (170 - ZERO));
		delay(totalTime/10);
		setSpeed(fp, 0, 0);
	}
}

void move(int dir)
{
	move(dir, false);
}
#endif