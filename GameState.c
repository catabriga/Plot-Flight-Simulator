#include "GameState.h"

struct GameState
{	
	Airplane** airplanes;
	int numAirplanes;
	int maxNumAirplanes;
	int selfAirplaneNumber;
	int firing;
	
	Bullet* bullets;
	int numBullets;
	int lastBullet;
	
	Targets* targets;

	ReceiverSocket* receiverSocket;
	SenderSocket*	senderServerSocket;
	
	SenderSocket** senderClientsSocket;
	int numClients;
	int maxNumClients;
		
	int isHost;
	
	int camera;
};

Airplane** getListAirplane(GameState* gameState)
{
	return gameState->airplanes;
}

int getSelfAirplaneNumber(GameState* gameState)
{
	return gameState->selfAirplaneNumber;
}

int getNumAirplanes(GameState* gameState)
{
	return gameState->numAirplanes;
}

static void joinHost(GameState* gameState, unsigned short clientPort)
{
	char data[3];
	data[0] = NEW_PLAYER_PACKET;
	
	data[1] = (char)((clientPort >> 8) & 0x00FF);
	data[2] = (char)(clientPort & 0x00FF);
	
	sendPacket(gameState->senderServerSocket, data, 3);
}

static void addPlane(GameState* gameState)
{
	if(gameState->numAirplanes < gameState->maxNumAirplanes)
	{
		Pose3D initialPose;
		Vector3D initialVelocity;
		
		initialPose.position.x = 0.0;//(double)(rand()%100) - 50;
		initialPose.position.y = 0.0;//(double)(rand()%100) - 50;
		initialPose.position.z = 200.0;//(double)(rand()%100) - 50;
		initialPose.orientation.roll = 0.0;//3.1415/4.0;
		initialPose.orientation.pitch = 0.0;//-3.1415/4.0;			
		initialPose.orientation.yaw = 0.0;//3.1415/4.0;
				
		initialVelocity.x = 0.05;//(double)(rand()%100)/10000.0;
		initialVelocity.y = 0.0;//(double)(rand()%100)/10000.0;
		initialVelocity.z = 0.0;//(double)(rand()%100)/10000.0;
		
		setAirplanePose(gameState->airplanes[gameState->numAirplanes], initialPose);
		setAirplaneVelocity(gameState->airplanes[gameState->numAirplanes], initialVelocity);
		
		Orientation3D o3D;
		o3D.roll = 0.0;//(double)(rand()%100)/1000000.0;
		o3D.pitch = 0.0;//-(double)(rand()%100)/1000000.0;
		o3D.yaw = 0.0;//(double)(rand()%100)/1000000.0;

		setAirplaneAngularVelocity(gameState->airplanes[gameState->numAirplanes], o3D);

		gameState->numAirplanes = gameState->numAirplanes + 1;
	}
}

GameState* createGameState(unsigned short hostPort, char* hostIpAddress, unsigned short clientPort)
{
	GameState* gameState = malloc(sizeof(GameState));

	gameState->maxNumAirplanes = MAX_NUM_AIRPLANES;
	gameState->numAirplanes = 0;
	gameState->selfAirplaneNumber = -1;
	gameState->airplanes = malloc(gameState->maxNumAirplanes*sizeof(Airplane*));
	gameState->firing = 0;

	gameState->numBullets = MAX_NUM_BULLETS;
	gameState->bullets = malloc(gameState->numBullets*sizeof(Bullet));
	gameState->lastBullet = 0;
	
	gameState->targets = createTargets();
	
	gameState->camera = 0;
	
	Vector3D zeroVec;
	zeroVec.x = 0;
	zeroVec.y = 0;
	zeroVec.z = 0;
	
	int i;
	for(i=0; i<gameState->numBullets; i++)
	{
		gameState->bullets[i].position = zeroVec;
		gameState->bullets[i].velocity = zeroVec;
		gameState->bullets[i].active = 0;
		
	}
	
	for(i=0; i<gameState->maxNumAirplanes; i++)
	{
		Pose3D pose;
		Vector3D vel;
		
		gameState->airplanes[i] = createAirplane(pose, vel);
	}

	gameState->maxNumClients = MAX_NUM_CLIENTS;
	gameState->senderClientsSocket = NULL;
	gameState->numClients = 0;
	
	if(hostIpAddress == NULL)	// If IP address is null we will host the game
	{
		gameState->receiverSocket = createReceiverSocket(hostPort);
		gameState->senderServerSocket = NULL;
		
		gameState->senderClientsSocket = malloc(gameState->maxNumClients*sizeof(SenderSocket*));
		
		for(i=0; i<gameState->maxNumClients; i++)
		{
			gameState->senderClientsSocket[i] = NULL;
		}
			
		gameState->isHost = 1;
			
		printf("\nHosting the game at port %d\n", hostPort);

		srand(getCurrentTime());
		addPlane(gameState);
		//addPlane(gameState);
		//addPlane(gameState);
		gameState->selfAirplaneNumber = 0;
			
	}
	else	// We will connect to a host
	{
		gameState->receiverSocket = createReceiverSocket(clientPort);
		gameState->senderServerSocket = createSenderSocketString(hostIpAddress, hostPort);

		gameState->senderClientsSocket = NULL;	
	
		gameState->isHost = 0;
		
		joinHost(gameState, clientPort);
	}

	return gameState;
}

