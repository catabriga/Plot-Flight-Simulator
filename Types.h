#ifndef TYPES_H_
#define TYPES_H_

#define MAX_PACKET_SIZE 4096
#define MAX_NUM_CLIENTS 15
#define MAX_NUM_AIRPLANES 16
#define MAX_NUM_BULLETS 64

struct Vector3D
{
	double x;
	double y;
	double z;
};
typedef struct Vector3D Vector3D;

struct Orientation3D
{
	double roll;
	double pitch;
	double yaw;
};
typedef struct Orientation3D Orientation3D;

struct Pose3D
{
	Vector3D position;
	Orientation3D orientation;
};
typedef struct Pose3D Pose3D;

struct Bullet
{
	Vector3D position;
	Vector3D velocity;
	int active;
	int plane;
};
typedef struct Bullet Bullet;

#endif // TYPES_H_
