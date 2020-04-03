#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <netdb.h>
#include <string.h>

#define SERV_ADDR "10.42.0.1"
#define SERV_PORT 1024
#define TEAM_ID   5             /* Your team ID */

#define MSG_ACK    0
#define MSG_START  1
#define MSG_STOP   2
#define MSG_KICK   3
#define MSG_TEST   4
#define MSG_OBJ_ID 5

#define SA struct sockaddr
#define Sleep( msec ) usleep(( msec ) * 1000 )

///// RICCARDO TORRES /////

/*This file contains three functions whose main purpose is (respectively)
to connect to the server, manage the stop signal and send a signal to warn
when a ball is kicked or an identified object.*/

int s;
uint16_t msgId = 0;

int sockfd, connfd;
uint8_t* buffer;
struct sockaddr_in servaddr, cli;



int read_from_server (int sock, uint8_t* buffer, size_t maxSize) {

  int bytes_read = read (sock, buffer, maxSize);

  if (bytes_read <= 0) {
    fprintf (stderr, "[ERR] Server unexpectedly closed connection...\n");
    //close (s);
    exit (EXIT_FAILURE);
  }

  printf ("[DEBUG] Received %d bytes\n", bytes_read);
  return bytes_read;
}


/* This function is identical to the one sent to us for the server test.
In fact it serves to connect to it, specifically it waits for the START signal. */

int waitSTART() {

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("[ERR] Socket creation failed...\n");
		exit(0);
	}
	else
		printf("[DEBUG] Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERV_ADDR);
	servaddr.sin_port = htons(SERV_PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("[ERR] Connection with the server failed...\n");
		exit(0);
	}
	else
		printf("[DEBUG] Connected to the server..\n");


	//Receive the start message and print the various parameters
	buffer = (uint8_t*) malloc(8*sizeof(uint8_t));
	if (buffer==NULL) {
		printf("[ERR] Memory finished\n");
	}
	read_from_server (sockfd, buffer, 8);

	uint16_t id = buffer[0] | buffer[1] << 8;

	uint8_t src = buffer[2];
	uint8_t dst = buffer[3];
	uint8_t type = buffer[4];

	printf("[DEBUG] ID of the start: %d\n", id);
	printf("[DEBUG] From: %d\n", src);
	printf("[DEBUG] To: %d\n", dst);
	printf("[DEBUG] Type: %d\n", type);

	if (type == 1) {
		printf("[DEBUG] START message received\n");
	}

	else {
		printf("[ERR] I expected a START message, but another message arrived\n");
		exit(55);
	}

	if (src != 255) {
		printf("[ERR] I expected a message from the server (id=255), but I received a message from a foreigner\n");
		exit(65);
	}

	if (dst != TEAM_ID) {
		printf("[ERR] I expected a message addressed to me (id = %d), but I wasn't the receiver (id receiver = %d)\n", TEAM_ID, dst);
		exit(75);
	}

  return 0;
}

/* The following function manages the STOP signal. The idea was to activate
a thread, immediately after receiving the start, in order to continuously
read the signals coming from the server and, in case of STOP signal, terminate
the program.
Unfortunately this functionality could not be implemented in the final project,
due to lack of time. */

int waitSTOP(){

  int stop=0;
  buffer = (uint8_t*) malloc(8*sizeof(uint8_t));
  if (buffer==NULL) {
    printf("[ERR] Memory finished\n");
  }

  while(stop==0){

    read_from_server (sockfd, buffer, 8);

    uint8_t src = buffer[2];
    uint8_t dst = buffer[3];
    uint8_t type = buffer[4];

    if (type == 2) {
      printf("End simulation!!!\n");
      stop==1;
      exit(0); // Exit from the code and stop the robot
    }

    if (src != 255) {
      printf("[ERR] I expected a message from the server (id=255), but I received a message from a foreigner\n");
      exit(65);
    }

    if (dst != TEAM_ID) {
      printf("[ERR] I expected a message addressed to me (id = %d), but I wasn't the receiver (id receiver = %d)\n", TEAM_ID, dst);
      exit(75);
    }
  }
  return 0;
}

/* This third and last function is used to manage the robot's communication to
the server when a ball has been sent or an object identified. In this case the
difference between the two messages has been interpreted by raising or not
raising a flag, which will then affect the type of message to be sent.
In both functions the ACK message is managed, in fact in case the server
should start some error in the message sent by us, we will send it again, this
for a maximum of two times.
Finally, it is worth mentioning that it has not been managed the possibility to
change the ID of the object or to store the old one, since in our code there is
not this function and therefore it seemed useless to add it to the sending
unction, which has been simplified by regenerating each time a different code. */