static void addNewPlayer(GameState* gameState, unsigned char* data, unsigned int ipAddress, unsigned short port)
{
	if(gameState->isHost)
	{
		if(gameState->numClients < gameState->maxNumClients)
		{
			unsigned short remoteReceiverPort = (data[1] << 8) + data[2];
		
			gameState->senderClientsSocket[gameState->numClients] = createSenderSocket(ipAddress, remoteReceiverPort);
			gameState->numClients = gameState->numClients + 1;
			
			addPlane(gameState);
			
			printf("New player joined from %d listening at port %d.\n", ipAddress, remoteReceiverPort);
		}
		else
		{
			printf("Someone failed to join the game. The game is full.\n");		
		}	
	}
	else
	{
		printf("Someone failed to join the game. You are not host.\n");		
	} 
}

static void updateStateWithPacket(GameState* gameState, unsigned char* data, unsigned int ipAddress, unsigned short port)
{
	if(gameState->isHost)
	{		
		printf("%d tried to cheat sending a fakdestroyTargetse update packet.\n", ipAddress);
			
	}
	else
	{
		StateUpdatePacket* stateUpdatePacket = (StateUpdatePacket*)(data);
		
		
		gameState->numAirplanes = stateUpdatePacket->numPlanes;
				
		int i;
		for(i=0; i<gameState->numAirplanes; i++)
		{
			if(i != gameState->selfAirplaneNumber || gameState->selfAirplaneNumber != stateUpdatePacket->selfPlane)
			{
				setAirplanePose(gameState->airplanes[i], stateUpdatePacket->planesPoses[i]);
				setAirplaneVelocity(gameState->airplanes[i], stateUpdatePacket->planesVelocities[i]);
				setAirplaneAcceleration(gameState->airplanes[i], stateUpdatePacket->planesAccelerations[i]);		
				setAirplaneAngularVelocity(gameState->airplanes[i], stateUpdatePacket->planesAngularVelocities[i]);	
			}
		}
		
		gameState->selfAirplaneNumber = stateUpdatePacket->selfPlane;
	} 
}

void updateClientMovementPacket(GameState* gameState, unsigned char* data, unsigned int ipAddress, unsigned short port)
{
	if(gameState->isHost)
	{		
		ClientMovementPacket* clientMovementPacket = (ClientMovementPacket*)(data);
		
		int planeNum = clientMovementPacket->planeNum;
		
		if(planeNum >= 0 && planeNum < gameState->numAirplanes)
		{				
			setAirplanePose(gameState->airplanes[planeNum], clientMovementPacket->planePose);
			setAirplaneVelocity(gameState->airplanes[planeNum], clientMovementPacket->planeVelocity);
			setAirplaneAcceleration(gameState->airplanes[planeNum], clientMovementPacket->planeAcceleration);		
			setAirplaneAngularVelocity(gameState->airplanes[planeNum], clientMovementPacket->planeAngularVelocity);	
		}
		else
		{
			printf("Invalid planeNum received from %u.\n", ipAddress);
		}	
	}
	else
	{
		printf("This client received a client movement packet. This should not happen.\n");
	}
}

