#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define TRIG 5
#define dt .01
#define MIN_ERROR 15
#define MIN -3
#define MAX 3
#define Kp .1
#define Kd .01
#define Ki .005
#define ZERO 153
#define  RADIUS 150 //need to set this

void setup() {
	wiringPiSetup();
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

//0 is right, 1 middle right, 2 is forward, 3 middle left, 4 left
//forward is boolean
int getCMGo(FILE *fp, int *direction, int *forward)
{
	//move to left
	printf("start\n");
	moveCM(fp, 50);
	delay(550);//maybe
	moveCM(fp, 0);
	int reading[5];
	printf("read start\n");
	reading[0] = getCMGood();
	//printf("position 0 and distance %i\n", reading[0]);
	//delay(1000);
	for(int i = 1; i < 5; i++)
	{
		moveCM(fp, 250);
		delay(45);
		moveCM(fp, 0);
		reading[i] = getCMGood();
		//printf("position %i and distance %i\n", i, reading[i]);
		//delay(1000);
	}
	int best = reading[0];
	*direction = 0;
	for(int i = 1; i < 5; i++)
	{
		if(reading[i] >  RADIUS + MIN_ERROR && reading[i] > best && best >  RADIUS + MIN_ERROR)
		{
			best = reading[i];
			*direction = i;
		}
		else if(reading[i] <=  RADIUS - MIN_ERROR && reading[i] < best)
		{
			best = reading[i];
			*direction = i;
		}
	}
	if(best >  RADIUS + MIN_ERROR)
	{
		*forward = 1;
	}
	return best;
}

void setSpeed(FILE *fp, int left, int right)
{
	fprintf(fp, "0=%i\n", left); //Servo#0
	fprintf(fp, "1=%i\n", right);
	fflush(fp);
}
void setSpeed(FILE *fp, int left, int right, int forward)
{
	if(forward)
	{
		setSpeed(fp, left, right);
	}
	else
	{
		setSpeed(fp, right, left);
	}
}

//what we want to do is either move in direction closest to  RADIUS or further than at least  RADIUS
int doPID(FILE *fp)
{
	static int leftSpeed = ZERO;
	static int rightSpeed = ZERO;
	static float last_error = 0;
	static float integral = 0;
	setSpeed(fp, 0, 0);
	int direction = 0;
	int forward = 0;
	int distance = getCMGo(fp, &direction, &forward);
	if(direction != 2)
	{
		if(direction < 2)
		{
			setSpeed(fp, ZERO + 40, ZERO + 40);
			delay(250);
			setSpeed(fp, 0, 0);
		}
		else
		{
			setSpeed(fp, ZERO - 40, ZERO - 40);
			delay(250);
			setSpeed(fp, 0, 0);
		}
		distance = getCMGo(fp, &direction, &forward);
	}
	printf("sonar reading %i\n", distance);
	if(distance > RADIUS - 2 * MIN_ERROR && distance < RADIUS + 2 * MIN_ERROR)
	{
		return 0;
	}
	float error = abs(distance -  RADIUS);
	float deriv = (error - last_error)/dt;
	integral = integral + error * dt;
	if(integral > .5*MAX)
	{
		integral = .5 * MAX;
	}
	else if (integral < .5*MIN)
	{
		integral = .5* MIN;
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
	leftSpeed = 190 + (30 * output/3);
	rightSpeed = ZERO - (190 - ZERO) - (30 * output/3);
	setSpeed(fp, leftSpeed, rightSpeed, forward);
	last_error = error;
	delay(500);
	return 1;
}

int main(void) {
    setup();
	
	
	FILE *fp;
	fp = fopen("/dev/servoblaster", "w");
	if (fp == NULL) {
		printf("Error opening file\n");
		exit(0);
	}
	/**int forward = 0;
	int direction = 0;
	int distance = getCMGo(fp, &direction, &forward);
	printf("the distance is %i, direction %i, forward %i\n", distance, direction, forward);*/
	
	while(doPID(fp))
	{
		delay(dt * 1000);
	}
        
	return 0;
}