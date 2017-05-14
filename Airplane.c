#include <stdlib.h>
#include <math.h>

#include "Airplane.h"

struct Airplane
{
	Pose3D pose;
	Vector3D velocity;
	Vector3D acceleration;
	Orientation3D angularVelocity;
	int hit;
	int crashed;	
};

static void drawEllipseCone(double a1,double b1,double a2,double b2,double dist, int n, double compNorm1, double compNorm2);
static void drawHalfWing(double thickness, double width, double length, double nLength, double nEllipse);
static void drawWing(double thickness, double width, double length, double nLength, double nEllipse);

void drawPlaneShadow(Airplane* plane)
{
	glPushMatrix();
	
		glColor3f(0.0f,0.0f,0.0f);
	
		glScalef(1.0,1.0,-1.0);
		glRotatef(90,0.0,0.0,1.0);
		glRotatef(90,1.0,0.0,0.0);

//		printf("\nAsa principal:\n");
		//glColor3f(0.2f,0.5f,0.2f);
		drawWing(0.05, 0.42, 3.6, 16, 16);//desenha a asa principal

//		printf("\nCorpo\n");
		//glColor3f(0.0f,0.3f,0.0f);
		glPushMatrix();
			glRotatef(90.0f,0.0f,1.0f,0.0f);
			glTranslatef(0.4f,-0.1f,0.0f);
			drawWing(0.3, 0.3, 3.0, 20, 16);//desenha o corpo
		glPopMatrix();

//		printf("\nAsa traseira\n");
		//glColor3f(0.2f,0.5f,0.2f);
		glPushMatrix();
			glTranslatef(0.0f,0.0f,-1.6f);
			drawWing(0.04, 0.25, 1.50, 8, 16);//desenha a asa traseira
		glPopMatrix();

//		printf("\nLeme\n");
		//glColor3f(0.6f,0.6f,0.0f);
		glPushMatrix();
			glTranslatef(0.0f,-0.80f,-1.65f);
			glRotatef(90.0f,0.0f,0.0f,1.0f);
			drawHalfWing(0.05, 0.20, 0.80, 8, 16);//desenha o "leme"
		glPopMatrix();
	glPopMatrix();
}

void drawPlane(Airplane* plane)
{
	glPushMatrix();
		glScalef(1.0,1.0,-1.0);
		glRotatef(90,0.0,0.0,1.0);
		glRotatef(90,1.0,0.0,0.0);

//		printf("\nAsa principal:\n");
		glColor3f(0.2f,0.5f,0.2f);
		drawWing(0.05, 0.42, 3.6, 16, 16);//desenha a asa principal

//		printf("\nCorpo\n");
		glColor3f(0.0f,0.3f,0.0f);
		glPushMatrix();
			glRotatef(90.0f,0.0f,1.0f,0.0f);
			glTranslatef(0.4f,-0.1f,0.0f);
			drawWing(0.3, 0.3, 3.0, 20, 16);//desenha o corpo
		glPopMatrix();

//		printf("\nAsa traseira\n");
		glColor3f(0.2f,0.5f,0.2f);
		glPushMatrix();
			glTranslatef(0.0f,0.0f,-1.6f);
			drawWing(0.04, 0.25, 1.50, 8, 16);//desenha a asa traseira
		glPopMatrix();

//		printf("\nLeme\n");
		glColor3f(0.6f,0.6f,0.0f);
		glPushMatrix();
			glTranslatef(0.0f,-0.80f,-1.65f);
			glRotatef(90.0f,0.0f,0.0f,1.0f);
			drawHalfWing(0.05, 0.20, 0.80, 8, 16);//desenha o "leme"
		glPopMatrix();
	glPopMatrix();
}