void updateCreateBulletPacket(GameState* gameState, unsigned char* data, unsigned int ipAddress, unsigned short port)
{
	BulletPacket* bulletPacket = (BulletPacket*)(data);

	if(gameState->isHost)
	{					
		int i;
		for(i=0; i<gameState->numClients; i++)
		{			
			sendPacket(gameState->senderClientsSocket[i], (char*)(&bulletPacket), sizeof(BulletPacket));			
		}
	}
	
	Bullet bullet;
	bullet.active = 1;
	bullet.position = bulletPacket->bulletPosition;
	bullet.velocity = bulletPacket->bulletVelocity;
	
	gameState->bullets[gameState->lastBullet] = bullet;
	
	gameState->lastBullet = gameState->lastBullet + 1;
	
	if(gameState->lastBullet >= gameState->numBullets)
	{
		gameState->lastBullet = 1;
	}	
	
}

void printPacketBytes(unsigned char* data, int size)
{
	printf("data = ");
	int i;
	for(i=0; i<size; i++)
	{
		 printf("%x ", data[i]);
	}
	printf("\n");
}

void printPacketInfo(unsigned char* data, int size, unsigned int ipAddress, unsigned short port)
{
	printf("Received %d bytes from port %u from ip %u\n", size, port,ipAddress);
	//printPacketBytes(data, size);
}

static void processReceivedPackets(GameState* gameState)
{
	unsigned char data[MAX_PACKET_SIZE];
	int size;
	unsigned int ipAddress;
	unsigned short port;
	
	size = receivePacket(gameState->receiverSocket, data, MAX_PACKET_SIZE, &ipAddress, &port);
	
	if(size > 0)
	{
		//printPacketInfo(data, size, ipAddress, port);		
		
		char packetType = data[0];
		
		switch(packetType)
		{
			case NEW_PLAYER_PACKET:
			{
				addNewPlayer(gameState, data, ipAddress, port);
			}break;
			
			case STATE_UPDATE_PACKET:
			{
				updateStateWithPacket(gameState, data, ipAddress, port);
			}break;
		
				case CLIENT_MOVEMENT_PACKET:
			{
				updateClientMovementPacket(gameState, data, ipAddress, port);
			}break;
		
			case CREATE_BULLET_PACKET:
			{
				updateCreateBulletPacket(gameState, data, ipAddress, port);
			}break; 
		}
		
	}
}


static void moveBullets(GameState* gameState,  long int delta_t)
{
	int i;
	for(i=0; i<gameState->numBullets; i++)
	{
		if(gameState->bullets[i].active == 1)
		{
			Vector3D pos = gameState->bullets[i].position;
			Vector3D vel = gameState->bullets[i].velocity;
		
			pos.x += vel.x;
			pos.y += vel.y;
			pos.z += vel.z;
		
			gameState->bullets[i].position = pos;
		}
	}		
}


static void sendBulletPacket(GameState* gameState, Bullet bullet)
{
	BulletPacket bulletPacket;
			
	bulletPacket.packetType = CREATE_BULLET_PACKET;
		
	bulletPacket.bulletPosition = bullet.position;
	bulletPacket.bulletVelocity = bullet.velocity;
	
	if(gameState->isHost)
	{
		int i;
		for(i=0; i<gameState->numClients; i++)
		{			
			sendPacket(gameState->senderClientsSocket[i], (char*)(&bulletPacket), sizeof(BulletPacket));			
		}
	}
	else
	{
		sendPacket(gameState->senderServerSocket, (char*)(&bulletPacket), sizeof(BulletPacket));
	}
}

