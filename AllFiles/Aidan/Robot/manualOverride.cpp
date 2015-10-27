#include <stdio.h>
#include<iostream>
#include<string>

//For threading
#include<thread>
#include<pthread.h>

//Sleep Stuff
#include<chrono>
#include<unistd.h>
#include<termios.h>

// Robot Stuff
#include"robot.h"




using namespace std;

//void *stayForward(void *arg);
//int kbhit(void);

//mutex charBufferLock;

char getch();


int main(int argc, char *argv[] ){


	Robot* robot = new Robot();
	
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
		
		if( /*kbhit()true*/true ){
			c = getch();
			
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
				robot->stayForward();
			}
			//refresh();
		}	// endif kbhit()
	}

	return 0; 
}

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
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
}
*/

// Make your robot to follow the color
// 
