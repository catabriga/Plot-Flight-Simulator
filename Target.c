#include "Target.h"

struct Targets
{
	Vector3D* pos;
	int* destroyed;
	int numTargets;
};


Targets* createTargets(void)
{

	Targets* targets = malloc(sizeof(Targets));

	targets->numTargets = 512;
	
	targets->pos = malloc(targets->numTargets * sizeof(Vector3D));
	targets->destroyed = malloc(targets->numTargets * sizeof(int));
	
	srand(getCurrentTime());
	int i;
	for(i=0; i<targets->numTargets; i++)
	{
		Vector3D pos;
		
		pos.x = rand()%2000 - 1000.0;
		pos.y = rand()%2000 - 1000.0;
		
		if(rand()%2 == 0)
		{
			pos.z = rand()%2000;
		}
		else
		{
			pos.z = 1.0;
		}
		
		targets->pos[i] = pos;
		targets->destroyed[i] = 0;
	}
	
	return targets;

}

static void drawTarget(void)
{
	glPushMatrix();
	
		glBegin(GL_QUADS);
		
			glVertex3f(-1.0,-1.0, 0.0);
			glVertex3f(-1.0, 1.0, 0.0);
			glVertex3f( 1.0, 1.0, 0.0);
			glVertex3f( 1.0,-1.0, 0.0);
			
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(-1.0, 0.0, 1.0);
			glVertex3f( 1.0, 0.0, 1.0);
			glVertex3f( 1.0, 0.0,-1.0);
			
			glVertex3f( 0.0,-1.0,-1.0);
			glVertex3f( 0.0,-1.0, 1.0);
			glVertex3f( 0.0, 1.0, 1.0);
			glVertex3f( 0.0, 1.0,-1.0);
						
		glEnd();
	
	glPopMatrix();
}

void destroyTargets(Targets* targets, Bullet* bullets, int numBullets)
{
	int i;
	for(i=0; i<targets->numTargets; i++)
	{
		if(targets->destroyed[i] == 0)
		{
			Vector3D posTarget = targets->pos[i];
		
			int j;
			for(j=0; j<numBullets; j++)
			{
				Vector3D posBullet = bullets[j].position;
				Vector3D velBullet = bullets[j].velocity;
				
				int n = 4;
				int k;
				for(k=0; k<n; k++)
				{						
					double distx = posTarget.x - posBullet.x + k*velBullet.x/(double)n;
					double disty = posTarget.y - posBullet.y + k*velBullet.y/(double)n;
					double distz = posTarget.z - posBullet.z + k*velBullet.z/(double)n;
			
					double dist = sqrt(distx*distx + disty*disty + distz*distz);
			
					if(dist < 1.0)
					{
						targets->destroyed[i] = 1;
					}
				}
			}		
		}	
	}
}

void drawTargets(Targets* targets)
{
	int i;
	for(i=0; i<targets->numTargets; i++)
	{
		if(targets->destroyed[i] == 0)
		{
			glPushMatrix();
		
				Vector3D pos = targets->pos[i];
		
				glTranslatef(pos.x, pos.y, pos.z);
		
				if(pos.z < 2.0)
				{
					glColor3f(0.1, 0.1, 0.7);
				}
				else
				{
					glColor3f(1.0, 1.0, 0.8);	
				}
		
				drawTarget();
		
			glPopMatrix();
		}
	}


}
