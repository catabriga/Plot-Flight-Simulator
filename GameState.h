
#ifndef GAMESTATE_H_
#define GAMESTATE_H_


#define GAME_TIME_STEP 20

typedef struct GameState GameState;

#include "ReceiverSocket.h"
#include "SenderSocket.h"
#include "GameProtocol.h"
#include "Airplane.h"
#include "Types.h"
#include "Timer.h"
#include "Panel.h"
#include "Terrain.h"
#include "Target.h"

GameState* createGameState(unsigned short hostPort, char* hostIpAddress, unsigned short clientPort);
void destroyGameState(GameState* gameState);

void drawGameState(GameState* gameState);
void processGameState(GameState* gameState, long int delta_t);

void setPlaneThrottle(GameState* gameState, int direction);

void setPlaneAilerons(GameState* gameState, int direction);
void setPlaneElevator(GameState* gameState, int direction);
void setPlaneRudder(GameState* gameState, int direction);

void setFire(GameState* gameState, int fire);

Airplane** getListAirplane(GameState* gameState);
int getSelfAirplaneNumber(GameState* gameState);
int getNumAirplanes(GameState* gameState);

void setCamera(GameState* gameState, int camera);

#endif /* GAMESTATE_H_ */
