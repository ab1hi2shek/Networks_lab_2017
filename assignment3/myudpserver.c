/*//////////////////////////////////////////

Names: Somesh Khandelia & Subhranil Mondal
Roll no.: 14/CS/105 & 13/CS/111
Assignment No.3
Group no. 24

*////////////////////////////////////////////
/*
 * udpserver.c - A simple UDP echo server
 * usage: udpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /*
   * check command line arguments
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /*
   * socket: create the parent socket
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /*
   * bind: associate the parent socket with a port
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr,
	   sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  /*
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
     (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    printf("\n\nThe received expression in server : %s\n", buf);

    /*
     * gethostbyaddr: determine who sent the datagram
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server received datagram from %s (%s)\n",
	   hostp->h_name, hostaddrp);
    printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);



    /////////////////////////Calculating the Result////////////////////////////////////
          int i=0;
          int z=0;
          int y=0,k;
          int result;
          char temp[2];
          while(buf[i]!='\0'){
            if(buf[i]!='+'&&buf[i]!='*'&&buf[i]!='-'&&buf[i]!='/'&&buf[i]!=' '){
              //if the character is a digit
              temp[0]=buf[i];
              temp[1]='\0';
              z=z*10+atoi(temp);

              i++;
            }
            else if(buf[i]=='+'){
              //if character is for addition

                if(y==0){
                  y=1;
                  k=z;
                  z=0;
                }
                else if(y==1){
                  result=k+z;
                  k=result;
                  z=0;
                  y=1;
                }
                else if(y==2){
                  result=k-z;
                  k=result;
                  z=0;
                  y=1;
                }
                else if(y==3){
                  result=k*z;
                  k=result;
                  z=0;
                  y=1;
                }
                else if(y==4){
                  result=k/z;
                  k=result;
                  z=0;
                  y=1;
                }
                i++;
              }//close for addition
            else if(buf[i]=='-'){
              //if character is for subtraction
              if(y==0){
                y=2;
                k=z;
                z=0;
              }
              else if(y==1){
                result=k+z;
                k=result;
                z=0;
                y=2;
              }
              else if(y==2){
                result=k-z;
                k=result;
                z=0;
                y=2;
              }
              else if(y==3){
                result=k*z;
                k=result;
                z=0;
                y=2;
              }
              else if(y==4){
                result=k/z;
                k=result;
                z=0;
                y=2;
              }
              i++;
            }//close for subtraction
            else if(buf[i]=='*'){
              //if character is for multiplication
              if(y==0){
                y=3;
                k=z;
                z=0;
              }
              else if(y==1){
                result=k+z;
                k=result;
                z=0;
                y=3;
              }
              else if(y==2){
                result=k-z;
                k=result;
                z=0;
                y=3;
              }
              else if(y==3){
                result=k*z;
                k=result;
                z=0;
                y=3;
              }
              else if(y==4){
                result=k/z;
                k=result;
                z=0;
                y=3;
              }
              i++;
            }//close for multiplication
            else if(buf[i]=='/'){
              //if character is for division
              if(y==0){
                y=4;
                k=z;
                z=0;
              }
              else if(y==1){
                result=k+z;
                k=result;
                z=0;
                y=4;
              }
              else if(y==2){
                result=k-z;
                k=result;
                z=0;
                y=4;
              }
              else if(y==3){
                result=k*z;
                k=result;
                z=0;
                y=4;
              }
              else if(y==4){
                result=k/z;
                k=result;
                z=0;
                y=4;
              }
              i++;
            }//close for division
            else{
              //if the character is a space
              i++;
            }
          }//all characters read


          if(y==0){
            result=z;
          }
          else if(y==1){
            result=k+z;
          }
          else if(y==2){
            result=k-z;
          }
          else if(y==3){
            result=k*z;
          }
          else if(y==4){
            result=k/z;
          }

    /////////////////////////Result Calculation finished/////////////////////////////////////

          char servermsg[100];
          for(i=0;i<100;i++)servermsg[i]='\0';
          snprintf(servermsg,sizeof(servermsg),"%d",result);


          printf("\n\n%d is the result\n\n",result);
          //printf("\n\n%s is the result in string\n\n",servermsg);


          n = sendto(sockfd, servermsg, 100, 0,
      	       (struct sockaddr *) &clientaddr, clientlen);
          if (n < 0)
            error("ERROR in sendto");


  }
}
