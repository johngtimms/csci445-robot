#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "Util_Map.h"
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <cstdlib>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "Util.h"
#include "Particle.h"
#include "Node.h"
#include "A_Star.h"
#include "robot.h"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int angle = 0;
int NUM_PARTICLES = 30;

#define VARIANCE 2
#define MOVEMENT 1
#define CAMERA_TRIES_MAX 10
#define IMAGE_THRES_REQ 12500

#define drawCross( center, color, d )                  \
line( image, cv::Point( center.x - d, center.y - d ),           \
cv::Point( center.x + d, center.y + d ), color, 2, CV_AA, 0);   \
line( image, cv::Point( center.x + d, center.y - d ),           \
cv::Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

#define redrawMacro() pos.x = node->getPosX(); \
		pos.y = node->getPosY(); \
		redraw(logo, pos, image, edges, doors, docks, arcReactorLoc, nodes); \
		waitKey(0)


void rotate (cv::Mat& src, cv::Mat& dst) {

	if(angle >= 360) {
		angle -= 360;
	}
	cout << "angle = " << angle << endl;	

	if(angle == 90) {
		flip(src.t(), dst, 1);
	}
	else if(angle == 180) {
		flip(src, dst, -1);	
	}
	else if(angle == 270) {
		flip(src.t(), dst, 0);
	}
	
}

void redraw(cv::Mat logo, Point pos, cv::Mat image, std::vector<Point> &edges, std::vector<Point> &virus,
                  std::vector<Point> &labA,
                  Point &labB, MapNode* nodes[][5]);
void updateProbability(std::vector<Particle> &particles, std::vector<cv::Point> &particlesShoot, double distance);
std::vector<Particle> resampleParticles(std::vector<Particle>& oldParticles);

int main (int argc, char * const argv[]) {
    char code;
    bool draw = true;
    Mat image;
    image = Mat::zeros(361, 301, CV_8UC3);
    image = cv::Scalar::all(0);
	setup();

    std::vector<Point > edges;
    std::vector<Point > doors;
    std::vector<Point > docks;
    Point arcReactorLoc;
    parseFile(edges,doors,docks,arcReactorLoc);
    drawMap(image, edges, doors, docks, arcReactorLoc);
	cv::Point pos(15, 15);
	cv :: Mat logo = cv :: imread ("ironman_icon.jpg");
	cv :: Mat imageROI;
    imageROI = image (cv :: Rect (pos.x, pos.y, logo.cols, logo.rows));
    logo.copyTo (imageROI);
    cv :: namedWindow("result");
    cv :: imshow ("result", image);
	cv::waitKey(0);
    
    MapNode* nodes[6][5];
    
    //generate nodes
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
		cv::Point* nodeLoc = new cv::Point((x * 60) + 30, (y * 60) + 30);
		MapNode* mapNode = new MapNode(*nodeLoc, x, y);
		nodes[y][x] = mapNode;
	  }
    }
    
    //figure out where each node can go
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
	//MapNode mapNode(nodeLoc);
	MapNode* mapNode = nodes[y][x];
	cv::Point nodeLoc = mapNode->getPosition();
	if(y - 1 >= 0)
	  mapNode->setDown((nodes[y - 1][x]));
	if(y + 1 <= 5)
	  mapNode->setUp((MapNode*)(nodes[y + 1][x]));
	if(x - 1 >= 0)
	  mapNode->setLeft((nodes[y][x - 1]));
	if(x + 1 <= 4)
	  mapNode->setRight((nodes[y][x + 1]));
	for(int move = 0; move < 31; move++)
	{
	  //check up
	  if((mapNode->getUp() != NULL) && (image.at<Vec3b>(nodeLoc.y + move, nodeLoc.x).val[0] == 255))
	    mapNode->setUp(NULL);
	  //check down
	  if(mapNode->getDown() != NULL && (image.at<Vec3b>(nodeLoc.y - move, nodeLoc.x).val[0] == 255))
	    mapNode->setDown(NULL);
	  //check left
	  if(mapNode->getLeft() != NULL && image.at<Vec3b>(nodeLoc.y, nodeLoc.x - move).val[0] == 255)
	    mapNode->setLeft(NULL);
	  //check right
	  if(mapNode->getRight() != NULL && image.at<Vec3b>(nodeLoc.y, nodeLoc.x + move).val[0] == 255)
	    mapNode->setRight(NULL);
	}
      }
    }
    
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
	MapNode* mapNode = nodes[y][x];
	cv::Point nodeLoc = mapNode->getPosition();
	if(mapNode->getDown() != NULL)
	  line(image, mapNode->getPosition(), mapNode->getDown()->getPosition(), Scalar(0, 255, 0), 4, 6);
	if(mapNode->getUp() != NULL)
	  line(image, mapNode->getPosition(), (mapNode->getUp())->getPosition(), Scalar(0, 255, 0), 4, 6);
	if(mapNode->getLeft() != NULL)
	  line(image, mapNode->getPosition(), mapNode->getLeft()->getPosition(), Scalar(0, 255, 0), 4, 6);
	if(mapNode->getRight() != NULL)
	  line(image, mapNode->getPosition(), mapNode->getRight()->getPosition(), Scalar(0, 255, 0), 4, 6);
      }
    }
    std::vector<MapNode*> possibleNodes(30);
    int i = 0;
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
		possibleNodes[i++] = nodes[y][x];
      }
    }
    printf("init size of possible locactions %i\n", possibleNodes.size());
    int lastMove = 0;
    //list of all nodes made, check sonar and remove impossible nodes
    while(possibleNodes.size() > 1)
    {
	  move(UP, true);
      int hasUp = checkSonar(1) / 60;
	  move(RIGHT, true);
      int hasRight = checkSonar(1) / 60;
	  move(DOWN, true);
      int hasDown = checkSonar(1) / 60;
	  move(LEFT, true);
      int hasLeft = checkSonar(1) / 60;
	   printf("size %i u%i d%i l%i r%i\n", possibleNodes.size(), hasDown, hasUp, hasLeft, hasRight);
	  for(int j = 0; j < possibleNodes.size(); j++)
	  {
	  //remove nodes that dont match this
		if(possibleNodes[j]->getDownFull() != hasDown)
		{
			possibleNodes.erase(possibleNodes.begin() + j);
			j--;
			continue;
		}
		if(possibleNodes[j]->getUpFull() != hasUp)
		{
			possibleNodes.erase(possibleNodes.begin() + j);
			j--;
			continue;
		}
		if(possibleNodes[j]->getRightFull() != hasRight)
		{
			possibleNodes.erase(possibleNodes.begin() + j);
			j--;
			continue;
		}
		if(possibleNodes[j]->getLeftFull() != hasLeft)
		{
			possibleNodes.erase(possibleNodes.begin() + j);
			j--;
			continue;
		}
      }
	  if(possibleNodes.size() == 0)
	  {
		  printf("error\n");
		  return 0;
	  }
      //do we need to move again
      if(possibleNodes.size() > 1)
      {
		//printf("ran\n");
		//favor up 1 and right 2
		if(hasUp && (lastMove & 4) != 4)
		{
			printf("went up\n");
			move(UP);
			lastMove |= 1;
			for(int z=0; z < possibleNodes.size(); z++)
			{
				possibleNodes[z] = possibleNodes[z]->getUp();
			}
		}
		else if(hasRight && (lastMove & 8) != 8)
		{
			printf("went right\n");
			lastMove |= 2;
			move(RIGHT);
			for(int z=0; z < possibleNodes.size(); z++)
			{
				possibleNodes[z] = possibleNodes[z]->getRight();
			}
		}
		else if(hasDown && (lastMove & 1) != 1)
		{
			lastMove |= 4;
			printf("went down\n");
			move(DOWN);
			for(int z=0; z < possibleNodes.size(); z++)
			{
				possibleNodes[z] = possibleNodes[z]->getDown();
			}
		}
		else if(hasLeft && (lastMove & 2) != 2)
		{
			lastMove |= 8;
			printf("went left\n");
			move(LEFT);
			for(int z=0; z < possibleNodes.size(); z++)
			{
				possibleNodes[z] = possibleNodes[z]->getLeft();
			}
		}
		else
		{
			//down first
			if((lastMove & 4) != 4)
			{
				lastMove &= ~1;
				lastMove |= 4;
				move(DOWN);
			}
			else if((lastMove & 8) != 8)
			{
				lastMove &= ~2;
				lastMove |= 8;
				move(LEFT);
			}
			else
			{
				lastMove = 0;
			}
		}
      }
    }
	MapNode* node = possibleNodes[0];
    printf("found robot at %i %i\n", node->getPosX(), node->getPosY());
    //should know where we are by this point
	
	redrawMacro();
	
	//**********************
	//OPEN UP VIDEO STUFF
	//**********************
	
	VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 160);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 120);

   // namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//Capture a temporary image from the camera
	Mat imgTmp;
    imgTmp = Mat::zeros(301, 301, CV_8UC3);

	cap.read(imgTmp); 

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
	
	//********************
	//end camera open
	//********************
	
	//start doing tasks
	int positionCheck = 0;
	bool visitedA = false;
	bool visitedB = false;
	bool visited2 = false;
	while(!visitedA || !visitedB)
	{
		MapNode* toVisit = NULL;
		switch(positionCheck)
		{
			case 0:
				toVisit = nodes[0][2];
				break;
			case 1:
				toVisit = nodes[0][3];
				break;
			case 2:
				toVisit = nodes[3][2];
				break;
			case 3:
				cout << "Visited all locations, missed a lab somewhere probably, error and ending\n";
				return 0;
		}
		if(positionCheck == 1)
		{
			string tempP = pathFind(node->getX(), node->getY(), toVisit->getX(), toVisit->getY(), nodes);
				toVisit = nodes[3][2];
				if(tempP.length() > pathFind(node->getX(), node->getY(), toVisit->getX(), toVisit->getY(), nodes).length())
					visited2 = true;
				else
					toVisit = nodes[0][3];
		}
		else if(positionCheck == 2)
			if(!visited2)
					toVisit = nodes[3][2];
				else
					toVisit = nodes[0][3];
		//move to node we are visiting
		node = node->traverse(toVisit, pathFind(node->getX(), node->getY(), toVisit->getX(), toVisit->getY(), nodes));
		redrawMacro();
		positionCheck++; //setup for next run if necessary
		
		//check sonar
		int toWallDist = checkSonar(1);
		if(toWallDist < 20)
		{
			while(checkSonar(1) < 20)
			{
				setSpeed(fp, ZERO - (180 - ZERO), 180);
				delay(20);
				setSpeed(fp, 0, 0);
			}
		}
		
		//camera check
		bool found = false;
		bool foundA = false;
		bool lookLeft = false;
		bool lookRight = false;
		bool lookStr = false;
		for(int tries = 0; tries < CAMERA_TRIES_MAX; tries++)
		{
			Mat imgOriginal;
			bool bSuccess = cap.read(imgOriginal); // read a new frame from video
			if (!bSuccess) //if not success, break loop
			{
				cout << "Cannot read a frame from video stream" << endl;
				return 0;
			}

			Mat imgHSV;
			cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
			
			if(!visitedA)
			{
				Mat imgThresholded;
				//booklet
				int iLowH = 80;
				int iHighH = 102;

				int iLowS = 32; 
				int iHighS = 255;

				int iLowV = 91;
				int iHighV = 255;

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
			
				found = dArea > IMAGE_THRES_REQ;
				
				if(found)
				{
					foundA = true;
					break;
				}
			}
			if(!visitedB && !found)
			{
				Mat imgThresholded;
				
				//green lego
				int iLowH = 63;
				int iHighH = 77;

				int iLowS = 73; 
				int iHighS = 255;

				int iLowV = 60;
				int iHighV = 255;

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
			
				found = dArea > IMAGE_THRES_REQ;
				
				if(found)
					break;
			}
			
			if(!lookLeft)
			{
				lookLeft = true;
				setSpeed(fp, 180, 180);
				delay(200);
				setSpeed(fp, 0, 0);
			}
			else if(!lookRight)
			{
				lookRight = true;
				setSpeed(fp, ZERO - (180 - ZERO), ZERO - (180 - ZERO));
				delay(400);
				setSpeed(fp, 0, 0);
			}
			else if(!lookStr)
			{
				lookStr = true;
				setSpeed(fp, 180, 180);
				delay(200);
				setSpeed(fp, 0, 0);
			}

		}	
		if(found && foundA)
		{
			cout << "Found virus A, going to lab A\n";
			node = node->traverse(nodes[4][0], pathFind(node->getX(), node->getY(), 0, 4, nodes));
			redrawMacro();
			visitedA = true;
		}
		else if(found)
		{
			cout << "Found virus B, going to lab B\n";
			node = node->traverse(nodes[4][4], pathFind(node->getX(), node->getY(), 4, 4, nodes));
			redrawMacro();
			visitedB = true;
		}
		else
		{
			cout << "No virus found, proceeding to next location!\n";
		}
		//if we looked left without looking straight, we need to fix position
		if(lookLeft && !lookStr)
		{
			lookRight = true;
			setSpeed(fp, ZERO - (180 - ZERO), ZERO - (180 - ZERO));
			delay(100);
			setSpeed(fp, 0, 0);
		}
	}
	printf("robot should be done\n");
    return 0;
}

