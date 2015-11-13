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

using namespace cv;
using namespace std;

int angle = 0;
int NUM_PARTICLES = 30;

#define VARIANCE 2
#define MOVEMENT 1

#define drawCross( center, color, d )                  \
line( image, cv::Point( center.x - d, center.y - d ),           \
cv::Point( center.x + d, center.y + d ), color, 2, CV_AA, 0);   \
line( image, cv::Point( center.x + d, center.y - d ),           \
cv::Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )


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
                  Point &labB, std::vector<Particle> &p, MapNode* nodes[][5]);
void updateProbability(std::vector<Particle> &particles, std::vector<cv::Point> &particlesShoot, double distance);
std::vector<Particle> resampleParticles(std::vector<Particle>& oldParticles);

/**
 * Rotate an image
 * From: http://opencv-code.com/quick-tips/how-to-rotate-image-in-opencv/
 */
void rotate(cv::Mat& src, double angle, cv::Mat& dst)
{
    int len = std::max(src.cols, src.rows);
    cv::Point2f pt(len/2., len/2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

    cv::warpAffine(src, dst, r, cv::Size(len, len));
}

int main (int argc, char * const argv[]) {
    char code;
    bool draw = true;
    Mat image;
    image = Mat::zeros(361, 301, CV_8UC3);
    image = cv::Scalar::all(0);

    std::vector<Point > edges;
    std::vector<Point > doors;
    std::vector<Point > docks;
    Point arcReactorLoc;
    parseFile(edges,doors,docks,arcReactorLoc);
    drawMap(image, edges, doors, docks, arcReactorLoc);
    
    MapNode* nodes[6][5];
    
    //generate nodes
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
	cv::Point* nodeLoc = new cv::Point((x * 60) + 30, (y * 60) + 30);
	MapNode* mapNode = new MapNode(*nodeLoc);
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
    
    //cout <<"path: " << pathFind(0, 0, 4, 0, nodes) << "\n";
    //cout << "getUpFull " << nodes[0][0]->getUpFull() << endl;
    
    cv :: Mat logo = cv :: imread ("ironman_icon.jpg");
    srand (time(NULL));
    MapNode* node = nodes[rand() % 6][rand() % 5];
    //MapNode* node = nodes[0][0];
    cv::Point pos(node->getPosition().x - 15, node->getPosition().y - 15);
    //figure out what node we are at without using pixel locations
    std::vector<MapNode*> possibleNodes(30);
    int i = 0;
    for(int y = 0; y < 6; y++)
    {
      for(int x = 0; x < 5; x++)
      {
	possibleNodes[i++] = nodes[y][x];
      }
    }
    printf("init size %i\n", possibleNodes.size());
    int lastMove = 0;
    //list of all nodes made, check sonar and remove impossible nodes
    while(possibleNodes.size() > 1)
    {
      int hasDown = 0;
      for(int move = 1; move < 1000; move++)
      {
	if(image.at<Vec3b>(pos.y + 15 - move, pos.x + 15).val[0] == 255)
	{
	  hasDown = move > 35 ? move / 60 : 0;
	  break;
	}
      }
      int hasUp = 0;
      for(int move = 1; move < 1000; move++)
      {
	if(image.at<Vec3b>(pos.y + 15 + move, pos.x + 15).val[0] == 255)
	{
	  hasUp = move > 35 ? move / 60 : 0;
	  break;
	}
      }
      int hasLeft = 0;
      for(int move = 1; move < 1000; move++)
      {
	if(image.at<Vec3b>(pos.y + 15, pos.x + 15 - move).val[0] == 255)
	{
	  hasLeft = move > 35 ? move / 60 : 0;
	  break;
	}
      }
      int hasRight = 0;
      for(int move = 1; move < 1000; move++)
      {
	if(image.at<Vec3b>(pos.y + 15, pos.x + 15 + move).val[0] == 255)
	{
	  hasRight = move > 35 ? move / 60 : 0;
	  break;
	}
      }
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
      //do we need to move again
      if(possibleNodes.size() > 1)
      {
	//printf("ran\n");
	//favor up 1 and right 2
	if(hasUp && (lastMove & 4) != 4)
	{
	  printf("went up\n");
	  lastMove |= 1;
	  pos.y += 60;
	  for(int z=0; z < possibleNodes.size(); z++)
	  {
	    possibleNodes[z] = possibleNodes[z]->getUp();
	  }
	}
	else if(hasRight && (lastMove & 8) != 8)
	{
	  printf("went right\n");
	  lastMove |= 2;
	  pos.x += 60;
	  for(int z=0; z < possibleNodes.size(); z++)
	  {
	    possibleNodes[z] = possibleNodes[z]->getRight();
	  }
	}
	else if(hasDown && (lastMove & 1) != 1)
	{
	  lastMove |= 4;
	  printf("went down\n");
	  pos.y -= 60;
	  for(int z=0; z < possibleNodes.size(); z++)
	  {
	    possibleNodes[z] = possibleNodes[z]->getDown();
	  }
	}
	else if(hasLeft && (lastMove & 2) != 2)
	{
	  lastMove |= 8;
	  printf("went left\n");
	  pos.x -= 60;
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
	  }
	  else if((lastMove & 8) != 8)
	  {
	    lastMove &= ~2;
	    lastMove |= 8;
	  }
	  else
	  {
	    lastMove = 0;
	  }
	}
      }
    }
    printf("found robot at %i %i\n", possibleNodes[0]->getPosition().y, possibleNodes[0]->getPosition().x);
    if(possibleNodes.size() > 1)
      printf("found robot at %i %i\n", possibleNodes[1]->getPosition().y, possibleNodes[1]->getPosition().x);
    //should know where we are by this point
    cv :: Mat imageROI;
    imageROI = image (cv :: Rect (pos.x, pos.y, logo.cols, logo.rows));
    logo.copyTo (imageROI);
    cv :: namedWindow("result");
    cv :: imshow ("result", image);
    cv :: waitKey (0);
    std::vector<Particle> p(NUM_PARTICLES);
    //create particles
    for(int i = 0 ; i < NUM_PARTICLES; i++)
    {   
      p[i] = Particle(cv::Point(50,50), 1/NUM_PARTICLES);
    }
    //end create

    while(1) {
        code = (char)cv::waitKey(100);
	
        
	//forward
        if (code == 'f') {
	  draw = true;
	  Point temp(pos.x, pos.y);
	  //printf("DEBUG: x before: %i, y before: %i\n", pos.x, pos.y);
          switch(angle)
	  {
	    case 0 :
	      pos.x = min((pos.x + MOVEMENT), 300-33);
	      break;
	    case 90 :
	      pos.y = min((pos.y + MOVEMENT), 360-33);
	      break;
	    case 180 :
	      pos.x = max((pos.x - MOVEMENT), 5);
	      break;
	    case 270 :
	      pos.y = max((pos.y - MOVEMENT), 5);
	      break;
	  }
	  //printf("DEBUG: x after: %i, y after: %i\n", pos.x, pos.y);
	  for(int i = 0 ; i < p.size(); i++)
	  {   
	    p[i].moveParticle(angle, 1, VARIANCE);
	  }
	  //printf("moved\n");
        }
        
        //sensor
        if (code == 's') {
	  //get probability checked
	  std:vector<Point> shoot(NUM_PARTICLES);
	  Point start;
	  Point end;
	  double distance = 0;
	  switch(angle)
	  {
	    case 0 :
	      start = Point(pos.x + 30, pos.y + 15);
	      end = Point(300, start.y);
	      distance = 300 - pos.x;
	      break;
	    case 90 :
	      start = Point(pos.x + 15, pos.y + 30);
	      end = Point(start.x, 300);
	      distance = 300 - pos.y;
	      break;
	    case 180 :
	      start = Point(pos.x, pos.y + 15);
	      end = Point(0, start.y);
	      distance = pos.x;
	      break;
	    case 270 :
	      start = Point(pos.x + 15, pos.y);
	      end = Point(start.x, 0);
	      distance = pos.y;
	      break;
	  }
	  //printf("DEBUG: start: %i %i, end %i %i\n",start.x, start.y, end.x, end.y);
	  for(int i = 0 ; i < NUM_PARTICLES; i++)
	  {   
	    shoot[i]= Point(end.x, end.y);
	  }
	  updateProbability(p, shoot, distance);
	  std::vector<Particle> drawer = resampleParticles(p);
	  //draw cross
	  double x = 0;
	  double y = 0;
	  //printf("drawer size %i\n", drawer.size());
	  for(int i = 0 ; i < drawer.size(); i++)
	  {   
	    Point drawerPoint = drawer[i].getPosition();
	    x += drawerPoint.x * drawer[i].getWeight();
	    y += drawerPoint.y * drawer[i].getWeight();
	  }
	  Point center(x, y);
	  //disable drawing to see cross and line
	  redraw(logo, pos, image, edges, doors, docks, arcReactorLoc, drawer, nodes);
	  line(image, start, end, Scalar(255, 255, 255), 3, 8);
	  drawCross(center, Scalar(0, 0, 255), 10);
	  cv :: imshow ("result", image);
	  draw = false;
        }
        
        //rotate
        if (code == 'r') {
	  draw = true;
	  //rotate object
	  //printf("DEBUG: x before: %i, y before: %i %i %i\n", pos.x, pos.y, logo.cols, logo.rows);
	  rotate(logo, 90, logo);
	  //printf("DEBUG: x after: %i, y after: %i %i % i\n", pos.x, pos.y, logo.cols, logo.rows);
	  angle = (angle + 90) % 360;
	  cout << "angle = " << angle << endl;
	  //end rotate
        }
        
        if (draw) {
	  redraw(logo, pos, image, edges, doors, docks, arcReactorLoc, p, nodes);
        }
        
    }
    
    return 0;
}

void redraw(cv::Mat logo, Point pos, cv::Mat image, std::vector<Point> &edges, std::vector<Point> &virus,
                  std::vector<Point> &labA,
                  Point &labB, std::vector<Particle> &p, MapNode* nodes[][5])
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
  for(int i = 0; i < p.size(); i++)
  {
    drawCross(p[i].getPosition(), Scalar(0, 255, 0), 5);
  }
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
