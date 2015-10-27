#ifndef ROBOT_H
#define ROBOT_H


class Robot{

private:	
	int orientation;
	bool flag_isForward;

public:
	Robot();
	~Robot();
	
	//Relative Movement
	void moveForward( int cm_for );
	void moveBackward( int cm_back );
	void turnLeft( int degLeft );
	void turnRight( int degRight );
	
	//Continuous Movement 
	void goForward();
	void stayForward();
	void stayBackward();
	void stayLeft();
	void stayRight();

	//Turning on Orientation
	void turnNorth();
	void turnEast();
	void turnSouth();
	void turnWest();
	void setOrientaiton( int degTrue );
	
	
	void followWall(int cm);
	
};

#endif