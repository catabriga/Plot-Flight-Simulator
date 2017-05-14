#include "Panel.h"

void drawSpeedometer(GameState *gameState)
{
	double maxVelocity = 0.2;
	double radiusSpeedometer = 0.16;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Vector3D airplaneVelocity = getAirplaneVelocity(airplane[selfAirplaneNumber]);
	double velocity = sqrt(airplaneVelocity.x*airplaneVelocity.x+airplaneVelocity.y*airplaneVelocity.y+airplaneVelocity.z*airplaneVelocity.z);
	double teta = 360*velocity/maxVelocity+90;

	glPushMatrix();
		glLoadIdentity();
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(1.0f,-1.0f,-3.0f);
		glPushMatrix();//ponteiro
			glRotatef(-teta,0.0f,0.0f,1.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(radiusSpeedometer*0.8,0.0f,0.0f);
			glEnd();
		glPopMatrix();
		glPushMatrix();//circulo
			glBegin(GL_LINE_LOOP);
				int i;
				double pi = 3.1415;
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}

void drawHorizon (GameState *gameState)
{
	double radiusSpeedometer = 0.16;
	double pi = 3.141589;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Pose3D airplanePose = getAirplanePose(airplane[selfAirplaneNumber]);

	double teta = airplanePose.orientation.roll*180/pi;

	glPushMatrix();
		glLoadIdentity();
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(-1.0f,-1.0f,-3.0f);
		glRotatef(-teta,0.0f,0.0f,1.0f);
			//ponteiro
			glBegin(GL_LINES);
				glVertex3f(-radiusSpeedometer,0.0f,0.0f);
				glVertex3f(radiusSpeedometer,0.0f,0.0f);
			glEnd();
			//circulo
			glBegin(GL_LINE_LOOP);
				int i;
				glColor3f(1.0f,0.0f,0.0f);
				for(i = 0; i < 179; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
				glColor3f(1.0f,1.0f,1.0f);
				for(; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
	glPopMatrix();
}

void drawEmbication(GameState *gameState)
{
	double radiusSpeedometer = 0.16;
	double pi = 3.141589;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Pose3D airplanePose = getAirplanePose(airplane[selfAirplaneNumber]);

	double teta = airplanePose.orientation.pitch*180/pi;

	glPushMatrix();
		glLoadIdentity();
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(-0.5f,-1.0f,-3.0f);
		glPushMatrix();//ponteiro
			glRotatef(teta,0.0f,0.0f,1.0f);
			glBegin(GL_LINES);
				glVertex3f(-radiusSpeedometer,0.0f,0.0f);
				glVertex3f(radiusSpeedometer,0.0f,0.0f);
			glEnd();
		glPopMatrix();
		glPushMatrix();//circulo
			glBegin(GL_LINE_LOOP);
				int i;
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}

void drawAltimeter(GameState *gameState)
{
	double radiusSpeedometer = 0.16;
	double pi = 3.141589;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Pose3D airplanePose = getAirplanePose(airplane[selfAirplaneNumber]);

	double maxAltitude = 5000;
	double altitude = airplanePose.position.z;
	double teta = 360*altitude/maxAltitude+90;

	glPushMatrix();
		glLoadIdentity();
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(-1.0f,1.0f,-3.0f);
		glPushMatrix();//ponteiro
			glRotatef(-teta,0.0f,0.0f,1.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(radiusSpeedometer*0.8,0.0f,0.0f);
			glEnd();
		glPopMatrix();
		glPushMatrix();//circulo
			glBegin(GL_LINE_LOOP);
				int i;
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}

void drawRadar(GameState *gameState)
{
	double radiusSpeedometer = 0.16;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Pose3D airplanePose = getAirplanePose(airplane[selfAirplaneNumber]);

	double maxDist = 500;
	double selfX = airplanePose.position.x;
	double selfY = airplanePose.position.y;
	double selfZ = airplanePose.position.z;

	double yaw = airplanePose.orientation.yaw;
	double pi = 3.141589;

	glPushMatrix();
		glPointSize(2.0f);
		glLoadIdentity();
		glTranslatef(1.0f,1.0f,-3.0f);
		glPushMatrix();//ponteiro
			glRotatef(-yaw*180/pi,0.0f,0.0f,1.0f);
			int i, numAirplanes = getNumAirplanes(gameState);
			
			glPointSize(3.0f);
			for(i=0;i<numAirplanes;i++)
			{
				double x = (getAirplanePose(airplane[i]).position.x-selfX);
				double y = -(getAirplanePose(airplane[i]).position.y-selfY);
				double z = getAirplanePose(airplane[i]).position.z-selfZ;
				double dist = sqrt(x*x+y*y);
				
				if(isCrashed(airplane[i]))
				{
					glColor3f(1.0f,0.0f,0.0f);
				}
				else
				{
					glColor3f(0.0f,0.0f,1.0f);
				}
				
				
				if(z>=0)
				{
					glBegin(GL_POINTS);
						if(dist<=maxDist)
						{
							glVertex3f(y*radiusSpeedometer/(maxDist),x*radiusSpeedometer/(maxDist),0.0f);
						}					
						else
						{
							glVertex3f(y*radiusSpeedometer/dist,x*radiusSpeedometer/dist,0.0f);
						}
					glEnd();
				}
				else
				{
					double scale=0.05;
					glBegin(GL_LINES);
						if(dist<=maxDist)
						{
							glVertex3f(y*radiusSpeedometer/(maxDist)-scale*radiusSpeedometer,x*radiusSpeedometer/(maxDist)-scale*radiusSpeedometer ,0.0f);
							glVertex3f(y*radiusSpeedometer/(maxDist)+scale*radiusSpeedometer,x*radiusSpeedometer/(maxDist)+scale*radiusSpeedometer,0.0f);
							glVertex3f(y*radiusSpeedometer/(maxDist)-scale*radiusSpeedometer,x*radiusSpeedometer/(maxDist)+scale*radiusSpeedometer,0.0f);
							glVertex3f(y*radiusSpeedometer/(maxDist)+scale*radiusSpeedometer,x*radiusSpeedometer/(maxDist)-scale*radiusSpeedometer,0.0f);
						}					
						else
						{
							glVertex3f(y*radiusSpeedometer/dist-scale*radiusSpeedometer,x*radiusSpeedometer/dist-scale*radiusSpeedometer,0.0f);
							glVertex3f(y*radiusSpeedometer/dist+scale*radiusSpeedometer,x*radiusSpeedometer/dist+scale*radiusSpeedometer,0.0f);
							glVertex3f(y*radiusSpeedometer/dist-scale*radiusSpeedometer,x*radiusSpeedometer/dist+scale*radiusSpeedometer,0.0f);
							glVertex3f(y*radiusSpeedometer/dist+scale*radiusSpeedometer,x*radiusSpeedometer/dist-scale*radiusSpeedometer,0.0f);
						}
					glEnd();
				}
			}
		glPopMatrix();
		glPushMatrix();//circulo
			glLineWidth(2.0f);
			glColor3f(1.0f,1.0f,1.0f);
			glBegin(GL_LINE_LOOP);
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
			glLineWidth(1.0f);
			glBegin(GL_LINE_LOOP);
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0)*0.5,radiusSpeedometer*sin(i*pi/180)*0.5,0.0f);
				}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}

void drawCompass(GameState *gameState)
{
	double radiusSpeedometer = 0.16;
	double pi = 3.141589;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Pose3D airplanePose = getAirplanePose(airplane[selfAirplaneNumber]);

	double teta = airplanePose.orientation.yaw*180/pi-90;

	glPushMatrix();
		glLoadIdentity();
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(0.0f,-1.0f,-3.0f);
		glPushMatrix();//ponteiro
			glRotatef(-teta,0.0f,0.0f,1.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(radiusSpeedometer*0.8,0.0f,0.0f);
			glEnd();
		glPopMatrix();
		glPushMatrix();//circulo
			glBegin(GL_LINE_LOOP);
				int i;
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}
/*
void drawCompass(GameState *gameState)
{
	double radiusSpeedometer = 0.16;
	double pi = 3.141589;

	Airplane** airplane = getListAirplane(gameState);
	int selfAirplaneNumber = getSelfAirplaneNumber(gameState);
	Pose3D airplanePose = getAirplanePose(airplane[selfAirplaneNumber]);

	double teta = airplanePose.orientation.yaw*180/pi;

	glPushMatrix();
		glLoadIdentity();
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(-1.5f,-1.0f,-3.0f);
		glPushMatrix();//ponteiro
			glRotatef(teta,0.0f,0.0f,1.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(radiusSpeedometer*0.8,0.0f,0.0f);
			glEnd();
		glPopMatrix();
		glPushMatrix();//circulo
			glBegin(GL_LINE_LOOP);
				int i;
				for(i = 0; i < 359; i++)
				{
					glVertex3f(radiusSpeedometer*cos(i*pi/180.0),radiusSpeedometer*sin(i*pi/180),0.0f);
				}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}*/
