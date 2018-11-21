#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h> // getch()
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <ctime>
#include <iostream>
#include <cstdlib>

#define PORT 2007
#define H 31
#define W 61
#define H2 5
#define W2 51
#define NR_GHOSTS 10
#define BUFFER 10000
#define MAX_PLAYERS 5

struct coord
{
   int x;
   int y;
};

struct PacMan {
   struct coord position;
   int vx;
   int vy;
   int lives;
   bool chasing;
   int food_collected;
};


struct Ghost {
   struct coord position;
   int vx;
   int vy;
   bool chasing;
};


struct Ghost allGhosts[NR_GHOSTS];

struct PacMan myPacMan = {
                           {
                              .x = 1,
                              .y = 1,
                           },
                           .vx = 0,
                           .vy = 0,
                           .lives = 3,
                           .chasing = false,
                           .food_collected = 0
                         };
char *msg;

char heading[H2][W2]=
{
    {"||  || |||||| ||  || ||   || |||||| |||||| ||||   "},
    {"||  || ||  || ||  || |||| ||   ||   ||     ||  || "},
    {"|||||| |||||| ||  || || ||||   ||   ||||   ||   ||"},
    {"||  || ||  || ||  || ||  |||   ||   ||     ||  || "},
    {"||  || ||  || |||||| ||   ||   ||   |||||| ||||   "},
};

char playfield[H][W] =
{
   { "############################################################" },
   { "#                            ##                            #" },
   { "# ########### #############  ##  ############# ########### #" },
   { "# ########### #############  ##  ############# ########### #" },
   { "#                                                          #" },
   { "# ########### ####    ################    #### ########### #" },
   { "#             ####           ##           ####             #" },
   { "############# ####           ##           #### #############" },
   { "############# #############  ##  ############# #############" },
   { "############# #############  ##  ############# #############" },
   { "############# ####                        #### #############" },
   { "############# ####                        #### #############" },
   { "############# ####                        #### #############" },
   { "#                                                          #" },
   { "#                                                          #" },
   { "############# ####                        #### #############" },
   { "############# ####                        #### #############" },
   { "############# ####                        #### #############" },
   { "############# ####                        #### #############" },
   { "############# ####                        #### #############" },
   { "############# ####    ################    #### #############" },
   { "#                            ##                            #" },
   { "# ########### #############  ## ############## ########### #" },
   { "#        ####                                  ####        #" },
   { "#        ####                                  ####        #" },
   { "####     #### ###     ################     ### ####     ####" },
   { "#             ###            ##            ###             #" },
   { "# #########################  ## ########################## #" },
   { "#                                                          #" },
   { "############################################################" }
}; // <-- CAUTION! Semicolon necessary!

void gameTitle(){

	system("title HAUNTED Server");
}

void initialize()
{
   // replace each empty field in the playfield
   //    with a food field
   for (int i = 0; i < H; i++)
   {
      for (int j = 0; j < W; j++)
      {
         if (playfield[i][j]==' ')
            playfield[i][j] = '.';
      }
   }

   // initialize all ghosts
   for (int i = 0; i < NR_GHOSTS; i++)
   {
        allGhosts[i].vx = 0;
        allGhosts[i].vy = 0;
        allGhosts[i].chasing = true;
        if (i<5)
            allGhosts[i].position.y = 14;
        allGhosts[0].position.x = 28;
        allGhosts[1].position.x = 29;
        allGhosts[2].position.x = 30;
        allGhosts[3].position.x = 31;
        allGhosts[4].position.x = 32;
        if (i>4)
            allGhosts[i].position.y = 13;
        allGhosts[5].position.x = 28;
        allGhosts[6].position.x = 29;
        allGhosts[7].position.x = 30;
        allGhosts[8].position.x = 31;
        allGhosts[9].position.x = 32;


        playfield[allGhosts[i].position.y][allGhosts[i].position.x] = 'G';

   }
} // initialize

void move_ghost()
{
    for (int i = 0; i < NR_GHOSTS; i++)
    {
        playfield[allGhosts[i].position.y][allGhosts[i].position.x] = ' ';
        int nx, ny, dir;
        dir = rand() % 4;
        if (dir == 0)
            allGhosts[i].vx = -1;
            //allGhosts[i].vy = 0;
        if (dir == 1)
            allGhosts[i].vy = -1;
            //allGhosts[i].vx = 0;
        if (dir == 2)
            allGhosts[i].vx = +1;
            //allGhosts[i].vy = 0;
        if (dir == 3)
            allGhosts[i].vy = +1;
            //allGhosts[i].vx = 0;

        nx = allGhosts[i].vx + allGhosts[i].position.x;
        ny = allGhosts[i].vy + allGhosts[i].position.y;

        if (playfield[ny][nx] == '#')
       {
          allGhosts[i].vx = 0;
          allGhosts[i].vy = 0;
       }

       allGhosts[i].position.x += allGhosts[i].vx;
       allGhosts[i].position.y += allGhosts[i].vy;

       playfield[allGhosts[i].position.y][allGhosts[i].position.x] = 'G';
    }

}

