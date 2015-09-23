#include"robot.h"


#include<ncurses.h>
#include<pthread.h>
#include<thread>
#include<chrono>
#include<unistd.h>


bool flag_isForward;

// Robot Stuff
Robot::Robot(){
	orientation = -1;
	flag_isForward = false;
}
Robot::~Robot(){

}

//Relative Movement
void Robot::moveForward( int cm_forward ){
	printw("Move forward %d cm\n", cm_forward);
}

void Robot::moveBackward( int cm_back ){
	printw("Move back %d cm\n", cm_back );
}

void Robot::turnLeft( int degLeft ){
	printw("Turn left %d degrees\n",degLeft);
}

void Robot::turnRight( int degRight ){
	printw("Turn right %d degrees \n",degRight );	
}



void *stayForThread(void *argv){
	while( flag_isForward ){
		printw("b \n");
		sleep(1);
		refresh();
	}
}

void Robot::goForward(){

	if( flag_isForward != true ){
		flag_isForward = true;
		pthread_t t1;
		int ret;
		printw("In main: creating thread\n");
		ret =  pthread_create(&t1, NULL, &stayForThread, NULL);
		if(ret != 0) {
			printf("Error: pthread_create() failed\n");
			exit(EXIT_FAILURE);
		}
	}else{
		flag_isForward = false;
	}

}

