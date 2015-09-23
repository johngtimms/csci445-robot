#include <iostream>




class Robot{
	
	int orientation;
	
	//Relative Movement
	
	void moveForward( int cm_for );
	void moveBackward( int cm_back );
	
	void turnLeft( int degLeft );
	void turnRight( int degRight );
	
	//Turning on Orientation
	void turnNorth();
	void turnEast();
	void turnSouth();
	void turnWest();
	void setOrientaiton( int degTrue );
	
	
}