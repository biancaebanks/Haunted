#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define BUFFER 1000
#define PORT 2007   //The port on which to listen for incoming data
#define SERVER "10.129.129.13"  //ip address of udp server

void sendTo();
void recFrom();

int score = 0;

int main(void)
{
    struct sockaddr_in connectedSocket;

    int s;
	int length=sizeof(connectedSocket);

    char receiveBuffer[BUFFER];
    char userInput[3], *message;

	//clear the buffer by filling null, it might have previously received data
    memset(receiveBuffer,'\0', BUFFER);

	WSADATA wsa;
	//Initialize winsock
    printf("\nInitializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("\nFailed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("\n...Initialized.\n\n\n");
    //create socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
    {
		printf("\n\nsocket() failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
    }
    //setup address structure
    memset((char *) &connectedSocket, 0, sizeof(connectedSocket));
    connectedSocket.sin_family = AF_INET;
    connectedSocket.sin_port = htons(PORT);
    connectedSocket.sin_addr.S_un.S_addr = inet_addr(SERVER);

    printf("Would you like to register to play Pac-Man? (y/n) \n");
    while(1)
    {
        gets(userInput);
        if (tolower(userInput[0]) == 'y')
        {
            printf("Connecting...\n");
            message[0] = 'y';
        }
        else if (tolower(userInput[0]) == 'n') {
            printf("Closing...\n");
            closesocket(s);
            WSACleanup();
            exit(1);
            break;
        }
        else if((tolower(userInput[0]) == 'w')||(tolower(userInput[0]) == 'a')||(tolower(userInput[0]) == 's')||(tolower(userInput[0]) == 'd'))
        {
            printf("Sending Direction...");
            if (userInput[0] == 'w')
                message[0]='w';
            else if (userInput[0] == 'a')
                message[0]='a';
            else if (userInput[0] == 's')
                message[0]='s';
            else if (userInput[0] == 'd')
                message[0]='d';
        }
        else
        {
            printf("INVALID INPUT");
        }
        //userInput[1] = '-';
        //message = createMessage(userInput);
            //send the message

        if (sendto(s, message,sizeof(message) , 0 , (struct sockaddr *) &connectedSocket, sizeof(connectedSocket)) == SOCKET_ERROR)
        {
            printf("\nsendto() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        printf("\nMessage Successfully sent to Server\n");
        fflush(stdout);
        if (recvfrom(s, receiveBuffer, BUFFER, 0, (struct sockaddr *) &connectedSocket,&length) == SOCKET_ERROR)
        {
            printf("\nrecvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        printf("Score : %i\n",score);
        printf("\nServer Says : %s", receiveBuffer);
        printf("\n");
        if(strcmp(receiveBuffer,"YOU LOSE!") == 0)
        {
            closesocket(s);
            WSACleanup();
            return 0;
        }
        if (strcmp(receiveBuffer,"Move Successful")== 0)
        {
            score++;
        }
    }

    closesocket(s);
    WSACleanup();

    return 0;

}
