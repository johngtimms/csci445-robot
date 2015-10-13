#include"robot.h"


#include<ncurses.h>
#include<pthread.h>
#include<thread>
#include<chrono>
#include<unistd.h>


bool flag_followWall;

// Robot Stuff
Robot::Robot(){
	orientation = -1;
	flag_followWall = false;
}
Robot::~Robot(){

}

//Relative Movement
void Robot::moveForward( int cm_forward ){
	printw("Move forward %d cm\n", cm_forward);
	// 
}

void Robot::moveBackward( int cm_back ){
	printw("Move back %d cm\n", cm_back );
	//
}

void Robot::turnLeft( int degLeft ){
	printw("Turn left %d degrees\n",degLeft);
	//
}

void Robot::turnRight( int degRight ){
	printw("Turn right %d degrees \n",degRight );
	//
}

// Continuous Movement
void Robot::stayForward(){
};

void Robot::stayBackward(){};
void Robot::stayLeft(){};
void Robot::stayRight(){};



// Continuous Functions 

void *followWall_thread(void *argv, int howFar){
	while( flag_isForward ){
		sleep(1);
		refresh();
	}
}

void Robot::followWall(int cm){

	if( flag_followWall != true ){
		flag_followWall = true;
		pthread_t t1;
		int ret;
		printw("In main: creating thread\n");
		ret =  pthread_create(&t1, NULL, &stayForThread, NULL);
		if(ret != 0) {
			printf("Error: pthread_create() failed\n");
			exit(EXIT_FAILURE);
		}
	}else{
		flag_followWall = false;
	}

}

