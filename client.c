#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include<fcntl.h>
#include<stdbool.h>

bool serverT = false;


char *validate(char *command)
{
    char *response = malloc(sizeof(char) * 1024);
    char *resp[2]; // Array to store parsed command arguments
    int i;

    if (sscanf(command, "findfile %ms", &resp[0]) == 1)
    {
        // If the command is "findfile <filename>", construct a find command
        // to search for the file in the home directory and retrieve its metadata
  sprintf(response, "output=$(find ~/ -maxdepth 1 -type f -name %s -exec stat -c '%%s\\t%%w\\t%%n' {} \\; | head -n 1); if [ -z \"$output\" ]; then echo \"File not found\"; else echo -e \"$output\"; fi", resp[0]);
        free(resp[0]);
    }
    else if (sscanf(command, "sgetfiles %ms %ms%*c", &resp[0], &resp[1]) == 2)
    {
        // If the command is "sgetfiles <size1> <size2>", construct a zip command
        // to create a tarball of files in the home directory based on their sizes
        sprintf(response, "zip temp.tar.gz $(find ~/ -maxdepth 1 -type f -size +%sc -size -%sc)", resp[0], resp[1]);
        free(resp[0]);
        free(resp[1]);
    }
    else if (sscanf(command, "dgetfiles %ms %ms%*c", &resp[0], &resp[1]) == 2)
    {
        // If the command is "dgetfiles <date1> <date2>", construct a zip command
        // to create a tarball of files in the home directory based on their modification time
        sprintf(response, "zip temp.tar.gz $(find ~/ -maxdepth 1 -type f -newermt \"%s\" ! -newermt \"%s\")", resp[0], resp[1]);
        free(resp[0]);
        free(resp[1]);
    }
    else if (sscanf(command, "getfiles %[^\n]", response) == 1)
    {
        // If the command is "getfiles <filename1> <filename2> ...", construct a zip command
        // to create a tarball of files in the home directory based on their names
        char *tkn;
        char files[1024] = "";
        int count = 0;
        tkn = strtok(response, " ");
        while (tkn != NULL)
        {
            char tmp[256];
            if (count > 0)
            {
                strcat(files, "-o ");
            }
            sprintf(tmp, "-name %s ", tkn);
            strcat(files, tmp);
            count++;
            tkn = strtok(NULL, " ");
        }
        sprintf(response, "zip temp.tar.gz $(find ~/ -maxdepth 1 -type f %s)", files);
    }
    else if (sscanf(command, "gettargz %[^\n]", response) == 1)
    {
        // If the command is "gettargz <extension1> <extension2> ...", construct a zip command
        // to create a tarball of files in the home directory based on their file extensions
        char *tkn;
        char files[1024] = "";
        int count = 0;
        tkn = strtok(response, " ");
        while (tkn != NULL)
        {
            char tmp[256];
            if (count > 0)
            {
                strcat(files, "-o ");
            }
                sprintf(tmp, "-iname \"*.%s\" ", tkn);
            strcat(files, tmp);
            count++;
            tkn = strtok(NULL, " ");
        }
        sprintf(response, "zip temp.tar.gz $(find ~/ -maxdepth 1 -type f %s)", files);
    }
     else if (strcmp(command, "quit") == 0)
    {

	if (!serverT){
FILE *fptr;
   int num;

   fptr = fopen("mirror_count.txt", "r+");

   if (fptr == NULL) {
      printf("Error opening file!");
      return "1";
   }

   if (fscanf(fptr, "%d", &num) == 1) {
      fseek(fptr, 0, SEEK_SET);  // move file pointer to beginning of file
      fprintf(fptr, "%d", num - 1);  // write updated number to file
      
   } 
}
else{

FILE *fptr;
   int num;

   fptr = fopen("server_count.txt", "r+");

   if (fptr == NULL) {
      printf("Error opening file!");
      return "1";
   }

   if (fscanf(fptr, "%d", &num) == 1) {
      fseek(fptr, 0, SEEK_SET);  // move file pointer to beginning of file
      fprintf(fptr, "%d", num - 1);  // write updated number to file
      
   }
}
        exit(0);
    }
    else
    {
        // If the command is not recognized, set the response to an error message
        sprintf(response, "Error: Invalid command");
    }

    return response;
}