static void drawWing(double thickness, double width, double length, double nLength, double nEllipse)
{
	glPushMatrix();
		glTranslatef(-length/2,0.0f,0.0f);
		drawHalfWing(thickness, width, length/2, nLength/2, nEllipse);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(length/2,0.0f,0.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		drawHalfWing(thickness, width, length/2, nLength/2, nEllipse);
	glPopMatrix();
}

static void drawHalfWing(double thickness, double width, double length, double nLength, double nEllipse)
{
	int i;
	double b, incThickness = thickness/(nLength), incWidth = width/(nLength);

	glPushMatrix();
		b = length*4/(pow(nLength,2)*pow(nLength+1,2));

		double reDist = length;
		double fator = 0.05;

		for(i = 1; i <= nLength; i++)
		{
			double dist = pow(i,3)*b;
			drawEllipseCone(incThickness*(i-1),incWidth*(i-1),incThickness*i,incWidth*i,dist, nEllipse, -reDist*fator, -(reDist-dist)*fator);
			glTranslatef(dist,0.0f,0.0f);
			reDist -= dist;
		}
	glPopMatrix();
}

static void drawEllipseCone(double a1,double b1,double a2,double b2,double dist, int n, double compNorm1, double compNorm2)
{
	double angIncrement = 2*3.14155926/n;
	double ang = 0.0;
	int i;
	double vx1, vy1, vx2, vy2, ux1, ux2, uy1, uy2, normW1, normW2, normaX1, normaY1, normaX2, normaY2, normaZ1, normaZ2;

	glPushMatrix();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLE_STRIP);
			for(i = 0; i <= n; i++)
			{
				vx1=a1*cos(ang);
				vy1=b1*sin(ang);
				vx2=a2*cos(ang);
				vy2=b2*sin(ang);
				ux1 = 2*vx1/(a1*a1);
				uy1 = 2*vy1/(b1*b1);
				ux2 = 2*vx2/(a2*a2);
				uy2 = 2*vy2/(b2*b2);
				normW1 = sqrt(ux1*ux1+uy1*uy1);
				normW2 = sqrt(ux2*ux2+uy2*uy2);
				ux1 = ux1/normW1;
				uy1 = uy1/normW1;
				ux2 = ux2/normW2;
				uy2 = uy2/normW2;

				normW1 = sqrt(ux1*ux1+uy1*uy1+compNorm1*compNorm1);
				normW2 = sqrt(ux2*ux2+uy2*uy2+compNorm2*compNorm2);
				normaX1 = ux1/normW1;
				normaY1 = uy1/normW1;
				normaX2 = ux2/normW2;
				normaY2 = uy2/normW2;
				
				normaZ1 = compNorm1/normW1;
				normaZ2 = compNorm2/normW2;
				glNormal3f(normaZ1,normaX1,normaY1);//falta fazer a inclinacao em x
				glVertex3f(0.0f,vx1,vy1);
				glNormal3f(normaZ2,normaX2,normaY2);//falta fazer a inclinacao em x
				glVertex3f(dist,vx2,vy2);
				ang += angIncrement;
			}
		glEnd();
/*		glColor3f(1.0f,1.0f,1.0f);*/
/*		glBegin(GL_LINES);*/
/*			ang = 0;*/
/*			for(i = 0; i <= n; i++)*/
/*			{*/
/*				vx1=a1*cos(ang);*/
/*				vy1=b1*sin(ang);*/
/*				vx2=a2*cos(ang);*/
/*				vy2=b2*sin(ang);*/
/*				ux1 = 2*vx1/(a1*a1);*/
/*				uy1 = 2*vy1/(b1*b1);*/
/*				ux2 = 2*vx2/(a2*a2);*/
/*				uy2 = 2*vy2/(b2*b2);*/
/*				normW1 = sqrt(ux1*ux1+uy1*uy1);*/
/*				normW2 = sqrt(ux2*ux2+uy2*uy2);*/
/*				ux1 = ux1/normW1;*/
/*				uy1 = uy1/normW1;*/
/*				ux2 = ux2/normW2;*/
/*				uy2 = uy2/normW2;*/

/*				normW1 = sqrt(ux1*ux1+uy1*uy1+compNorm1*compNorm1);*/
/*				normW2 = sqrt(ux2*ux2+uy2*uy2+compNorm2*compNorm2);*/
/*				normaX1 = ux1/normW1;*/
/*				normaY1 = uy1/normW1;*/
/*				normaX2 = ux2/normW2;*/
/*				normaY2 = uy2/normW2;*/
/*				normaZ1 = compNorm1/normW1;*/
/*				normaZ2 = compNorm2/normW2;*/
/*				glVertex3f(0.0f,vx1,vy1);*/
/*				glVertex3f(normaZ1,vx1+normaX1,vy1+normaY1);*/
/*				glVertex3f(dist,vx2,vy2);*/
/*				glVertex3f(dist+normaZ2,vx2+normaX2,vy2+normaY2);*/
/*				ang += angIncrement;*/
/*			}*/
/*		glEnd();*/
	glPopMatrix();
}