void redraw(cv::Mat logo, Point pos, cv::Mat image, std::vector<Point> &edges, std::vector<Point> &virus,
                  std::vector<Point> &labA,
                  Point &labB, MapNode* nodes[][5])
{
  static cv::Mat clear = Mat::zeros(361, 301, CV_8UC3);
  clear.copyTo(image);
  int thickness = 6;
  int lineType = 8;
  
  //Draw Wall
  for(int i = 0;i < edges.size()-1; i+=2){
    Point p1 = edges[i];
    Point p2 = edges[i+1];
    line (image, p1, p2, Scalar(255, 0, 0), thickness, lineType);
  }
  //Draw Virus Location
    for(int i = 0;i < virus.size()-1; i+=2) {
        Point p1 = virus[i];
        Point p2 = virus[i+1];
        
        Point topLeft1, topRight1, bottomLeft1, bottomRight1;
        topLeft1.x = int (p1.x - p2.x/2);
        topLeft1.y = int (p1.y - p2.y/2);
        topRight1.x = int (p1.x + p2.x/2);
        topRight1.y = int (p1.y - p2.y/2);
        bottomLeft1.x = int (p1.x - p2.x/2);
        bottomLeft1.y = int (p1.y + p2.y/2);
        bottomRight1.x = int (p1.x + p2.x/2);
        bottomRight1.y = int (p1.y + p2.y/2);
        
        line(image,topLeft1, bottomRight1, CV_RGB(0, 150, 150));
        line(image,topRight1, bottomLeft1, CV_RGB(0, 150, 150));
    }
    
    //Draw LabA Location
    for(int i = 0;i < labA.size()-1; i+=2){
        Point p1 = labA[i];
        Point p2 = labA[i+1];
        
        Point topLeft, topRight, bottomLeft, bottomRight;
        topLeft.x = int (p1.x - p2.x/2);
        topLeft.y = int (p1.y - p2.y/2);
        topRight.x = int (p1.x + p2.x/2);
        topRight.y = int (p1.y - p2.y/2);
        bottomLeft.x = int (p1.x - p2.x/2);
        bottomLeft.y = int (p1.y + p2.y/2);
        bottomRight.x = int (p1.x + p2.x/2);
        bottomRight.y = int (p1.y + p2.y/2);
        rectangle(image, topLeft, bottomRight, cvScalar(204, 0, 204, 0),1,8,0);
        
        line(image,topLeft, bottomRight, CV_RGB(204,0,204));
        line(image,topRight, bottomLeft, CV_RGB(204,0,204));
    }
    //Draw LabB Location
    circle(image, labB, 10, CV_RGB(255,0,0), 5, CV_AA, 0);
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
	MapNode* mapNode = nodes[y][x];
	cv::Point nodeLoc = mapNode->getPosition();
	if(mapNode->getDown() != NULL)
	  line(image, mapNode->getPosition(), mapNode->getDown()->getPosition(), Scalar(0, 255, 0), 4, 6);
	if(mapNode->getUp() != NULL)
	  line(image, mapNode->getPosition(), (mapNode->getUp())->getPosition(), Scalar(0, 255, 0), 4, 6);
	if(mapNode->getLeft() != NULL)
	  line(image, mapNode->getPosition(), mapNode->getLeft()->getPosition(), Scalar(0, 255, 0), 4, 6);
	if(mapNode->getRight() != NULL)
	  line(image, mapNode->getPosition(), mapNode->getRight()->getPosition(), Scalar(0, 255, 0), 4, 6);
      }
    }
  cv :: Mat imageROI;
  imageROI = image (cv :: Rect (pos.x, pos.y, logo.cols, logo.rows));
  logo.copyTo (imageROI);
 // for(int i = 0; i < p.size(); i++)
 // {
 //   drawCross(p[i].getPosition(), Scalar(0, 255, 0), 5);
 // }
  cv :: imshow ("result", image);
}

