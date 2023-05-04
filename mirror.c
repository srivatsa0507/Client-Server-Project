#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// function to process each client connection in a separate thread
void *processclient(void *arg)
{
    int cnctn = *((int *)arg); // cast the connection to int type and dereference the pointer
    char buff[1024];
    FILE *fp;

    // loop to receive and process commands from client
    while (1)
    {
        // clear buffer and receive command from client
        memset(buff, 0, sizeof(buff));
        recv(cnctn, buff, sizeof(buff), 0);

        // if client sends "quit", break the loop
        if (strcmp(buff, "quit") == 0)
        {
            break;
        }

        // process the command and send the result to the client
        fp = popen(buff, "r"); // open a pipe to execute the command and read the output
        if (fp == NULL)
        {
            perror("popen failed");
            exit(EXIT_FAILURE);
        }
        memset(buff, 0, sizeof(buff));
        while (fgets(buff, sizeof(buff), fp)) // read output line by line and send to client
        {
            send(cnctn, buff, strlen(buff), 0);
            memset(buff, 0, sizeof(buff));
        }
        pclose(fp); // close the pipe
    }
    close(cnctn); // close the client connection
    return NULL;
}

int con;

int main()
{

FILE *fptr;

   fptr = fopen("mirror_count.txt", "w");

   if (fptr == NULL) {
      printf("Error opening file!");
      return 1;
   }

   fprintf(fptr, "%d", 0);

   fclose(fptr);
    int socketOfServer;
    struct sockaddr_in serverAddress, clientAddress; // define server and client socket addresses
    socklen_t addressLen = sizeof(clientAddress);

    socketOfServer = socket(AF_INET, SOCK_STREAM, 0); // create a TCP socket
    serverAddress.sin_family = AF_INET; // set the address family
    serverAddress.sin_port = htons(4000); // set the port number in network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // set the IP address to any available interface
    memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero)); // set the rest of the struct to 0

    // bind the socket to the server address and port
    if (bind(socketOfServer, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(socketOfServer, 5) == -1) // backlog is 5
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // loop to accept incoming connections and process each in a separate thread
    while (1)
    {
        con = accept(socketOfServer, (struct sockaddr *)&clientAddress, &addressLen); // accept the connection
        printf("Connection established with client\n");
        pthread_t thread;
        pthread_create(&thread, NULL, processclient, (void *)&con); // create a new thread to handle the connection
    }

    close(socketOfServer); // close the server socket
    return 0;
}
