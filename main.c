#include "Window.h"
#include "GLDraw.h"
#include "GameState.h"
#include "Timer.h"

#define HOST_DEFAULT_PORT 6112
#define CLIENT_DEFAULT_PORT 6113

GameState* gameState;

void mouseFunc(int type, int button, int x, int y);
void keyPress(int code);
void keyRelease(int code);  

GameState* initGameState(int argc, char** argv)
{
	GameState* gameState = NULL;
	
	if(argc == 4)
	{
		gameState = createGameState(atoi(argv[1]), argv[2], atoi(argv[3]));
	}
	else if(argc == 3)
	{
		gameState = createGameState(atoi(argv[1]), argv[2], CLIENT_DEFAULT_PORT);
	}
	else if(argc == 2)
	{
		gameState = createGameState(atoi(argv[1]), NULL, CLIENT_DEFAULT_PORT);
	}
	else
	{
		gameState = createGameState(HOST_DEFAULT_PORT, NULL, CLIENT_DEFAULT_PORT);
	}
	
	return gameState;
} 

void printInstruction(void)
{
	printf("\nUsage: plot [host port] [host ip] [client port]\n\n");
	printf("Keys:\n");
	printf("\tThrottle: W and S\n");
	printf("\tRudder: A and D\n");
	printf("\tAilerons: Arrow Left and Arrow Right\n");
	printf("\tElevator: Arrow Up and Arrow Down\n");
	printf("\tFire: Shift Or Space\n");
	printf("\tCameras: Numbers 1 to 4\n");
}

int main(int argc, char** argv)
{	
	printInstruction();

	window* w = initWindow(400,400);
	
	initGl();
	
	gameState = initGameState(argc, argv);
	
	long int lastTime = getCurrentTime();
	
	while(processWindow(w, mouseFunc, keyPress, keyRelease))
	{
		if(getCurrentTime() - lastTime >= GAME_TIME_STEP)
		{
			processGameState(gameState, GAME_TIME_STEP);
			lastTime += GAME_TIME_STEP;
		
			drawGameState(gameState);
			showWindow(w);

		}
	}
	return 0;
}


void mouseFunc(int type, int button, int x, int y)
{
	//printf("mouse - type: %d, button: %d, x: %d, y: %d\n", type, button, x, y);
}

void keyPress(int code)
{
	//printf("key: %d, type: press\n", code);
	
	switch(code)
	{
		case 10:
		case 11:
		case 12:
		case 13:
		{
			setCamera(gameState, code - 10);
		}break;
	
		case 25:	// W
		{	
			setPlaneThrottle(gameState, 1);
		}break;
		
		case 38:	// A
		{
			setPlaneRudder(gameState, 1);
		}break;
		
		case 39:	// S
		{
			setPlaneThrottle(gameState, -1);
		}break;
		
		case 40:	// D
		{
			setPlaneRudder(gameState, -1);
		}break;
		
		case 113:	// LEFT ARROW
		{
			setPlaneAilerons(gameState, -1);
		}break;
		
		case 114:	// RIGHT ARROW
		{
			setPlaneAilerons(gameState, 1);
		}break;
		
		case 111:	// UP ARROW
		{
			setPlaneElevator(gameState, 1);
		}break;
		
		case 116:	// DOWN ARROW
		{
			setPlaneElevator(gameState, -1);
		}break;
		
		case 50:
		case 65:	// SPACE
		{
			setFire(gameState, 1);
		}break;
	}
}

void keyRelease(int code)
{
	//printf("key: %d, type: release\n", code);
	
	switch(code)
	{
		case 25:	// W
		{	
			setPlaneThrottle(gameState, 0);
		}break;
		
		case 38:	// A
		{
			setPlaneRudder(gameState, 0);
		}break;
		
		case 39:	// S
		{
			setPlaneThrottle(gameState, 0);
		}break;
		
		case 40:	// D
		{
			setPlaneRudder(gameState, 0);
		}break;
		
		case 113:	// LEFT ARROW
		{
			setPlaneAilerons(gameState, 0);
		}break;
		
		case 114:	// RIGHT ARROW
		{
			setPlaneAilerons(gameState, 0);
		}break;
		
		case 111:	// UP ARROW
		{
			setPlaneElevator(gameState, 0);
		}break;
		
		case 116:	// DOWN ARROW
		{
			setPlaneElevator(gameState, 0);
		}break;
		
		case 50:
		case 65:	// SPACE
		{
			setFire(gameState, 0);
		}break;
		
	}
}