int move_man(int m)
{
   // delete PacMan from old position
   playfield[myPacMan.position.y][myPacMan.position.x] = ' ';

   // compute new desired coordinate (nx,ny)
   int nx = myPacMan.vx + myPacMan.position.x;
   int ny = myPacMan.vy + myPacMan.position.y;

   // test for wall

   if (playfield[ny][nx] == '#')
   {
      myPacMan.vx = 0;
      myPacMan.vy = 0;
      m= 9;
   }

   if (playfield[ny][nx] == 'G')
   {
      myPacMan.vx = 0;
      myPacMan.vy = 0;
      m = 8;
   }

   // update PacMan's coordinate
   myPacMan.position.x += myPacMan.vx;
   myPacMan.position.y += myPacMan.vy;

   // is there a food piece at the new location?
   if (playfield[ny][nx] == '.')
   {
      myPacMan.food_collected++;
      m = 6;
      playfield[ny][nx]= ' ';
   }

   // put PacMan back again to playfield
   playfield[myPacMan.position.y][myPacMan.position.x] = 'M';

   return m;
}

void show_heading()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("\t\t\t\t\t\t\t\t\t      ");
    for (int i = 0; i < H2; i++)
    {
      for (int j = 0; j < W2; j++)
      {
          if (heading[i][j]=='|')
          {
              SetConsoleTextAttribute(h, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
              printf("%c", heading[i][j]);
          }
          else if (heading[i][j]==' ')
            printf("%c", heading[i][j]);
      }
      //printf("\n");
      printf("\n\t\t\t\t\t\t\t\t\t      ");

   }
}

void show_playfield()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    show_heading();
    //printf("\n");
    printf("\n\t\t\t\t\t\t\t\t\t");
    for (int i = 0; i < H; i++)
    {
      for (int j = 0; j < W; j++)
      {
          if (playfield[i][j]=='#')
          {
              SetConsoleTextAttribute(h, FOREGROUND_BLUE|FOREGROUND_INTENSITY);
              printf("%c", playfield[i][j]);
          }
          else if (playfield[i][j]=='G')
          {
              SetConsoleTextAttribute(h, FOREGROUND_RED|FOREGROUND_INTENSITY);
              printf("%c", playfield[i][j]);
          }
          else if (playfield[i][j]=='.')
          {
              SetConsoleTextAttribute(h, FOREGROUND_RED|FOREGROUND_GREEN);
              printf("%c", playfield[i][j]);
          }
          else
          {
             SetConsoleTextAttribute(h, FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN);
             printf("%c", playfield[i][j]);
          }
      }
      printf("\n\t\t\t\t\t\t\t\t\t");

   }

   printf("\t\t\t  Score: %d\n", myPacMan.food_collected);
}


// Set cursor position in console
void set_cursor_position(int x, int y)
{
   //Initialize the coordinates
   COORD coord = { x, y };
   //Set the position
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

} // set_cursor_position


void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void consoleResize()
{
    HANDLE hOut;
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    SMALL_RECT DisplayArea = {0, 0, 70, 80};

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hOut,&SBInfo);

    SetConsoleWindowInfo(hOut,TRUE,&DisplayArea);
}

int parsingMSG(char str)
{
    int msgNumber;
    //char *token = strtok(str, "-");
    char token = str;
    if (token == 'y')
        msgNumber = 1;
    else if (token == 'w')
        msgNumber = 2;
    else if (token == 'a')
        msgNumber = 3;
    else if (token == 's')
        msgNumber = 4;
    else if (token == 'd')
        msgNumber = 5;

        else
            printf("ERROR");

    return msgNumber;
}

int conv(char *ip)
{
    int num = 0;
    int p1,p2,p3,p4;
    //tok = strtok(ip,".");
    p1 = std::atoi(strtok(ip,"."));
    p2 = std::atoi(strtok(ip,"."));
    p3 = std::atoi(strtok(ip,"."));
    p4 = std::atoi(strtok(ip,"."));

    num = (p1*1000000000)+(p2*1000000)+(p3*1000)+p4;
    return num;
}

