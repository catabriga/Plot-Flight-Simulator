/*
 * SenderSocket.c
 *
 *  Created on: Mar 14, 2010
 *      Author: lucas
 */

#include "ReceiverSocket.h"

#define BUFFER_SIZE 1024

struct ReceiverSocket
{
	int handle;
};


ReceiverSocket* createReceiverSocket(unsigned short port)
{
	ReceiverSocket* receiverSocket = (ReceiverSocket*)malloc(sizeof(ReceiverSocket));

	receiverSocket->handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if ( receiverSocket->handle <= 0 )
	{
		printf( "failed to create socket\n" );
		return NULL;
	}

	struct sockaddr_in tempAddress;
	tempAddress.sin_family = AF_INET;
	tempAddress.sin_addr.s_addr = INADDR_ANY;
	tempAddress.sin_port = htons( port );

	if ( bind( receiverSocket->handle, (struct sockaddr*) &tempAddress, sizeof(struct sockaddr_in) ) < 0 )
	{
		printf( "failed to bind socket\n" );
		return NULL;
	}

	int nonBlocking = 1;
	if ( fcntl( receiverSocket->handle, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
	{
		printf( "failed to set non-blocking socket\n" );
		return NULL;
	}

	return receiverSocket;

}

int receivePacket(ReceiverSocket* receiverSocket, unsigned char* packetData, int maximumPacketSize, unsigned int* ipAddress, unsigned short* port)
{
	struct sockaddr_in from;
	socklen_t fromLength = sizeof( from );

	int receivedBytes = recvfrom( receiverSocket->handle, (char*)packetData, maximumPacketSize,
								   0, (struct sockaddr*)&from, &fromLength );

	if ( receivedBytes > 0 )
	{
		//*ipAddress = ntohl( from.sin_addr.s_addr );
		//*port = ntohs( from.sin_port );

		*ipAddress = ( from.sin_addr.s_addr );
		*port = ( from.sin_port );
	}
	else
	{
		//printf("No packet received\n");
	}


	return receivedBytes;
	// process received packet


}

void deleteReceiverSocket(ReceiverSocket* receiverSocket)
{
	free(receiverSocket);
}

