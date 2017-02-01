/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream> //maybe this is needed for the decodeMessage
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

double alpha = 1.5;
double beta = -1.8;
double gamma = 0.1;
double peppu = 0.1;
double joyVal[4];
std::string stringArray[4];

void error(const char *msg)	//function for catching communication errors
{
    perror(msg);
    exit(1);
}

void decodeMessage(char _msg[],double *_joyVal){
  //Should read the buffer array and return string and decode using ":"
  std::string testString(_msg);
  printf("Here is the msg from decoder: %s \n", _msg);
  sscanf(_msg, "%lf:%lf:%lf:%lf", &_joyVal[0], &_joyVal[1], &_joyVal[2], &_joyVal[3]);
  //sscanf(_msg, "%f:%f:%f:%f", &alpha, &beta, &gamma, &peppu);
  for (int i = 0; i<5; i++){
    std::cout << joyVal[i] << std::endl;
  }
}

void sendCodedMessage(int _newsockfd){
  //Works, but needs cleaning
  std::ostringstream ostr;
  ostr.str("");
  ostr << alpha << ":" << beta << ":" << gamma;
  std::string s;
  s = ostr.str();
  int n = write(_newsockfd,s.c_str(),s.length());
}

int main(int argc, char *argv[])
{
	 int sockfd, newsockfd, portno;				//bit addresses
     socklen_t clilen;
     char buffer[256];							//messages buffer
     struct sockaddr_in serv_addr, cli_addr;	//communcation structs
     int n;										//communcation handle
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     //bind socket to address
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //bind socket for communication
              error("ERROR on binding");

     //listen for connection on sockfd, 5 is the amount of possible connections allowed
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
     	 	 (struct sockaddr *) &cli_addr,
              	 &clilen);
     if (newsockfd < 0)
    	 error("ERROR on accept");


     //read what the socket has to say
     	 int i = 0;
     	 while (i < 3){
  			 bzero(buffer,256);
  			 n = read(newsockfd,buffer,255);
  			 if (n < 0) error("ERROR reading from socket");
  			 //printf("Here is the message: %s \n",buffer);
         //Test new function
         decodeMessage(buffer, joyVal);
  			 //break communication if client sends "quit"
  			 std::string str(buffer);
  			 std::string subst = str.substr(0,4);
  			 if (subst == "quit"){
  				 printf("The client has quit the conversation \n");
  				 n = write(newsockfd, "Closing the connection",22);
  				 i = 3;
			 }

			 //Write back to client
			 else{
         //Try the new send function
         sendCodedMessage(newsockfd);
         /*
				 n = write(newsockfd,"I got your message",18);
				 if (n < 0) error("ERROR writing to socket");
         */
       }
     }
     //close the socket and remove the connection
     close(newsockfd);
     close(sockfd);
     return 0;
}
