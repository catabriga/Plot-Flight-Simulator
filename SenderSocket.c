/*
 * SenderSocket.c
 *
 *  Created on: Mar 14, 2010
 *      Author: lucas
 */

#include "SenderSocket.h"

#define BUFFER_SIZE 1024

struct SenderSocket
{
	int handle;
	struct sockaddr_in address;
};

SenderSocket* createSenderSocket(unsigned int ipAddress, unsigned short port)
{
	SenderSocket* senderSocket = (SenderSocket*)malloc(sizeof(SenderSocket));

	senderSocket->handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if ( senderSocket->handle <= 0 )
	{
		printf( "failed to create socket\n" );
		return NULL;
	}

	senderSocket->address.sin_family = AF_INET;
	senderSocket->address.sin_addr.s_addr = ipAddress;
	senderSocket->address.sin_port = htons( port );

	return senderSocket;

}

SenderSocket* createSenderSocketString(char* ipAddress, unsigned short port)
{
	SenderSocket* senderSocket = (SenderSocket*)malloc(sizeof(SenderSocket));

	senderSocket->handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if ( senderSocket->handle <= 0 )
	{
		printf( "failed to create socket\n" );
		return NULL;
	}

	//printf("%d", inet_addr(ipAddress));

	senderSocket->address.sin_family = AF_INET;
	senderSocket->address.sin_addr.s_addr = inet_addr(ipAddress);
	senderSocket->address.sin_port = htons( port );

	return senderSocket;

}

void sendPacket(SenderSocket* senderSocket, char* packetData, int packetSize)
{
	int sent_bytes = sendto( senderSocket->handle, packetData, packetSize,
	                         0, (struct sockaddr*)&senderSocket->address, sizeof(struct sockaddr_in) );

	if ( sent_bytes != packetSize )
	{
		printf( "failed to send packet: return value = %d\n", sent_bytes );
	}

}

void deleteSenderSocket(SenderSocket* senderSocket)
{
	free(senderSocket);
}

