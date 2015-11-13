#ifndef Map_Node_H
#define Map_Node_H

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <cstdlib>

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "Util.h"
#include <math.h>

using namespace std;

class MapNode {
    
private:
    cv::Point location;
    MapNode *downNode;
    MapNode *upNode;
    MapNode *rightNode;
    MapNode *leftNode;
    
public:
    MapNode(cv::Point start) {
        location = start;
	downNode = NULL;
	upNode = NULL;
	rightNode = NULL;
	leftNode = NULL;
    }
    ~MapNode(){}
    
    
    void setDown(MapNode* m)
    {
      downNode = m;
    }
    
    MapNode* getDown()
    {
      return downNode;
    }
    
    int getDownFull()
    {
      MapNode* node = downNode;
      int ret = 0;
      while(node != NULL)
      {
	ret++;
	node = node->getDown();
      }
      return ret;
    }
    
    void setUp(MapNode* m)
    {
      upNode = m;
    }
    
    MapNode* getUp()
    {
      return upNode;
    }
    
    int getUpFull()
    {
      MapNode* node = upNode;
      int ret = 0;
      while(node != NULL)
      {
	node = node->getUp();
	ret++;
      }
      return ret;
    }
    
    void setLeft(MapNode* m)
    {
      leftNode = m;
    }
    
    MapNode* getLeft()
    {
      return leftNode;
    }
    
    int getLeftFull()
    {
      MapNode* node = leftNode;
      int ret = 0;
      while(node != NULL)
      {
	node = node->getLeft();
	ret++;
      }
      return ret;
    }
    
    void setRight(MapNode* m)
    {
      rightNode = m;
    }
    
    MapNode* getRight()
    {
      return rightNode;
    }
    
    int getRightFull()
    {
      MapNode* node = rightNode;
      int ret = 0;
      while(node != NULL)
      {
	node = node->getRight();
	ret++;
      }
      return ret;
    }
    
    cv::Point getPosition(){return location;}  
    
};
#endif