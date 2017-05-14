#ifndef AIRPLANE_H_
#define AIRPLANE_H_

#include "Types.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

typedef struct Airplane Airplane;

Airplane* createAirplane(Pose3D initialPose, Vector3D initialVelocity);
void deleteAirplane(Airplane* plane);

Pose3D getAirplanePose(Airplane* airplane);
Vector3D getAirplaneVelocity(Airplane* airplane);
Vector3D getAirplaneAcceleration(Airplane* airplane);
Orientation3D getAirplaneAngularVelocity(Airplane* airplane);

void drawPlane(Airplane* plane);
void drawPlaneShadow(Airplane* plane);


void setAirplanePose(Airplane* airplane, Pose3D pose);
void setAirplaneVelocity(Airplane* airplane, Vector3D velocity);
void setAirplaneRollVelocity(Airplane* airplane, double rollVelocity);
void setAirplanePitchVelocity(Airplane* airplane, double pitchVelocity);
void setAirplaneYawVelocity(Airplane* airplane, double yawVelocity);
void setAirplaneAngularVelocity(Airplane* airplane, Orientation3D angularVelocity);
void setAirplaneAcceleration(Airplane* airplane, Vector3D acceleration);
void moveAirplane(Airplane* airplane, long int delta_t);
void hitAirplane(Airplane* airplane, int selfPlaneNum, Bullet* bullets, int numBullets);

int isCrashed(Airplane* airplane);

#endif /* AIRPLANE_H_ */