void updateProbability(std::vector<Particle> &particles, std::vector<cv::Point> &particlesShoot, double distance) {
    
    float total_probabilities = 0.0;
    float new_weight = 0.0;
    float old_probabilities = 0.0;
    float new_probability = 0.0;
    double map_distance = 0.0;
    double sonar_variance = 10.0;
    
    // update all the particle probabilities
    for (int i=0; i<particles.size(); i++){
        cv::Point pos = particles[i].getPosition();
        
        // use heading to calculate the map distance from particle to wall.
        //map_distance =  distToEdge(direction,cv::Point(pos));
        map_distance = distToEdge(particles[i].getPosition(), particlesShoot[i]);
        
        // Compute new probability using measured distance , map distance and sonar variance
        new_probability =  getProbability(distance, sonar_variance, map_distance); //distance by sonar report, sonar variance, given loaction
        // update each probability and compute total probabilities
        old_probabilities = particles[i].getWeight(); //P(robot@location)
        new_weight = old_probabilities * new_probability; //P(Sensor Reading| Robot @ Location) * P(robot@location)
        particles[i].setWeight(new_weight);
        total_probabilities += new_weight; //Ex: 0.25 + 0.25 + 0.3 = 0.8, so N = 1/0.8
    }
    
    
    // Normalize all probabilities
    for (int i=0; i<particles.size(); i++){
        //normalized probability = probability / total probabilities
        particles[i].setWeight(particles[i].getWeight()/total_probabilities); //0.25/0.8 + 0.25/0.8 + 0.3/0.8 = 1
    }
    
}


