#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include "sockets.h"
#include "blowfish.cc"

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
ssize_t read_return;

// TODO: Clean this all up!


//------- KDC Set-up ----------//


int setupKDCSocket(int portno) {
 //Get a socket of the right type for now 
 int sockfd = socket(AF_INET, SOCK_STREAM, 0);

 if (sockfd < 0) {
  printf("Error opening socket on KDC");
  exit(1);

 }
 
 printf("KDC started on Port: %d\n", portno);
 
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

  return sockfd;   

}

int KDCServerSocketAccept(int KDCSocket){
  int newsockfd;

  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);

  newsockfd = accept(KDCSocket, (struct sockaddr *) &cli_addr, &clilen);
  printf("Requesting Ks from KDC\n");
  if (newsockfd < 0){
   printf("ERROR on accept");
   exit(1);

  }

  return newsockfd;

}

void writeN(int socket){
 int x = 5;
 int n = write(socket, &x, sizeof(x));
 
 printf("Write N1: %d to socket %d\n", x, socket);
 if (n <0){
  printf("Error writing N1 to socket \n:");
  exit(0);
 }

}

int readN(int socket){
 int x;
 int n = read(socket, &x, sizeof(x));
 if (n < 0){
  printf("Error reading N1 from socket\n");
  exit(1);
 }
 printf("N is %d", x);
 return x;



}

void writeKB(int socket){


  char KBbuffer[256];
  printf("Enter Kb: \n");
  memset(&KBbuffer, '\0', 256);
  fgets(KBbuffer, 255,stdin);

  int kb = write(socket,KBbuffer,strlen(KBbuffer));
  if(kb < 0){
   printf("Error writing KB back to socket\n");
   exit(0);
  }

}


void writeKA(int socket){
  char KAbuffer[256];
  printf("Enter Ka: \n");
  memset(&KAbuffer, '\0', 256);
  fgets(KAbuffer, 255,stdin);
 
  

  int ka = write(socket,KAbuffer,strlen(KAbuffer));
  if (ka < 0){
    printf("ERROR writing KA back to socket\n");
    exit(0);
  }
}

void writeKS(int socket){
  char KSbuffer[256];
  printf("Enter Ks: \n");
  memset(&KSbuffer, '\0', 256);
  fgets(KSbuffer,255,stdin);

  int ks = write(socket,KSbuffer,strlen(KSbuffer));
  if (ks < 0) {
    printf("ERROR writing KS back to socket\n");
    exit(0);
  }
}

int readKA(int socket){

char KAbuffer[256];
 memset(&KAbuffer, '\0', 256);
 int ka = read(socket,KAbuffer,256);
 if (ka <0){
  printf( "Error reading Ka from KDC Socket\n");
  exit(1);
 }

 printf("%s", KAbuffer);

 return 0;
}

int readKB(int socket){
char KBbuffer[256];
memset(&KBbuffer, '\0', 256);

int kb = read(socket,KBbuffer,256);
 if (kb <0){
  printf("Error reading KB from KDC Socket \n");
  exit(1);
}
 printf("%s",KBbuffer);

 return 0;
}

int readKS(int socket){
 char KSbuffer[256];
 memset(&KSbuffer, '\0', 256);
 int ks = read(socket,KSbuffer, 256);
 if (ks < 0){
   printf("Error reading KS for KDC socket\n");
   exit(1);
 }
 
  printf("%s",KSbuffer);
 //computation to n
 
 return 0;
 
}


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