int isCrashed(Airplane* airplane)
{
	return airplane->crashed;
}

Pose3D getAirplanePose(Airplane* airplane)
{
	return airplane->pose;
}

Vector3D getAirplaneVelocity(Airplane* airplane)
{
	return airplane->velocity;
}

Vector3D getAirplaneAcceleration(Airplane* airplane)
{
	return airplane->acceleration;
}

Orientation3D getAirplaneAngularVelocity(Airplane* airplane)
{
	return airplane->angularVelocity;
}

void setAirplanePose(Airplane* airplane, Pose3D pose)
{
	airplane->pose = pose;
}

void setAirplaneVelocity(Airplane* airplane, Vector3D velocity)
{
	airplane->velocity = velocity;
}

void setAirplaneAngularVelocity(Airplane* airplane, Orientation3D angularVelocity)
{
	airplane->angularVelocity = angularVelocity;
}

void setAirplaneRollVelocity(Airplane* airplane, double rollVelocity)
{
	airplane->angularVelocity.roll = rollVelocity;
}

void setAirplanePitchVelocity(Airplane* airplane, double pitchVelocity)
{
	airplane->angularVelocity.pitch = pitchVelocity;
}

void setAirplaneYawVelocity(Airplane* airplane, double yawVelocity)
{
	airplane->angularVelocity.yaw = yawVelocity;
}

void setAirplaneAcceleration(Airplane* airplane, Vector3D acceleration)
{
	airplane->acceleration = acceleration;
}

Airplane* createAirplane(Pose3D initialPose, Vector3D initialVelocity)
{
	Airplane* airplane = malloc(sizeof(Airplane));
	
	airplane->pose = initialPose;
	airplane->velocity = initialVelocity;
	
	airplane->acceleration.x = 0;
	airplane->acceleration.y = 0;
	airplane->acceleration.z = 0;
	
	airplane->angularVelocity.roll = 0;
	airplane->angularVelocity.pitch = 0;
	airplane->angularVelocity.yaw = 0;
	
	airplane->hit = 0;
	airplane->crashed = 0;
	
	return airplane;
}

double* createRotationMatrix(Orientation3D angles)
{
	double sinRoll 	= sin(angles.roll);
	double cosRoll	= cos(angles.roll);
	double sinPitch = sin(angles.pitch);
	double cosPitch = cos(angles.pitch);
	double sinYaw 	= sin(angles.yaw);
	double cosYaw 	= cos(angles.yaw);
	
	double* matrix = malloc(9*sizeof(double));
	matrix[0 + 3*0] = cosPitch*cosYaw;
	matrix[0 + 3*1] = sinRoll*sinPitch*cosYaw-cosRoll*sinYaw;
	matrix[0 + 3*2] = sinRoll*sinYaw + cosRoll*sinPitch*cosYaw;
	matrix[1 + 3*0] = cosPitch*sinYaw;
	matrix[1 + 3*1] = cosRoll*cosYaw + sinRoll*sinPitch*sinYaw;
	matrix[1 + 3*2] = -sinRoll*cosYaw + cosRoll*sinPitch*sinYaw;
	matrix[2 + 3*0] = -sinPitch;
	matrix[2 + 3*1] = sinRoll*cosPitch;
	matrix[2 + 3*2] = cosRoll*cosPitch; 
	
	return matrix;
}