int sendObject(int ball, int x, int y, int shape){

	//Receive the start message and print the various parameters
	buffer = (uint8_t*) malloc(8*sizeof(uint8_t));
	if (buffer==NULL) {
		printf("[ERR] Memory finished\n");
	}

  uint16_t randomID = rand() % 65535; // object id

  while(randomID < 5) {
      uint16_t randomID = rand() % 65535;
  }


  if(ball==1){

    ///// SEND BALL //////////
    uint16_t BallID = randomID; //id random
    buffer[0] = (uint8_t) BallID;
    buffer[1] = (uint8_t)(BallID >> 8);
    buffer[2] = TEAM_ID; //src, us
    buffer[3] = 255;  //dst, server (id=255)
    buffer[4] = MSG_KICK; //Message type
    buffer[5] = x;
    buffer[6] = y;
    buffer[7] = 0; //3 random bytes
    write(sockfd, buffer, 8);

    printf ("[DEBUG] Ready to receive an ACK\n");
    read_from_server (sockfd, buffer, 8);
    //uint16_t id = buffer[0] | buffer[1] << 8; //i don't care about ack id
    uint8_t src = buffer[2];
    uint8_t dst = buffer[3];
    uint8_t type = buffer[4];
    uint8_t error = buffer[5];
    uint16_t idConfirmed = buffer[6] | buffer[7] << 8;

    if (src!=255) {
        printf("[ERR] I expected a message from the server (id=255), but I received a message from a foreigner (id sender = %d)\n", src);
        //free(buffer);
        return 1;
      //exit(65);
    }

    if (dst != TEAM_ID) {
      printf("[ERR] I expected a message addressed to me (id = %d), but I wasn't the receiver (id receiver = %d)\n", TEAM_ID, dst);
        //free(buffer);
        return 1;
      //exit(75);
    }

    if (type!=3) {
        printf("[ERR] I expected an ACK (type=0), but I received another type of message\n");
        //free(buffer);
        return 1;
        //exit(55);
    }

    if (error>0) {
        printf("[ERR] My last message was not well processed: %d\n", error);
        //free(buffer);
        return 1;
        //exit(85);
    }

    if (idConfirmed != BallID) {
        printf("[ERR] ID issue\n");
        //free(buffer);
        return 1;
        //exit(95);
    }

    printf("[DEBUG] ACK received\n");

  } else {

    ///// SEND OBJECT /////
    uint16_t objectID = randomID; //id
    buffer[0] = (uint8_t)  objectID;
    buffer[1] = (uint8_t)(objectID >> 8);
    buffer[2] = TEAM_ID; //src, us
    buffer[3] = 255;  //dst, server (id=255)
    buffer[4] = objectID; //Message type
    buffer[5] = shape; //cylinder
    buffer[6] = x; //x
    buffer[7] = y; //y
    write(sockfd, buffer, 8);


    printf ("[DEBUG] Ready to receive an ACK\n");

    read_from_server (sockfd, buffer, 8);
    //uint16_t id = buffer[0] | buffer[1] << 8; //i don't care about ack id
    uint8_t src = buffer[2];
    uint8_t dst = buffer[3];
    uint8_t type = buffer[4];
    uint8_t error = buffer[5];
    uint8_t idConfirmed = buffer[6] | buffer[7] << 8;

    if (src!=255) {
    		printf("[ERR] I expected a message from the server (id=255), but I received a message from a foreigner (id sender = %d)\n", src);
  	    //free(buffer);
        return 1;
  		//exit(65);
    }

    if (dst != TEAM_ID) {
  		printf("[ERR] I expected a message addressed to me (id = %d), but I wasn't the receiver (id receiver = %d)\n", TEAM_ID, dst);
  	    //free(buffer);
        return 1;
  		//exit(75);
    }

    if (type!=0) {
    		printf("[ERR] I expected an ACK (type=0), but I received another type of message\n");
  	    //free(buffer);
        return 1;
    		//exit(55);
    }

    if (error>0) {
    		printf("[ERR] My last message was not well processed: %d\n", error);
  	    //free(buffer);
        return 1;
     		//exit(85);
    }

    if (idConfirmed != objectID) {
    		printf("[ERR] ID issue\n");
  	    //free(buffer);
        return 1;
     		//exit(95);
    }

    printf("[DEBUG] ACK received\n");

  }
  return 0;
}
