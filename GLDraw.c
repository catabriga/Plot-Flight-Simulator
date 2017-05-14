#include "GLDraw.h"

void initGl(void)
{
	int zero = 0;
	glutInit(&zero, NULL);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);        
	glDepthFunc(GL_LESS);     
	glEnable(GL_DEPTH_TEST);  
	glShadeModel(GL_SMOOTH);     

	GLfloat light_ambient[]= { 0.5f, 0.5f, 0.5f, 1.0f };                 
	GLfloat light_diffuse[]= { 0.4f, 0.4f, 0.3f, 1.0f };                        
	GLfloat light_position[]= { 0.0f, 100.0f, 200.0f, 100.0f };        

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);                        
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);                
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);        
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);           

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();     

	gluPerspective(45.0f,600.0/600.0,0.1f,10000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	
}