void printMatrix(double* matrix)
{
	printf("% lf % lf % lf\n", matrix[0 + 3*0], matrix[0 + 3*1], matrix[0 + 3*2]);
	printf("% lf % lf % lf\n", matrix[1 + 3*0], matrix[1 + 3*1], matrix[1 + 3*2]);
	printf("% lf % lf % lf\n", matrix[2 + 3*0], matrix[2 + 3*1], matrix[2 + 3*2]);
	printf("\n");
}

Orientation3D rotateOrientation(Orientation3D initialOrientation, Orientation3D rotateBy)
{
	double* initialMatrix = createRotationMatrix(initialOrientation);
	double* rotationMatrix = createRotationMatrix(rotateBy);
	
	//printMatrix(initialMatrix);
	//printMatrix(rotationMatrix);
	
	double finalMatrix[9];
	
	int i;
	for(i=0; i<3; i++)
	{
		int j;
		for(j=0; j<3; j++)
		{
			finalMatrix[i + 3*j] = 0.0;
			int k;
			for(k=0; k<3; k++)
			{
				finalMatrix[i + 3*j] += initialMatrix[i + 3*k]*rotationMatrix[k + 3*j];
			}
		}
	}
	
	//printMatrix(finalMatrix);
	
	Orientation3D finalOrientation;
	finalOrientation.roll = atan2(finalMatrix[2 + 3*1], finalMatrix[2 + 3*2]);
	finalOrientation.pitch = -asin(finalMatrix[2 + 3*0]);
	finalOrientation.yaw = atan2(finalMatrix[1 + 3*0], finalMatrix[0 + 3*0]);
	
	//double pi = 3.141589;
	//printf("r: % lf p:% lf y:% lf\n\n", finalOrientation.roll*180.0/pi, finalOrientation.pitch*180.0/pi, finalOrientation.yaw*180.0/pi);

	free(initialMatrix);
	free(rotationMatrix);

	return finalOrientation;
}

void hitAirplane(Airplane* airplane, int selfPlaneNum, Bullet* bullets, int numBullets)
{
	Vector3D posPlane = airplane->pose.position;

	int i;
	for(i=0; i<numBullets; i++)
	{
		if(bullets[i].plane != selfPlaneNum)
		{
			Vector3D posBullet = bullets[i].position;
			Vector3D velBullet = bullets[i].velocity;
		
			int n = 4;
			int k;
			for(k=0; k<n; k++)
			{								
				double distx = posPlane.x - posBullet.x + k*velBullet.x/(double)n;;
				double disty = posPlane.y - posBullet.y + k*velBullet.y/(double)n;;
				double distz = posPlane.z - posBullet.z + k*velBullet.z/(double)n;;
		
				double dist = sqrt(distx*distx + disty*disty + distz*distz);
		
				if(dist < 1.5)
				{
					airplane->hit = 1;
				}
			}
		}
	}
}

void moveAirplane1(Airplane* airplane, long int delta_t)
{
	double pi = 3.141589;

	Orientation3D a;
	a.roll = 0.0;//pi/4.0;
	a.pitch = 0.0;//pi/2.0;
	a.yaw = pi/2.0;//pi/2.0;
	
	Orientation3D b;
	b.roll = 0.0;//pi/4.0;
	b.pitch = pi/4.0;// pi/4.0;
	b.yaw = 0.0;//pi/2.0;
	
	Orientation3D c = rotateOrientation(a,b);
	
	airplane->pose.orientation = c;

}

