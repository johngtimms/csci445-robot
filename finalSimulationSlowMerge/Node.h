#ifndef Map_Node_H
#define Map_Node_H

#include <iostream>
#include <vector>
#include "robot.h"

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
	int posX, posY;
    
public:
    MapNode(cv::Point start, int x, int y) {
        location = start;
		downNode = NULL;
		upNode = NULL;
		rightNode = NULL;
		leftNode = NULL;
		posX = x;
		posY = y;
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
	
	int getX()
	{
		return posX;
	}
	
	int getY()
	{
		return posY;
	}
	
	MapNode* traverse(MapNode* node, string path)
	{
		cout << "Going from x:" << posX << " y:" << posY << " to x:" << node->getX() << " y:" << node->getY() << " using path " << path << "\n";
		for(int i = 0; i < path.length(); i++)
		{
			cout << "Moving in direction " << path.at(i) << "\n";
			int toMove = ((int)path.at(i)) - '0';
			move(toMove);
		}
		cout << "Should be at position\n";
		return node;
	}
    
    cv::Point getPosition(){return location;} 

	int getPosX()
	{
		return location.x - 15;
	}
	
	int getPosY()
	{
		return location.y - 15;
	}
    
};
#endif