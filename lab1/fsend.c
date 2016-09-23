#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "sockets.h"

TODO: Clean this all up!

int server (int portno) {
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

//int client (char* addr, int port) {
  //open socket and connect to addr on port
  
  //send data over socket

  //close socket
//}

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

int main (int argc, char *argv[]) {

  //parse input
  //IP, Port, etc.

  //are we a server or a client?
  char *listen = "-l";
  if (argc > 1 && *argv[1] == *listen) {
    printf("server\n");
    //server(9285)
  } else {
    printf("client\n");
    //client(localhost, 9285)
  }

  return 0;
}
 