std::vector<Particle> resampleParticles(std::vector<Particle>& oldParticles) {
    std::vector<Particle> newParticles;
    
    //Calculate a Cumulative Distribution Function for our particle weights
    std::vector<double> CDF;
    CDF.resize(oldParticles.size());
    CDF[0] = ((Particle)oldParticles[0]).getWeight();
    
    for(int i=1; i<CDF.size(); i++)
        CDF[i] = CDF[i-1] + oldParticles[i].getWeight();
    //Loop through our particles as if spinning a roulette wheel.
    //The random u that we start with determines the initial offset
    //and each particle will have a probability of getting landed on and
    //saved proportional to its posterior probability. If a particle has a very large
    //posterior, then it may get landed on many times. If a particle has a very low
    //posterior, then it may not get landed on at all. By incrementing by
    // 1/(numParticles) we ensure that we don't change the number of particles in our
    // returned set.
    
    int i = 0;
    double u = randomDouble()* 1.0/double(oldParticles.size());
    //double u = 1.0/double(oldParticles.size());
    
    
    for(int j=0; j < oldParticles.size(); j++){
        while(u > CDF[i]) //if i particle is very small, we don't want to let it in newparticle, so i++
            i++;
        //cout << "i = " << i << endl;
        
        Particle p = oldParticles[i]; //kill ridiculos particles, and leave possible particles
        p.setWeight(1.0/double(oldParticles.size()));
        newParticles.push_back(p);
        //cout << " particles[" << i << "].x_2 = " << newParticles[i].getPosition().x << endl;
        
        u += 1.0/double(oldParticles.size());
        //cout << "u = " << u << endl;
    }

    
    return newParticles;
}
