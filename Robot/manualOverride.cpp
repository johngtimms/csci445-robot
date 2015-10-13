#include <stdio.h>
#include<iostream>
#include<string>

#include"opencv2/highgui/highgui.cpp"



//For threading
#include<thread>
#include<pthread.h>

//Sleep Stuff
#include<chrono>
#include<unistd.h>

// Robot Stuff
#include"robot.h"








using namespace std;

//void *stayForward(void *arg);
//int kbhit(void);

//mutex charBufferLock;


int main(int argc, char *argv[] ){


//	Robot* robot = new Robot();
	
	// Initialize Terminal
/*	initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);*/

	printf("\n");
	printf("/////////////////////////////////// \n");
	printf("Welcome to Manual Override \n" );
	printf("/////////////////////////////////// \n\n");



	while(true){
//		printf("a");

		char c;

		if( waitKey(10) == 27 ){
			printf("Escape pressed\n");
		}
		
		
/*		if( kbhit()true ){
			//c = getch();
			
			if( c == 'w' ){
				robot->moveForward(4);
			}
			if( c == 's' ){
				robot->moveBackward(4);
			}
			if( c == 'a' ){
				robot->turnLeft(4);
			}
			if( c == 'd' ){
				robot->turnRight(4);
			}
			if( c == 1 ){
				robot->goForward();
			}
			//refresh();
		}	// endif kbhit()*/
	}

	return 0;
}


/*int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}*/


// Make your robot to follow the color
// 
