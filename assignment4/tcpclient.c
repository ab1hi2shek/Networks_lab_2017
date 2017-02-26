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

int main(void){

int sockfd;
struct sockaddr_in serv_addr,cli_addr;

serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
serv_addr.sin_port=htons(6000);

char arr[100];
char buf[100];
do{
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
    printf("Socket creation error in client\n");
    exit(0);
  }

  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    printf("Unable to connect to server\n");
    exit(0);
  }
  printf("\n\nThis is TCP client\n\nEnter text to be sent: ");
  gets(arr);
  send(sockfd,arr,strlen(arr)+1,0);
  //for(i=0;i<20;i++)result[i]='\0';
  //recv(sockfd,result,20,0);

  //printf("\nThe result received from server is : %s\n\n",result);

  close(sockfd);

}while(1);

return 0;
}