static void fireBullet(GameState* gameState)
{
	static long int lastBulletTime = 0;
	if(lastBulletTime == 0)
	{		
		lastBulletTime = getCurrentTime();
	}
	
	if(getCurrentTime() - lastBulletTime < 100)
	{
		return;
	}	
	
	if(gameState->firing == 0)
	{
		return;
	}	
	

	if(gameState->selfAirplaneNumber == -1)
	{
		return;
	}
	
	lastBulletTime = getCurrentTime();

	
	Airplane* selfPlane = gameState->airplanes[gameState->selfAirplaneNumber];

	Bullet bullet;
	
	Orientation3D angles = getAirplanePose(selfPlane).orientation;
	//double sinRoll 	= sin(angles.roll);
	//double cosRoll	= cos(angles.roll);
	double sinPitch = sin(angles.pitch);
	double cosPitch = cos(angles.pitch);
	double sinYaw 	= sin(angles.yaw);
	double cosYaw 	= cos(angles.yaw);
	
	double* rotation = malloc(9*sizeof(double));
	rotation[0 + 3*0] = cosPitch*cosYaw;
	//rotation[0 + 3*1] = sinRoll*sinPitch*cosYaw-cosRoll*sinYaw;
	//rotation[0 + 3*2] = sinRoll*sinYaw + cosRoll*sinPitch*cosYaw;
	rotation[1 + 3*0] = cosPitch*sinYaw;
	//rotation[1 + 3*1] = cosRoll*cosYaw + sinRoll*sinPitch*sinYaw;
	//rotation[1 + 3*2] = -sinRoll*cosYaw + cosRoll*sinPitch*sinYaw;
	rotation[2 + 3*0] = -sinPitch;
	//rotation[2 + 3*1] = sinRoll*cosPitch;
	//rotation[2 + 3*2] = cosRoll*cosPitch;
	
	Vector3D planePos = getAirplanePose(selfPlane).position;
		
	bullet.position = planePos;
	
	double bulletVel = 6.0;
	
	bullet.velocity.x = rotation[0 + 3*0]*bulletVel + (rand()%1000 - 500)/15000.0;
	bullet.velocity.y = rotation[1 + 3*0]*bulletVel + (rand()%1000 - 500)/15000.0;
	bullet.velocity.z = rotation[2 + 3*0]*bulletVel + (rand()%1000 - 500)/15000.0;

	bullet.active = 1;
	
	bullet.plane = gameState->selfAirplaneNumber;
	
	gameState->bullets[gameState->lastBullet] = bullet;
	
	gameState->lastBullet = gameState->lastBullet + 1;
	
	if(gameState->lastBullet >= gameState->numBullets)
	{
		gameState->lastBullet = 1;
	}
	
	sendBulletPacket(gameState, bullet);
}


static void advanceState(GameState* gameState, long int delta_t)
{
	int i;
	for(i=0; i<gameState->numAirplanes; i++)
	{
		moveAirplane(gameState->airplanes[i], delta_t);	
		
		if(i == gameState->selfAirplaneNumber)
		{
			hitAirplane(gameState->airplanes[i], gameState->selfAirplaneNumber, gameState->bullets, gameState->numBullets);
		}
	}	
	
	moveBullets(gameState, delta_t);
	destroyTargets(gameState->targets, gameState->bullets, gameState->numBullets);
	fireBullet(gameState);
}

static void sendCurrentState(GameState* gameState)
{
	static long lastSentTime = 0;
		
	if(lastSentTime == 0)
	{
		lastSentTime = getCurrentTime();
	}
	
	if(getCurrentTime() - lastSentTime > 100)
	{	
		lastSentTime = getCurrentTime();
		
		if(gameState->isHost)
		{
			StateUpdatePacket stateUpdatePacket;
	
			stateUpdatePacket.packetType = STATE_UPDATE_PACKET;
			stateUpdatePacket.numPlanes = gameState->numAirplanes;
	
			int i;
			for(i=0; i<gameState->numAirplanes; i++)
			{
				stateUpdatePacket.planesPoses[i] = getAirplanePose(gameState->airplanes[i]);
				stateUpdatePacket.planesVelocities[i] = getAirplaneVelocity(gameState->airplanes[i]);
				stateUpdatePacket.planesAccelerations[i] = getAirplaneAcceleration(gameState->airplanes[i]);
				stateUpdatePacket.planesAngularVelocities[i] = getAirplaneAngularVelocity(gameState->airplanes[i]);
			}
	
			for(i=0; i<gameState->numClients; i++)
			{
				stateUpdatePacket.selfPlane = i+1;
		
				sendPacket(gameState->senderClientsSocket[i], (char*)(&stateUpdatePacket), sizeof(StateUpdatePacket));			
			}
		
		}
		else
		{
			if(gameState->selfAirplaneNumber == -1)
			{
				return;
			}
	
			ClientMovementPacket clientMovementPacket;
			
			clientMovementPacket.packetType = CLIENT_MOVEMENT_PACKET;
			
			clientMovementPacket.planeNum = gameState->selfAirplaneNumber;
			
			Airplane* selfPlane = gameState->airplanes[gameState->selfAirplaneNumber];
			
			clientMovementPacket.planePose = getAirplanePose(selfPlane);
			clientMovementPacket.planeVelocity = getAirplaneVelocity(selfPlane);
			clientMovementPacket.planeAcceleration = getAirplaneAcceleration(selfPlane);
			clientMovementPacket.planeAngularVelocity = getAirplaneAngularVelocity(selfPlane);
			
			sendPacket(gameState->senderServerSocket, (char*)(&clientMovementPacket), sizeof(ClientMovementPacket));	
		}
	}
}


