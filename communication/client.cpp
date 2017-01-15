#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void decodeMessage(char _msg[]){
  //Should read the buffer array and return string and decode using ":"
  printf("Here is the msg from decoder: %s \n", _msg);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("You are connected to the server.\n Write your message and press 'Enter' to send it.\n Type 'quit' to exit the conversation.\n ");

    int i = 0;
    while (i < 3){
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
        	error("ERROR writing to socket");

    	 //break communication if client types "quit"
    	 std::string str(buffer);
    	 std::string subst = str.substr(0,4);
    	 if (subst == "quit"){
    		 printf("The client has quit the conversation \n");
    		 i = 3;
    	 }

    	//reset buffer for incoming communication
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0){
        	error("ERROR reading from socket");
        }
        //printf("%s\n",buffer);
        decodeMessage(buffer);
    }
    close(sockfd);
    return 0;
}
