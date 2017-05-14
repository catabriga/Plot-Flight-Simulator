/*
 * ReceiverSocket.h
 *
 *  Created on: Mar 14, 2010
 *      Author: lucas
 */

#ifndef RECEIVERSOCKET_H_
#define RECEIVERSOCKET_H_

typedef struct ReceiverSocket ReceiverSocket;


#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

ReceiverSocket* createReceiverSocket(unsigned short port);

int receivePacket(ReceiverSocket* receiverSocket, unsigned char* packetData, int maximumPacketSize, unsigned int* ipAddress, unsigned short* port);

void deleteReceiverSocket(ReceiverSocket* receiverSocket);

#endif /* RECEIVERSOCKET_H_ */