void setFire(GameState* gameState, int fire)
{
	gameState->firing = fire;
}

void setPlaneAilerons(GameState* gameState, int direction)
{
	if(gameState->selfAirplaneNumber == -1)
	{
		return;
	}

	if(direction == 1)
	{		
		setAirplaneRollVelocity(gameState->airplanes[gameState->selfAirplaneNumber], 1.0/500.0);
	}
	else if(direction == 0)
	{		
		setAirplaneRollVelocity(gameState->airplanes[gameState->selfAirplaneNumber], 0.0);
	}
	else
	{	
		setAirplaneRollVelocity(gameState->airplanes[gameState->selfAirplaneNumber], -1.0/500.0);
	}
}

void setPlaneElevator(GameState* gameState, int direction)
{
	if(gameState->selfAirplaneNumber == -1)
	{
		return;
	}

	if(direction == 1)
	{		
		setAirplanePitchVelocity(gameState->airplanes[gameState->selfAirplaneNumber], 1.0/2000.0);
	}
	else if(direction == 0)
	{		
		setAirplanePitchVelocity(gameState->airplanes[gameState->selfAirplaneNumber], 0.0);
	}
	else
	{	
		setAirplanePitchVelocity(gameState->airplanes[gameState->selfAirplaneNumber], -1.0/2000.0);
	}
}

void setPlaneRudder(GameState* gameState, int direction)
{
	if(gameState->selfAirplaneNumber == -1)
	{
		return;
	}

	if(direction == 1)
	{		
		setAirplaneYawVelocity(gameState->airplanes[gameState->selfAirplaneNumber], 1.0/5000.0);
	}
	else if(direction == 0)
	{		
		setAirplaneYawVelocity(gameState->airplanes[gameState->selfAirplaneNumber], 0.0);
	}
	else
	{	
		setAirplaneYawVelocity(gameState->airplanes[gameState->selfAirplaneNumber], -1.0/5000.0);
	}
}


void setPlaneThrottle(GameState* gameState, int direction)
{
	if(gameState->selfAirplaneNumber == -1)
	{
		return;
	}

	Vector3D acceleration;
	
	acceleration.y = 0.0;
	acceleration.z = 0.0;

	if(direction == 1)
	{
		acceleration.x = 1.0/100000.0;
	
		setAirplaneAcceleration(gameState->airplanes[gameState->selfAirplaneNumber], acceleration);
	}
	else if(direction == 0)
	{
		acceleration.x = 0.0;
	
		setAirplaneAcceleration(gameState->airplanes[gameState->selfAirplaneNumber], acceleration);
	}
	else
	{
		acceleration.x = -0.5/100000.0;
	
		setAirplaneAcceleration(gameState->airplanes[gameState->selfAirplaneNumber], acceleration);
	}

}

void setCamera(GameState* gameState, int camera)
{
	gameState->camera = camera;
}

void processGameState(GameState* gameState, long int delta_t)
{
	processReceivedPackets(gameState);

	advanceState(gameState, delta_t);
	
	sendCurrentState(gameState);
}

static double radiansToDegrees(double angle)
{
	return angle*180.0/(3.14159265);
}


static void drawAirplanes(GameState* gameState)
{
	int i;
	for(i=0; i<gameState->numAirplanes; i++)
	{
	
		Airplane* plane = gameState->airplanes[i];
		Pose3D p3D = getAirplanePose(plane);

	
		if(gameState->camera > 1 || i != gameState->selfAirplaneNumber)
		{
						
			glPushMatrix();
				glTranslatef(p3D.position.x, p3D.position.y, p3D.position.z);
		
				glRotatef(radiansToDegrees(p3D.orientation.yaw), 0.0f, 0.0f, 1.0f);
				glRotatef(radiansToDegrees(p3D.orientation.pitch), 0.0f, 1.0f, 0.0f);
				glRotatef(radiansToDegrees(p3D.orientation.roll), 1.0f, 0.0f, 0.0f);			
		
				drawPlane(plane);
			glPopMatrix();
				
		}
										
		glPushMatrix();
			glTranslatef(p3D.position.x, p3D.position.y, 0.1);
			
			glScalef(1.0,1.0,0.001);
	
			glRotatef(radiansToDegrees(p3D.orientation.yaw), 0.0f, 0.0f, 1.0f);
			glRotatef(radiansToDegrees(p3D.orientation.pitch), 0.0f, 1.0f, 0.0f);
			glRotatef(radiansToDegrees(p3D.orientation.roll), 1.0f, 0.0f, 0.0f);			
	
			drawPlaneShadow(plane);
		glPopMatrix();
			
	}
}