int main()
{



    FILE *fptr;
   int num;

   fptr = fopen("server_count.txt", "r");

   if (fptr == NULL) {
      printf("Error opening server_count file!");
      return 1;
   }

   if (fscanf(fptr, "%d", &num) == 1) {
      
   } 
fclose(fptr);

FILE *fptr2;
   int num2;

   fptr2 = fopen("mirror_count.txt", "r");

   if (fptr2 == NULL) {
      printf("Error opening mirror_count file!");
      return 1;
   }

   if (fscanf(fptr2, "%d", &num2) == 1) {
      
   } else {
      printf("The mirror_count file is empty or does not contain a number.");
   }

   fclose(fptr2);

    // Create a socket for the client
    int socketOfClient;
    struct sockaddr_in serverAddress;

    // Create a socket with IPv4 address family, TCP socket type, and default protocol (0)
    socketOfClient = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address
    serverAddress.sin_family = AF_INET;

    if(num<4){
    serverT = true;
    serverAddress.sin_port = htons(3000);
    FILE *fptr;
    int num;
    fptr = fopen("server_count.txt", "r+");

   if (fptr == NULL) {
      printf("Error opening file!");
      return 1;
   }

   if (fscanf(fptr, "%d", &num) == 1) {
      fseek(fptr, 0, SEEK_SET);  // move file pointer to beginning of file
      fprintf(fptr, "%d", num + 1);  // write updated number to file
      
   } else {
      printf("The server_count is empty or does not contain a number.");
   }

   fclose(fptr);
}
else {
    serverAddress.sin_port = htons(4000);
FILE *fptr;
   int num3=0;

   fptr = fopen("mirror_count.txt", "r+");

   if (fptr == NULL) {
      printf("Error opening file!");
      return 1;
   }

   if (fscanf(fptr, "%d", &num3) == 1) {
      fseek(fptr, 0, SEEK_SET);  // move file pointer to beginning of file
      fprintf(fptr, "%d", num3 + 1);  // write updated number to file
      
   } else {
      printf("The mirror_count is empty or does not contain a number.");
   }
}

 // Set the port number to 3000
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set the IP address to localhost (127.0.0.1)
    memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero));

    // Connect to the server
    if (connect(socketOfClient, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(socketOfClient, F_GETFL, 0);
    fcntl(socketOfClient, F_SETFL, flags | O_NONBLOCK);

    // Enter the main loop for sending and receiving data
    while (1)
    {
        // Prepare buffers for user input and server output
        char command[1024];
        char output[1024];
        char *response;

        // Prompt for user input
        printf("Enter command: ");
        fgets(command, 1024, stdin);
        fflush(stdin);

        // Remove trailing newline character from user input
        if (command[strlen(command) - 1] == '\n')
        {
            command[strlen(command) - 1] = '\0';
        }

        // Call the validate function to validate the user input
        response = validate(command);
	
        // Send the validated command to the server
        send(socketOfClient, response, strlen(response), 0);

        // Use select to check for data received from the server
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(socketOfClient, &read_fds);
        struct timeval timeout;
        timeout.tv_sec = 1; // Set the timeout value in seconds
        timeout.tv_usec = 0;

        int select_result = select(socketOfClient + 1, &read_fds, NULL, NULL, &timeout);

        if (select_result == -1)
        {
            perror("select failed");
            exit(EXIT_FAILURE);
        }
        else if (select_result == 0)
        {
            // No data received from the server, continue to the next iteration
            continue;
        }
        else
        {
            // Receive data from the server and print it to the console
            memset(output, 0, sizeof(output));
            while (recv(socketOfClient, output, sizeof(output), 0) > 0)
            {
                printf("%s", output);
                memset(output, 0, sizeof(output));
            }
        }
    }



    // Close the client socket
    close(socketOfClient);


    return 0;
}


