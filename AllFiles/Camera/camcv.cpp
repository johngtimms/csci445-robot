#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <wiringPi.h>

using namespace cv;
using namespace std;

#define TRIG 5
#define dt .01
#define MIN_ERROR 10
#define MIN_pid -3
#define MAX_pid 3
#define Kp .005
#define Kd 0
#define Ki 0
#define ZERO 153

void setSpeed(FILE *fp, int left, int right)
{
	fprintf(fp, "0=%i\n", left); //Servo#0, Counter Clockwise
	fprintf(fp, "1=%i\n", right); //Clockwise
	fflush(fp);
}

void setup() {
	wiringPiSetup();
}

float doPID(FILE *fp, float distance)
{
	static int leftSpeed = ZERO;
	static int rightSpeed = ZERO;
	static float last_error = 0;
	static float integral = 0;
	static float goodDistance = 80;
	float error = distance - goodDistance;
	float deriv = (error - last_error)/dt;
	integral = integral + error * dt;
	if(integral > .5)
	{
		integral = .5;
	}
	else if (integral < -.5)
	{
		integral = -.5;
	}
	float output = .04 * error + .009 * integral + Kd * deriv;
	if(output < MIN_pid)
	{
		output = MIN_pid;
	}
	else if (output > MAX_pid)
	{
		output = MAX_pid;
	}
	//printf("Current output %f\n", output);
	//negative means go faster right
	//positive means go faster left
	float setOut = 0;
	if(abs(output) > 1)
	{
		setOut = output;
	}
	last_error = error;
	return setOut;
}

double doPID2(FILE *fp, double distance, double dis)
{
	static int leftSpeed = ZERO;
	static int rightSpeed = ZERO;
	static float last_error = 0;
	static float integral = 0;
	//static double goodDistance = dis;
	double error = dis - distance;
	double deriv = (error - last_error)/dt;
	integral = integral + error * dt;
	if(integral > .5)
	{
		integral = .5;
	}
	else if (integral < -.5)
	{
		integral = -.5;
	}
	double output = Kp * error + Ki * integral + Kd * deriv;
	if(output < MIN_pid)
	{
		output = MIN_pid;
	}
	else if (output > MAX_pid)
	{
		output = MAX_pid;
	}
	//printf("Current output %f\n", output);
	//negative means go faster right
	//positive means go faster left
	float setOut = 0;
	if(abs(output) > 1)
	{
		setOut = output;
	}
	//printf("left=%i right=%i", leftSpeed, rightSpeed);
	last_error = error;
	return setOut;
}

 int main( int argc, char** argv )
 {
	 setup();

	FILE *fp;
	fp = fopen("/dev/servoblaster", "w");
	if (fp == NULL) {
		printf("Error opening file\n");
		exit(0);
	}
    VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

cap.set(CV_CAP_PROP_FRAME_WIDTH, 160);
cap.set(CV_CAP_PROP_FRAME_HEIGHT, 120);

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  int iLowH = 122;
 int iHighH = 179;

  int iLowS = 73; 
 int iHighS = 255;

  int iLowV = 60;
 int iHighV = 255;

  //Create trackbars in "Control" window
 createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control", &iHighH, 179);

  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

  createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);

  int iLastX = -1; 
 int iLastY = -1;

  //Capture a temporary image from the camera
 Mat imgTmp;
    imgTmp = Mat::zeros(301, 301, CV_8UC3);

 cap.read(imgTmp); 

  //Create a black image with the size as the camera output
 Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video



         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

    Mat imgHSV;

   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  Mat imgThresholded;

   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (removes small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //Calculate the moments of the thresholded image
  Moments oMoments = moments(imgThresholded);

   double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;

   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if (dArea > 10000)
  {
   //calculate the position of the ball
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;        
        
   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
   }

	static double goodArea = dArea;
	
	float horizonal = doPID(fp, posX);
	double vertical = doPID2(fp, dArea, goodArea);
	
	
	if(posX - MIN_ERROR > 60 && posX + MIN_ERROR < 80)
	{
		horizonal = 0;
	}
	if(dArea > goodArea*.7 && dArea < goodArea*1.4)
	{
		vertical = 0;
	}
	float left = ZERO + (15 * horizonal/3) + (15 * vertical/3);
	float right = ZERO + (15 * horizonal/3) - (15 * vertical/3);
	setSpeed(fp, left, right);
    iLastX = posX;
	iLastY = posY;
  }
  else
  {
	  setSpeed(fp, 0, 0);
  }

   imshow("Thresholded Image", imgThresholded); //show the thresholded image

   imgOriginal = imgOriginal + imgLines;
  imshow("Original", imgOriginal); //show the original image

        if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }

	setSpeed(fp, 0, 0);
   return 0;
}
