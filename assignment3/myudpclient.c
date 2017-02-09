/*//////////////////////////////////////////

Names: Somesh Khandelia & Subhranil Mondal
Roll no.: 14/CS/105 & 13/CS/111
Assignment No.3
Group no. 24

*////////////////////////////////////////////
/*
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    char result[100];

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    do{
      /* socket: create the socket */
      sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      if (sockfd < 0)
          error("ERROR opening socket");

      /* gethostbyname: get the server's DNS entry */
      server = gethostbyname(hostname);
      if (server == NULL) {
          fprintf(stderr,"ERROR, no such host as %s\n", hostname);
          exit(0);
      }

      /* build the server's Internet address */
      bzero((char *) &serveraddr, sizeof(serveraddr));
      serveraddr.sin_family = AF_INET;
      bcopy((char *)server->h_addr,
  	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
      serveraddr.sin_port = htons(portno);

      /* get a message from the user */
      bzero(buf, BUFSIZE);
      printf("Enter the arithmetic expression: ");
      gets(buf);



      if(buf[0]!='-'||buf[1]!='1'||buf[2]!='\0'){
        printf("\n\nThe arithmetic expression entered in the client was : %s\n",buf);
        printf("\n\nNo. of characters in expression: %d\n",strlen(buf));
        serverlen = sizeof(serveraddr);
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&serveraddr, serverlen);

        int i;
        char result[100];
       for(i=0;i<100;i++)result[i]='\0';
    // bzero(result,BUFSIZE);

          n = recvfrom(sockfd, result, 100, 0, (struct sockaddr*)&serveraddr, &serverlen);
          if(n<0){
            printf("\nrecv from error\n");
          }
          printf("\n\nThe result received from server is : ");
          for(i=0;i<100;i++){
            if(result[i]!='\0')
            printf("%c",result[i]);
          }
          printf("\n\n");
        //printf("\nThe result received from server is : %s\n\n",result);
        //memset(result,0,255);
      //  bzero(result,BUFSIZE);
      }
      else{
        printf("Client terminated\n\n");
      }
      close(sockfd);

    }while(buf[0]!='-'||buf[1]!='1'||buf[2]!='\0');

    return 0;
}
