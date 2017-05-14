/*
 * SenderSocket.h
 *
 *  Created on: Mar 14, 2010
 *      Author: lucas
 */

#ifndef SENDERSOCKET_H_
#define SENDERSOCKET_H_

typedef struct SenderSocket SenderSocket;


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

SenderSocket* createSenderSocketString(char* ipAddress, unsigned short port);
SenderSocket* createSenderSocket(unsigned int ipAddress, unsigned short port);

void sendPacket(SenderSocket* senderSocket, char* packetData, int packetSize);

void deleteSenderSocket(SenderSocket* senderSocket);

#endif /* SENDERSOCKET_H_ */