Vector3D getLiftAcceleration(Airplane* airplane)
{
	Vector3D lift;
	
	double liftFactor1 = -0.0;
	//double liftFactor2 = 0.0;
	
	lift.x = 0.0;
	lift.y = 0.0;
	lift.z = liftFactor1*airplane->velocity.x;// + liftFactor2*airplane->velocity.z*airplane->velocity.z;
	
	return lift;
}

void applyGravity(Airplane* airplane, long int delta_t)
{
	Orientation3D planeAngles = airplane->pose.orientation;
	planeAngles.roll = planeAngles.roll;
	planeAngles.pitch = planeAngles.pitch;
	planeAngles.yaw = planeAngles.yaw;	
	
	double* rotation = createRotationMatrix(planeAngles);
	
	Vector3D globalReferenceVelocity;
	globalReferenceVelocity.x = rotation[0 + 3*0]*airplane->velocity.x + rotation[0 + 3*1]*airplane->velocity.y + rotation[0 + 3*2]*airplane->velocity.z;
	globalReferenceVelocity.y = rotation[1 + 3*0]*airplane->velocity.x + rotation[1 + 3*1]*airplane->velocity.y + rotation[1 + 3*2]*airplane->velocity.z;
	globalReferenceVelocity.z = rotation[2 + 3*0]*airplane->velocity.x + rotation[2 + 3*1]*airplane->velocity.y + rotation[2 + 3*2]*airplane->velocity.z;

	
	double ground_paralallel_speed = sqrt(globalReferenceVelocity.x*globalReferenceVelocity.x + globalReferenceVelocity.y*globalReferenceVelocity.y);
	
	Vector3D gravityGlobal;
	
	//gravityGlobal.x = 0.0;
	//gravityGlobal.y = 0.0;
	gravityGlobal.z = -0.0001/(10.0*ground_paralallel_speed+1.0) + 0.000066667;
	
	if(gravityGlobal.z > 0.0)
	{
		gravityGlobal.z = 0.0;
	}	
	
	//globalReferenceVelocity.x += gravityGlobal.x * delta_t;
	//globalReferenceVelocity.y += gravityGlobal.y * delta_t;
	globalReferenceVelocity.z += gravityGlobal.z * delta_t;
	
	airplane->pose.orientation.pitch = -atan2(globalReferenceVelocity.z, ground_paralallel_speed);
		
	airplane->velocity.x = rotation[0 + 3*0]*globalReferenceVelocity.x + rotation[1 + 3*0]*globalReferenceVelocity.y + rotation[2 + 3*0]*globalReferenceVelocity.z;
	//airplane->velocity.y = rotation[0 + 3*1]*globalReferenceVelocity.x + rotation[1 + 3*1]*globalReferenceVelocity.y + rotation[2 + 3*1]*globalReferenceVelocity.z;
	//airplane->velocity.z = rotation[0 + 3*2]*globalReferenceVelocity.x + rotation[1 + 3*2]*globalReferenceVelocity.y + rotation[2 + 3*2]*globalReferenceVelocity.z;

	free(rotation);
}

Vector3D getDragAcceleration(Airplane* airplane)
{
	Vector3D drag;
	
	double dragFactor1 = -0.001;
	double dragFactor2 = -0.01;
	double dragFactor3 = -0.1;
	
	if(airplane->velocity.x < 0)
	{
		dragFactor1 = -dragFactor1;
	}
	if(airplane->velocity.y < 0)
	{
		dragFactor2 = -dragFactor2;
	}
	if(airplane->velocity.z < 0)
	{
		dragFactor3 = -dragFactor3;
	}
	
	drag.x = dragFactor1 * airplane->velocity.x * airplane->velocity.x;
	drag.y = dragFactor2 * airplane->velocity.y * airplane->velocity.y;
	drag.z = dragFactor3 * airplane->velocity.z * airplane->velocity.z;
	
	return drag;
}

