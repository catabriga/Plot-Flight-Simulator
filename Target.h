#ifndef TARGET_H_
#define TARGET_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#include "Types.h"
#include "Timer.h"


typedef struct Targets Targets;


Targets* createTargets(void);
void drawTargets(Targets* targets);
void destroyTargets(Targets* targets, Bullet* bullets, int numBullets);

#endif
