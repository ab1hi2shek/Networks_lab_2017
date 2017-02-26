/*//////////////////////////////////////////

Names: Somesh Khandelia & Subhranil Mondal
Roll no.: 14/CS/105 & 13/CS/111
Assignment No.4
Group no. B24

*////////////////////////////////////////////
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<sys/select.h>
#include<sys/time.h>
#include <netdb.h>

#define MAXLINE 1024

int main(void)
 {
   int listenfd, connfd, udpfd, nready, maxfdp1,i;
   char mesg[MAXLINE];
   pid_t childpid;
   fd_set rset;
   ssize_t n;
   socklen_t len;
   const int on = 1;
   struct sockaddr_in cliaddr, servaddr;
   //void sig_chld(int);
   char buff[100];
   /* create listening TCP socket */
   listenfd = socket(AF_INET, SOCK_STREAM, 0) ;
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(6000);

   setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

   bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

   listen(listenfd, 5);

   /* create UDP socket */
   udpfd = socket(AF_INET, SOCK_DGRAM, 0);
   bzero(&servaddr, sizeof(servaddr));

   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(6000);
   bind(udpfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

   FD_ZERO(&rset);
   maxfdp1 = listenfd>udpfd?listenfd:udpfd;
   maxfdp1++;
   while(1) {

     FD_SET(listenfd, &rset);
     FD_SET(udpfd, &rset);
     if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {

        continue; /* back to for() */

      }
///////////if TCP client is ready/////////////////////////////////////////
      if (FD_ISSET(listenfd, &rset)) {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
        if (connfd < 0) {
            printf("Accept error\n");
            exit(0);
          }

          for(i=0; i < 100; i++) {
            buff[i] = '\0';
          }
          recv(connfd, buff, 100, 0);
          printf("\n\nThe received text in server(from TCP client) : %s\n", buff);
          close(connfd);

      }

//////////////if UDP client is ready///////////////////////////////////////
      if (FD_ISSET(udpfd, &rset)) {
        len = sizeof(cliaddr);
        for(i=0; i < MAXLINE; i++) {
          mesg[i] = '\0';
        }
        n = recvfrom(udpfd,mesg,MAXLINE,0,(struct sockaddr *)&cliaddr,&len);

        printf("\n\nThe received expression in server from UDP client : %s\n", mesg);
        //sendto(udpfd, mesg, n, 0, (struct sockaddr *) &cliaddr, len);
      }

    }//end of while
}//end of main
