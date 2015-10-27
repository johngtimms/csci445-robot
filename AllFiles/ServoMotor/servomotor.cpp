#include <stdio.h>
#include <stdlib.h>
#include "wiringPi.h"
int main(void) {
 FILE *fp;
 fp = fopen("/dev/servoblaster", "w");
 if (fp == NULL) {
   printf("Error opening file\n");
   exit(0);
 }
 //forward
 fprintf(fp, "0=90\n"); //Servo#0, Counter Clockwise
 fprintf(fp, "1=192\n"); //Clockwise
 fflush(fp);
 delay(1000);
 //backwards
 fprintf(fp, "0=182\n"); //Servo#0, Counter Clockwise
 fprintf(fp, "1=100\n"); //Clockwise
 fflush(fp);
 delay(1000);
 //turn right
 fprintf(fp, "0=182\n"); //Servo#0, Counter Clockwise
 fprintf(fp, "1=182\n"); //Clockwise
 fflush(fp);
 delay(1000);
 //turn left
 fprintf(fp, "0=90\n"); //Servo#0, Counter Clockwise
 fprintf(fp, "1=90\n"); //Clockwise
 fflush(fp);
 delay(1000);
 fprintf(fp, "0=0\n"); //Stop
 fprintf(fp, "1=0\n"); //Stop
 fflush(fp);
 delay(1000);
 fclose(fp);
 return 0;
}
