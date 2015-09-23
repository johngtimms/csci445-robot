


#include <stdio.h>
#include<conio.h>
#include<iostream>
//#include<chrono>

using namespace std;

bool flag_isForward;

void stayForward(){
	
	while( flag_isForward ){
		printf("Is moving forward \n");
		//sleep_for(chrono::nanoseconds(10));
	}
	
}

int main(int argc, char *argv[] ){

	flag_isForward = false;

	printf("\n");
	printf("/////////////////////////////////// \n");
	printf("Welcome to Manual Override \n" );
	printf("/////////////////////////////////////  \n");
	printf("/////////////////////////////////////  \n\n");

	
	while(true){
		
		if( kbhit()!=0 ){
			//printf("%s", getch() );
			//printf( "%c", getch() );
			char c = getch();
			
			if( c == 'w' ){
				printf("Move Forward\n");
			}
			if( c == 's' ){
				printf("Move Back\n");
			}
			if( c == 'a' ){
				printf("Move Left\n");
			}
			if( c == 'd' ){
				printf("Move Right\n");
			}
			if( c == 1 ){
				if( flag_isForward )
					flag_isForward = false;
				else{
					printf("Ctrl-W clicked \n");
					thread::thread t1;
					//(stayForward,"forwardThread");
					//t1.join();
				}
			}
		}
	
	}

	return 0;
}