void moveAirplane(Airplane* airplane, long int delta_t)
{		
	Orientation3D angularVelocityRotation;
	angularVelocityRotation.roll  = airplane->angularVelocity.roll  * delta_t;
	angularVelocityRotation.pitch = airplane->angularVelocity.pitch  * delta_t;
	angularVelocityRotation.yaw   = airplane->angularVelocity.yaw  * delta_t;
	
	Orientation3D newOrientation = rotateOrientation(airplane->pose.orientation, angularVelocityRotation);
	
	airplane->pose.orientation = newOrientation;
			
		
	Vector3D planeReferenceAcc;
	
	planeReferenceAcc = airplane->acceleration;
	
	if(airplane->hit == 1)
	{
		planeReferenceAcc.x = 0.0;
		planeReferenceAcc.y = 0.0;
		planeReferenceAcc.z = 0.0;
	}
	
	Vector3D liftAcc = getLiftAcceleration(airplane);
	planeReferenceAcc.x += liftAcc.x;
	planeReferenceAcc.y += liftAcc.y;
	planeReferenceAcc.z += liftAcc.z;
	
	//Vector3D gravity = getGravityAcceleration(airplane);

	//planeReferenceAcc.x += gravity.x;
	//planeReferenceAcc.y += gravity.y;
	//planeReferenceAcc.z += gravity.z;
	
	Vector3D drag = getDragAcceleration(airplane);

	planeReferenceAcc.x += drag.x;
	planeReferenceAcc.y += drag.y;
	planeReferenceAcc.z += drag.z;

	airplane->velocity.x += planeReferenceAcc.x * delta_t;
	airplane->velocity.y += planeReferenceAcc.y * delta_t;
	airplane->velocity.z += planeReferenceAcc.z * delta_t;
	
	applyGravity(airplane, delta_t);
	
	Orientation3D planeAngles = airplane->pose.orientation;
	planeAngles.roll = planeAngles.roll;
	planeAngles.pitch = planeAngles.pitch;
	planeAngles.yaw = planeAngles.yaw;	
	
	double* rotation = createRotationMatrix(planeAngles);

	Vector3D globalReferenceVelocity;
	globalReferenceVelocity.x = rotation[0 + 3*0]*airplane->velocity.x + rotation[0 + 3*1]*airplane->velocity.y + rotation[0 + 3*2]*airplane->velocity.z;
	globalReferenceVelocity.y = rotation[1 + 3*0]*airplane->velocity.x + rotation[1 + 3*1]*airplane->velocity.y + rotation[1 + 3*2]*airplane->velocity.z;
	globalReferenceVelocity.z = rotation[2 + 3*0]*airplane->velocity.x + rotation[2 + 3*1]*airplane->velocity.y + rotation[2 + 3*2]*airplane->velocity.z;
	free(rotation);
		
	airplane->pose.position.x += globalReferenceVelocity.x * delta_t;
	airplane->pose.position.y += globalReferenceVelocity.y * delta_t;
	airplane->pose.position.z += globalReferenceVelocity.z * delta_t;
	
	if(airplane->pose.position.z < 0.0 || airplane->crashed)
	{
		airplane->pose.position.z = 0.0;
		airplane->velocity.x = 0.0;
		airplane->velocity.y = 0.0;
		airplane->velocity.z = 0.0;
		
		airplane->crashed = 1;
		
		//airplane->velocity.x -= rotation[1 + 3*0]*airplane->velocity.y;
		//airplane->velocity.y -= rotation[1 + 3*1]*airplane->velocity.y;
		//airplane->velocity.z -= rotation[1 + 3*2]*airplane->velocity.y;
	}

	
	
	//printf("x:% lf y:% lf z:% lf\n",airplane->pose.position.x, airplane->pose.position.y, airplane->pose.position.z);
	//printf("x:% lf y:% lf z:% lf\n",airplane->velocity.x, airplane->velocity.y, airplane->velocity.z);
	//printf("x:% lf y:% lf z:% lf\n",planeReferenceAcc.x, planeReferenceAcc.y, planeReferenceAcc.z);	
	//printf("\n");
}

void deleteAirplane(Airplane* plane)
{
	free(plane);
}
