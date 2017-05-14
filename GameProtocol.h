
#include "Types.h"

#define NEW_PLAYER_PACKET 32

#define STATE_UPDATE_PACKET 42

#define CLIENT_MOVEMENT_PACKET 52

#define CREATE_BULLET_PACKET 62

typedef struct StateUpdatePacket StateUpdatePacket;
struct StateUpdatePacket
{
	char packetType;
	
	int numPlanes;
	int selfPlane;
	Pose3D planesPoses[MAX_NUM_AIRPLANES];
	Vector3D planesVelocities[MAX_NUM_AIRPLANES];
	Vector3D planesAccelerations[MAX_NUM_AIRPLANES];
	Orientation3D planesAngularVelocities[MAX_NUM_AIRPLANES];
};


typedef struct ClientMovementPacket ClientMovementPacket;
struct ClientMovementPacket
{
	char packetType;
		
	int planeNum;
	Pose3D planePose;
	Vector3D planeVelocity;
	Vector3D planeAcceleration;
	Orientation3D planeAngularVelocity;
};

typedef struct BulletPacket BulletPacket;
struct BulletPacket
{
	char packetType;
	
	Vector3D bulletPosition;
	Vector3D bulletVelocity;
};
