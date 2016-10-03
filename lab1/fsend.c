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

int readInt(int socket){
  //  bzero(buffer,256);

  // loop until EOF
  while(1) {
    char buffer[256];
    memset(&buffer, '\0', 256);
    int n = read(socket,buffer,255);
    if (n < 0) {
      //printf("ERROR reading from socket\n");
      exit(1);
    }

    printf("%s",buffer);
    n = write(socket,"I got your message",18);
    if (n < 0) {
      //printf("ERROR writing to socket\n");
      exit(1);
    }
  }
  close(socket);
  return 0;

}

int readFile(int socket){



}






//********************CLIENT********************

int calltheServer(int portno);



void writeClient(int socket, const char *filename){
  //char buffer[256];
  //printf("Please enter the message: ");
  //memset(&buffer, '\0', 256);

  FILE* fd = fopen(filename,"rb");
  long lSize;
  char * buffer;
  size_t result;

  //fd = fopen(filename,"rd"); //open file

  // obtain file size:
  fseek (fd, 0 , SEEK_END);
  lSize = ftell (fd);
  rewind (fd);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {
    fputs ("Memory error",stderr); exit (2);
  }

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,fd);
  if (result != lSize) {
    fputs ("Reading error",stderr); exit (3);
  }

  /* the whole file is now loaded in the memory buffer. */




  //fread(buff,SIZE,NUMELEM,fd);
  printf("\n The bytes read are [%s]\n",buffer);

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
  printf("%s\n",buffer);
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
  server = gethostbyname("localhost");
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





void parse(char *line, char **argv){
  while (*line != '\0') {       /* if not the end of line ....... */
    while (*line == ' ' || *line == '\t' || *line == '\n')
      *line++ = '\0';     /* replace white spaces with 0    */
    *argv++ = line;          /* save the argument position     */
    while (*line != '\0' && *line != ' ' &&
        *line != '\t' && *line != '\n')
      line++;             /* skip the argument until ...    */
  }
  *argv = '\0';                 /* mark the end of argument list  */
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

int main (int argc, char *argv[]) {
  //parse input
  //IP, Port, etc.

  //TODO: Scan ARGV for args
  file = argv[argc-1];
  if (arrayContains(argv, "-h")) {
    char* helpMessage = "\t-h\t\tPrint this help screen\n\t-v\t\tVerbose output\n\t-p port\t\tSet the port to connect on (e.g., 9285)\n\t-n bytes\tNumber of bytes to send, defaults whole file\n\t-o offset\tOffset into file to start sending\n\t-l\t\tListen (on server side) on port instead of connecting and\n\t\t\twrite output to file and dest_ip refers to which ip to bind to.\n\t\t\t(default:localhost)\n";
    printf("fsend [OPTIONS] dest_ip file\n%s",helpMessage);
  } else if (argc > 1) {
    if (arrayContains(argv, "-v")) {
      verbose = 1;
    }
    if (arrayContains(argv, "-p")) { // see if there is -p####
      port = atoi(argv[arrayContains(argv, "-p")+1]);
      //if (verbose) {
      printf("port number: %d\n", port);
      //}
    }
    //    arg = "-o";
    //    if (*argv[1] == *arg) {
    //
    //    }
    if (arrayContains(argv, "-l")) {
      printf("server\n");
      printf("Setting up Server Socket\n");
      //set up up port port
      // int port = 5124;
      int setup = setupServerSocket(port);
      int accept = serverSocketAccept(setup);
      int socketOn = readInt(accept); //read from the accepted Socket, need to change to read any inpu
    } 
    //server(9285)
  } 
  if(!arrayContains(argv, "-l") && !arrayContains(argv, "-h")) {
    printf("Setting up Client\n");
    printf("Reading File....\n");
    //readFile(); 
    //int port = 5124;
    //waits for call
    int call = calltheServer(port);
    //int serv = serverSocketAccept(call);
    if (file != "") {
      printf("file: %s\n", file);
      writeClient(call, file);
    }
    //client(localhost, 9285)
  }
  printf("fuck! We here?");
  return 0;
}
