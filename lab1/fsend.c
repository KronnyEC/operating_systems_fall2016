#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "sockets.h"

#define FILENAME "input.txt"
#define SIZE 1
#define NUMELEM 5

char *file = "";
int verbose = 0;
int port = 0;
char* possibleIPs[2];
int offset = 0;
long bytes = 0;
int flag = 0;
long fsize = 0;

// TODO: Clean this all up!


int setupServerSocket (int portno) {
  //Get a soocket of the right type for now test with int sockfd
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0){
    printf("Error opening socket");
    exit(1);

  }

  printf("Server Socket started on Port: %d\n", portno);
  // port number

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr));

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

  // Basically the machine we are on...
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig bype first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);

  // Bind the socket to the given port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(1);
  }

  // set it up to listen
  listen(sockfd,5); 
  //open listen socket on port

  //wait for input

  //close socket
  return sockfd;
}

int calltheServeer(int portno){
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    fprintf(stderr,"ERROR opening socket\n");
    exit(0);
  }
}

int serverSocketAccept(int serverSocket){
  int newsockfd;
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);

  newsockfd = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0){
    printf("ERROR on accept");
    exit(1);
  }

  return newsockfd;
}

int readFile(int socket){
  //  bzero(buffer,256);

  // loop until EOF
  while(1){
    char buffer[256];
    memset(&buffer, '\0', 256);
    int n = read(socket,buffer,255);
    if (n < 0) {
      printf("ERROR reading from socket\n");
      exit(1);
    }

    printf("%s",buffer);
    //n = write(socket,"File Size Recieved", 18);
    //if (n < 0) {
      //printf("ERROR writing to socket\n");
      //exit(1);
    //}
  }  
  close(socket);
  return 0;

}



//********************CLIENT********************

int calltheServer(int portno);



void writeClient(int socket, const char *filename, int offset, long amountToSend, int flag){
  //char buffer[256];
  //printf("Please enter the message: ");
  //memset(&buffer, '\0', 256);

  FILE* fd = fopen(filename,"rb");
  long lSize;
  char * buffer;
  size_t result;
  long totalSize; 
  long allocationSize;
  //fd = fopen(filename,"rd"); //open file

  // obtain file size:
  
 
  //totalSize = ftell:(fd);
  //printf("File size is %d bytes \n", totalSize);  
  fseek (fd, 0, SEEK_END); //find the entire fileSize
  lSize = ftell (fd);
  
  if (verbose) {
    printf("File size: %d\n", lSize);
  }
  //fsize = lSize;

  //printf("File size: %d\n", fsize);
  
  fseek (fd, offset, SEEK_SET); //get to the offset. 
  totalSize = ftell(fd);
  //printf("Amout to offset file size is %d bytes \n", totalSize);

  if (flag == 1){ 
    allocationSize = amountToSend;
    //fsize = allocationSize;
    //printf("Send the following amount to the Server %d\n: ", amountToSend); 

  } else {
    allocationSize = lSize - totalSize;
    //fsize = allocationSize;
  }
  //printf("Allocation Size for file is %d\n : ", allocationSize);

  //rewind (fd);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*allocationSize);
  if (buffer == NULL) {
    fputs ("Memory error",stderr); exit (2);
  }

  // copy the file into the buffer:
  result = fread (buffer,1,allocationSize,fd);
  if (result != allocationSize) {
    fputs ("Reading error",stderr); exit (3);
  }

  /* the whole file is now loaded in the memory buffer. */




  //fread(buff,SIZE,NUMELEM,fd);
  //printf("\n The bytes read are [%s]\n",buffer);

  //fgets(buffer,255,stdin);


  int n = write(socket,buffer,strlen(buffer));
  if (n < 0) {
    printf("ERROR writing to socket\n");
    exit(0);
  }

  //memset(&buffer, '\0', 256);
  //n = read(socket,buffer,255);

  if (result < 0) {
    printf("ERROR reading from socket\n");
    exit(0);
  }
  //printf("%s\n",buffer);
  close(socket);
  // terminate
  fclose (fd);
  free (buffer);

}


int calltheServer(int portno){ //char* host{  
  // Socket pointer
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr,"ERROR opening socket\n");
    exit(0);
  }

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr));
  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

  // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig byte first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);


  // Setup the server host address
  struct hostent *server;
  server = gethostbyname(possibleIPs[0]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);  /// dest, src, size

  // Connect to the server
  if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR connecting\n");
    exit(0);
  }

  return sockfd;
}

int arrayContains (char** argArray, char* argToFind) {
  // if contained, return index where it was found
  // if not found, return 0 (0 is never returned otherwise)

  int i = 1;
  for(i; i<sizeof(argArray)-1;++i){
    if (strcmp(argArray[i], argToFind) == 0) {
      return i;
    }
  }
  return 0;
}

int main(int argc, char *argv[]){
  int i;
  int ipIndex = 0;
  long sz = 0;

  for (i = 1; i < (argc - 1); i++) {
    if (strcmp("-h", argv[i]) == 0) {
      char* helpMessage = "\t-h\t\tPrint this help screen\n\t-v\t\tVerbose output\n\t-p port\t\tSet the port to connect on (e.g., 9285)\n\t-n bytes\tNumber of bytes to send, defaults whole file\n\t-o offset\tOffset into file to start sending\n\t-l\t\tListen (on server side) on port instead of connecting and\n\t\t\twrite output to file and dest_ip refers to which ip to bind to.\n\t\t\t(default:localhost)\n";
      printf("fsend [OPTIONS] dest_ip file\n%s",helpMessage);
    }
    if (strcmp("-v", argv[i]) == 0) {
      verbose = 1;
    }
    if (strcmp("-p", argv[i]) == 0) {
      port = atoi(argv[i+1]);
    }
    if (strcmp("-n", argv[i]) == 0){
      //printf("Number of bytes to send : %i\n", atoi(argv[i+1]));
      flag = 1;
      bytes = atoi(argv[i+1]);
    }
    if (strcmp("-o", argv[i]) == 0){
      //printf("Offset into file : %i\n", atoi(argv[i+1]));
      offset = atoi(argv[i+1]);
      //printf("Offset is %i\n", offset);

    }
    if ((strcmp("localhost", argv[i]) == 0) || strstr(argv[i], ".") != NULL) {
      possibleIPs[ipIndex] = argv[i];
      ipIndex++;
    }
  }
  for (i = 1; i < (argc - 1); i++) {
    if (strcmp("-l", argv[i]) == 0) {
      printf("Setting up Server\n");
      //printf("Server setup on port %i\n", port); 
      int setup = setupServerSocket(port);
      int accept = serverSocketAccept(setup);
      int socketOn = readFile(accept);
      return 0;
    }
  }

  file = argv[argc - 1];
  
  //For File length for verbose
  //FILE *fp;
  //int len;
  //fp = fopen(file, "r");
  //fseek(fp, 0, SEEK_END);  
  //len = ftell(fp);
  //fclose(fp);

  int call =  calltheServer(port);
  //printf("Offset amount for file %i\n", offset);

  if (verbose) {
    printf("Connecting to IP: %s\n", possibleIPs[0]);
    printf("On port: %d\n", port);
    printf("File name: %s \n", file);
    printf("File size: %d \n", fsize);
    printf("Sending file [offset=%i | send size = %d]\n", offset, bytes);  
  }

  writeClient(call, file, offset, bytes, flag);
  if (verbose) {
    printf("Send success!\n");
  }
  return 0;
}
