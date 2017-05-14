#ifndef PANEL_H_
#define PANEL_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "GameState.h"
#include "Airplane.h"
#include "Types.h"

void drawSpeedometer(GameState *gameState);
void drawHorizon (GameState *gameState);
void drawEmbication(GameState *gameState);
void drawAltimeter(GameState *gameState);
void drawRadar(GameState *gameState);
void drawCompass(GameState *gameState);

#endif /* PANEL_H_ */
