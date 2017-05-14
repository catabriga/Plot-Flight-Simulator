#include "Terrain.h"

static void drawTree(void)
{
	glPushMatrix();
	
		glColor3f(0.0f,0.5f,0.0f);
		
		glBegin(GL_TRIANGLES);
			
			glVertex3f(-3.0,0.0,0.0);
			glVertex3f(3.0,0.0,0.0);
			glVertex3f(0.0,0.0,20.0);
			
			glVertex3f(0.0,3.0,0.0);
			glVertex3f(0.0,-3.0,0.0);
			glVertex3f(0.0,0.0,20.0);		
		
		glEnd();
	
	glPopMatrix();
}

static void drawTrees(void)
{
	static Vector3D* trees = NULL;
	static int numTrees = 0;
	
	if(trees == NULL)
	{
		numTrees = 1000;
		trees = malloc(numTrees*sizeof(Vector3D));
	
		srand(getCurrentTime());
		
		int i;
		for(i=0; i<numTrees; i++)
		{
			Vector3D vec;
			
			vec.x = rand()%2000 - 1000.0;
			vec.y = rand()%2000 - 1000.0;
			vec.z = 0.0;
			
			trees[i] = vec;			
		}
	
	}
	
	glPushMatrix();
	
		int i;
		for(i=0; i<numTrees; i++)
		{
			glPushMatrix();
				
				Vector3D tree = trees[i];
			
				glTranslatef(tree.x, tree.y, tree.z);
				drawTree();		
						
			glPopMatrix();
		}
	
	glPopMatrix();
}

void drawLand(void)
{
	glPushMatrix();
	
		glColor3f(0.57f,0.29f,0.0f);
		int i;	
		glBegin(GL_QUADS);
					
			glVertex3f(-1000.0,-1000.0, 0.0);
			glVertex3f(-1000.0,1000.0, 0.0);
			glVertex3f(1000.0,1000.0, 0.0);
			glVertex3f(1000.0,-1000.0, 0.0);
				
		glEnd();
	
		glLineWidth(2.0);
		glColor3f(0.0f,0.0f,0.0f);
			
		glBegin(GL_LINES);
		
			for(i = -10; i < 10; i++)
			{						
				glVertex3f(-1000.0, 100*i, 0.001);
				glVertex3f( 1000.0, 100*i, 0.001);
				
				glVertex3f( 100*i, -1000.0, 0.001);
				glVertex3f( 100*i,  1000.0, 0.001);
			}
		glEnd();

	glPopMatrix();

}

void drawSineLand(void)
{
	glPushMatrix();
	
		glColor3f(0.0f,0.5f,0.5f);
		int i;	
		glBegin(GL_QUADS);
		
			for(i = -10; i < 10; i++)
			{		
				int j;
				for(j = -10; j < 10; j++)
				{			
					glVertex3f(1000*i		, 1000*j			, 50*sin((i+j)/2.0f) + 50.0);
					glVertex3f(1000*(i+1.0)	, 1000*j			, 50*sin((i+j+1.0)/2.0f) + 50.0);					
					glVertex3f(1000*(i+1.0)	, 1000*(j+1.0)	, 50*sin((i+j+2.0)/2.0f) + 50.0);
					glVertex3f(1000*i		, 1000*(j+1.0)	, 50*sin((i+j+1.0)/2.0f) + 50.0);
				}
			}
		glEnd();
	
		glLineWidth(2.0);
		glColor3f(0.0f,0.0f,0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
			
		glBegin(GL_QUADS);
		
			for(i = -10; i < 10; i++)
			{		
				int j;
				for(j = -10; j < 10; j++)
				{			
					glVertex3f(1000*i		, 1000*j			, 50*sin((i+j)/2.0f) + 50.0);
					glVertex3f(1000*(i+1.0)	, 1000*j			, 50*sin((i+j+1.0)/2.0f) + 50.0);					
					glVertex3f(1000*(i+1.0)	, 1000*(j+1.0)	, 50*sin((i+j+2.0)/2.0f) + 50.0);
					glVertex3f(1000*i		, 1000*(j+1.0)	, 50*sin((i+j+1.0)/2.0f) + 50.0);
				}
			}
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	glPopMatrix();
}

void drawTerrain(void)
{	
	glPushMatrix();
	
		drawLand();
		drawTrees();	
		
	glPopMatrix();	
}
