#include <stdio.h>

int server (int port) {
  //open listen socket on port
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //wait for input
  
  //close socket
}

int client (char* addr, int port) {
  //open socket and connect to addr on port
  
  //send data over socket

  //close socket
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