int readFile(int socket, char* read_input){

while(1){
	char buffer[BUFSIZ];
	int filefd = open(read_input, O_CREAT|O_WRONLY, 0777);
        if (filefd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        do {
            read_return = read(socket, buffer, BUFSIZ);
            if (read_return == -1) {
                perror("read error on SERVER READ");
                exit(EXIT_FAILURE);
            }
            if (write(filefd, buffer, read_return) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        } while (read_return > 0);
        close(filefd);
        close(socket);
        //free(buffer);
    }
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

  int filefd;
  ssize_t read_return;
  ssize_t read_length;
  char bufferTest[BUFSIZ];
  char lengthBuffer[amountToSend];
  char * bufferLength;
  filefd = open(filename, O_RDONLY);

  

 while(1){
  //read_length = read(amountToSend, lengthBuffer, BUFSIZ); 
  read_return = read(filefd, bufferTest, BUFSIZ);
 
  if (read_return == 0){
     break;
  }
  if (read_return == -1){
    perror("read error on WRITECLIENT");
    exit(EXIT_FAILURE);
  }

  //int len = write(socket,bufferLength,read_length); 
  int n = write(socket,buffer,read_return);
  if (n < 0) {
    printf("ERROR writing to socket\n");
    exit(0);
  }


  //if (result < 0) {
    //printf("ERROR reading from socket\n");
    //exit(0);
 // }
}
  //close(socket);
  close(filefd); 
  //free(bufferLength);
  free (buffer);


}

int callServerB(char* host, int portNum){


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
  //printf("Port number is as follows : %d", portNum);

  serv_addr.sin_port = htons(9111);

  

  //printf("Host is as follows: %s", host);
  // Setup the server host address
  struct hostent *server;
  server = gethostbyname("thing3.cs.uwec.edu");
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

long nonceGen(long nonce) {
    const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 1;
	long t = A * (state % Q) - R * (state / Q);
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	return (long)(((double) state/M)* nonce);
}

int main(int argc, char *argv[]){
  int i;
  int ipIndex = 0;
  long sz = 0;

  long nonce = nonceGen(5647892341);

  for (i = 1; i < (argc - 1); i++) {
    if (strcmp("-h", argv[i]) == 0) {
      char* helpMessage = "\t-h\t\tPrint this help screen\n\t-v\t\tVerbose output\n\t-p port\t\tSet the port to connect on (e.g., 9285)\n\t-n bytes\tNumber of bytes to send, defaults whole file\n\t-o offset\tOffset into file to start sending\n\t-l\t\tListen (on server side) on port instead of connecting and\n\t\t\twrite output to file and dest_ip refers to which ip to bind to.\n\t\t\t(default:localhost)\n";
      printf("fsnd [OPTIONS] dest_ip file\n%s",helpMessage);
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
      //Test for both stdout and the accepted file
      int socketOn = readFile(accept, argv[4]);
      
      return 0;
    }

    if (strcmp("-k", argv[i]) == 0) {
      printf("Setting up KDC\n");
      int setup = setupKDCSocket(port);
      //write the Eka over to requester.
      int accept = KDCServerSocketAccept(setup);
      int nRecd = readN(accept);         
      //read back to the CLIENT
      printf("recieved the following %d\n", nRecd);
     
      //Create Session Key
      //writeKS(accept);
      writeKS(accept);    
      writeKA(accept);
      writeKB(accept);
      return 0;
  }
 } 

  file = argv[argc - 1];
  
  //CALL THE KDC FIRST
  int call =  calltheServer(port);
  //printf("Offset amount for file %i\n", offset);
  printf("Sending N1 over to the KDC\n");
  //WRITE 
  writeN(call);
  readKS(call);
  readKA(call);
  readKB(call);
  //Read FROM KDC, Close socket
  close(call);
  //int kReturned = readN(call);
  //printf("N1 is %d\n", kReturned);
  
  //NOW WE CALL the next Client
  
  char portBuffer[256];
  printf("Please Enter Port Number for B: \n");
  memset(&portBuffer, '\0', 256);
  fgets(portBuffer,255,stdin);
   
  //int p = (int)portBuffer;
  char hostBuffer[256];
  printf("Please Enter Host for B: \n");
  memset(&hostBuffer, '\0', 256);
  fgets(hostBuffer,255,stdin);
 
  int newCall = callServerB(hostBuffer, 9111);
  printf("Connecting to B\n"); 
  
  char Nabuffer[256];
  printf("Please Enter Na : \n");  
  memset(&Nabuffer, '\0', 256);
  fgets(Nabuffer,255,stdin);

  //char KabufferInput[256];
  printf("Please Enter Ka : \n");
  
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