static void drawBullets(GameState* gameState)
{
	glPushMatrix();
	
		glLineWidth(2.0);
		glBegin(GL_LINES);
		
			glColor3f(1.0,1.0,0.2);
			int i;
			for(i=0; i<gameState->numBullets; i++)
			{
				if(gameState->bullets[i].active == 1)
				{
					Vector3D pos = gameState->bullets[i].position;
					Vector3D vel = gameState->bullets[i].velocity;
				
					glVertex3f(pos.x, pos.y, pos.z);
					glVertex3f(pos.x + vel.x, pos.y + vel.y, pos.z + vel.z);
				}
			}
		glEnd();
		
	glPopMatrix();
}

static void setCameraPosition(GameState* gameState)
{
	if(gameState->camera == 0)
	{	
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.5f, 0.0f, -5.00f);
		glRotatef(75.0f, 0.0f, 1.0f, 0.0f);
	}
	else if(gameState->camera == 1)
	{
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.2f, 0.0f, -0.5f);
		glRotatef(85.0f, 0.0f, 1.0f, 0.0f);
	}
	else if(gameState->camera == 2)
	{
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -10.0f);		
	}
	else
	{
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 0.0f, -20.0f);			
	}
}

static void drawCross(void)
{
	glPushMatrix();
	
	
		glTranslatef(2.0, 0.0, 0.22);
		glColor3f( 0.0, 1.0, 0.0);
		glLineWidth(1.0);
		
		glBegin(GL_LINES);
		
			glVertex3f( 1.0, 0.0, 0.04);
			glVertex3f( 1.0, 0.0, 0.08);
		
			glVertex3f( 1.0, 0.0,-0.04);
			glVertex3f( 1.0, 0.0,-0.08);
				
			glVertex3f( 1.0, 0.04, 0.0);
			glVertex3f( 1.0, 0.08, 0.0);

			glVertex3f( 1.0,-0.04, 0.0);
			glVertex3f( 1.0,-0.08, 0.0);
		
		glEnd();
	
	glPopMatrix();
}

void drawGameState(GameState* gameState)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
	glLoadIdentity();                       // Reset The View

	Airplane *selfPlane = NULL;
	if(gameState->selfAirplaneNumber != -1)
	{
		selfPlane = gameState->airplanes[gameState->selfAirplaneNumber];
		drawSpeedometer(gameState);
		drawHorizon(gameState);
		drawEmbication(gameState);
		drawAltimeter(gameState);
		drawRadar(gameState);
		drawCompass(gameState);
	}

	glPushMatrix();

		setCameraPosition(gameState);
		
		if(gameState->selfAirplaneNumber != -1)
		{
			if(gameState->camera == 0)
			{
				drawPlane(selfPlane);
			}
			else if(gameState->camera == 1)
			{
				drawCross();
			}
						
			Pose3D p3D = getAirplanePose(selfPlane);
			
			if(gameState->camera != 2)
			{
				glRotatef(radiansToDegrees(p3D.orientation.roll), -1.0f, 0.0f, 0.0f);
				glRotatef(radiansToDegrees(p3D.orientation.pitch), 0.0f, -1.0f, 0.0f);
				glRotatef(radiansToDegrees(p3D.orientation.yaw), 0.0f, 0.0f, -1.0f);			
			
				glTranslatef(-p3D.position.x, -p3D.position.y, -p3D.position.z);
			}
		}
				
		drawAirplanes(gameState);
		drawTerrain();
		drawBullets(gameState);
		drawTargets(gameState->targets);
		
	glPopMatrix();
}

void destroyGameState(GameState* gameState)
{
	int i;
	for(i=0; i<gameState->maxNumAirplanes; i++)
	{
		deleteAirplane(gameState->airplanes[i]);
	}
	free(gameState->airplanes);
	
	deleteReceiverSocket(gameState->receiverSocket);
	deleteSenderSocket(gameState->senderServerSocket);
	
	if(gameState->isHost)
	{
		
		for(i=0; i<gameState->maxNumClients; i++)
		{
			deleteSenderSocket(gameState->senderClientsSocket[i]);
		}	
		free(gameState->senderClientsSocket);
	}
	
	free(gameState);
}
