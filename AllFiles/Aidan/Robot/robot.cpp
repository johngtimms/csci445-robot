#include"robot.h"


//#include<ncurses.h>
#include<pthread.h>
#include<thread>
#include<chrono>
#include<unistd.h>


bool flag_followWall;
bool flag_isForward;

// Robot Stuff
Robot::Robot(){
	orientation = -1;
	flag_followWall = false;
}
Robot::~Robot(){

}

//Relative Movement
void Robot::moveForward( int cm_forward ){
	printf("Move forward %d cm\n", cm_forward);
	// 
}

void Robot::moveBackward( int cm_back ){
	printf("Move back %d cm\n", cm_back );
	//
}

void Robot::turnLeft( int degLeft ){
	printf("Turn left %d degrees\n",degLeft);
	//
}

void Robot::turnRight( int degRight ){
	printf("Turn right %d degrees \n",degRight );
	//
}

// Continuous Functions 

void *keepForward_thread(void *argv){
	while( flag_isForward ){
		sleep(1);
	//	refresh();
	}
}

// Continuous Movement
void Robot::stayForward(){
if( flag_isForward != true ){
		
	flag_isForward = true;
	pthread_t t1;
	int ret;
	printf("In main: creating thread\n");
	ret =  pthread_create(&t1, NULL, &keepForward_thread, NULL);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}
	}else{
		flag_isForward = false;
	}

};

void Robot::stayBackward(){};
void Robot::stayLeft(){};
void Robot::stayRight(){};





void Robot::followWall(int cm){

	/*if( flag_followWall != true ){
		flag_followWall = true;
		pthread_t t1;
		int ret;
		printf("In main: creating thread\n");
		/*ret =  pthread_create(&t1, NULL, &followWall_thread, NULL);
		if(ret != 0) {
			printf("Error: pthread_create() failed\n");
			exit(EXIT_FAILURE);
		}
	}else{
		flag_followWall = false;
	}*/

}