int* registering(int* ipAddresses, int ipCount, int ip)
{
    //printf("Register\n");
    for (int i=0;i<MAX_PLAYERS;++i)
    {
        if (ipAddresses[ipCount]== ip)
        {

            msg = "Already Registered.";
            break;
        }
    }
    ipAddresses[ipCount]=ip;
    msg = "Use w,a,s or d to move ";
    return ipAddresses;
}

int checkIP(int message, int* ipAddresses, int ip, int registered)
{
    int saveMessage = message;

    for (int i=0;i<MAX_PLAYERS;++i)
    {
        if (ipAddresses[i]== ip)
        {
            registered = 1;
            message = saveMessage;
            break;
        }
        else
        {
            message = 7;
        }
    }
    return message;
}
void createMessages(int message)
{
    switch (message)
    {
    case 1:
        msg = "Use w,a,s or d to move ";
    case 2:
        msg = "Enter move";
        break;
    case 3:
        msg = "Enter move";
        break;
    case 4:
        msg = "Enter move";
        break;
    case 5:
        msg = "Enter move";
        break;
    case 6:
        msg = "Move Successful";
        break;
    case 7:
        msg = "Not Registered";
        break;
    case 8:
        msg = "YOU LOSE!";
        break;
    case 9:
        msg = "WRONG WAY!";
        break;
    default:
        msg = "Invalid Entry";
        break;
    }
}
int move_figures(int message)
{
    switch (message)
    {
    case 2:
        myPacMan.vy = -1; // cursor up
        myPacMan.vx = 0;
        break;
    case 3:
        myPacMan.vx = -1;
        myPacMan.vy = 0;
        break;
    case 4:
        myPacMan.vy = +1;
        myPacMan.vx = 0;
        break;
    case 5:
        myPacMan.vx = +1;
        myPacMan.vy = 0;
        break;
    case 6:
        myPacMan.vx = 0;
        myPacMan.vy = 0;
        break;
    }
    message = move_man(message);
    move_ghost();
    return message;
}



int main()
{
    SOCKET s;
    struct sockaddr_in serverSocket, clientSocket;
    char receiveBuffer[BUFFER];
    int clientSocketLength, recv_len;
    int registered = 0; //0 means ip does not exist
    WSADATA wsa;
    int ipCount = 0;
    int ipAddresses[5];
    int message, ip;
    int *addr_ip = ipAddresses;
    clientSocketLength = sizeof(clientSocket) ;

    gameTitle();
    //Initialize winsock
    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
      printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Socket Initialized.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_addr.s_addr = INADDR_ANY;
    serverSocket.sin_port = htons( PORT );

    //Bind
    if( bind(s ,(struct sockaddr *)&serverSocket , sizeof(serverSocket)) == SOCKET_ERROR)
    {
        printf("\nBind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Bind done\n\n");


    Sleep(1000);

    consoleResize();
    initialize();
    //keep listening for data

    while(1)
    {
        hidecursor();
        //system("cls");

       // printf("\n\t\t\tWaiting for data...\n");
        fflush(stdout);
        //show_playfield();

        if((recv_len = recvfrom(s, receiveBuffer, BUFFER, 0, (struct sockaddr *) &clientSocket, &clientSocketLength)) == SOCKET_ERROR)
        {
            printf("\n\nrecvfrom() failed with error code : %d" , WSAGetLastError());
            //exit(EXIT_FAILURE);
            while(1);
        }

        //print details of the client/peer and the data received
        //printf("\n\nReceived packet from %s:%d\n", inet_ntoa(clientSocket.sin_addr), ntohs(clientSocket.sin_port));
       //printf("\nClient Says: " );
        //printf("%c", receiveBuffer[0]);
       // Sleep(1000);


        message = parsingMSG(receiveBuffer[0]);
        ip = conv(inet_ntoa(clientSocket.sin_addr));
        if (message == 1)
        {
            addr_ip = registering(ipAddresses,ipCount,ip);
            ++ipCount;
            show_playfield();
        }
        else if ((message == 2)||(message == 3)||(message == 4)||(message == 5))
        {
            message = checkIP(message,ipAddresses, ip, registered);
            message = move_figures(message);
            createMessages(message);
            system("cls");
            show_playfield();
        }
       else
        msg = "INVALID!";

        fflush(stdout);

        //printf("Final Message: %s\n", msg);

       if (sendto(s, msg, 100, 0, (struct sockaddr*) &clientSocket, clientSocketLength) == SOCKET_ERROR)
        {
            printf("\nsendto() failed with error code : %d" , WSAGetLastError());
			while(1);
        }
		//else
			//printf("\nMessage Sent Back to Client");

        if (msg == "YOU LOSE!")
            break;
        Sleep( 1000/50);
        set_cursor_position(0,0);
    }
    closesocket(s);
    WSACleanup();
    return 0;
}
