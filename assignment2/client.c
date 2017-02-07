/*//////////////////////////////////////////

Names: Somesh Khandelia & Subhranil Mondal
Roll no.: 14/CS/105 & 13/CS/111
Assignment No.2
Group no. 24

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
serv_addr.sin_port=htons(50105);

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

  printf("Enter the arithmetic expression: ");
  gets(arr);


  if(arr[0]!='-'||arr[1]!='1'||arr[2]!='\0'){
    printf("\n\nThe arithmetic expression entered in the client was : %s\n",arr);
    printf("\n\nNo. of characters in expression: %d\n",strlen(arr));
    send(sockfd,arr,strlen(arr)+1,0);
    char result[20];
    int i;
    for(i=0;i<20;i++)result[i]='\0';
    recv(sockfd,result,20,0);

    printf("\nThe result received from server is : %s\n\n",result);
  }
  else{
    printf("Client terminated\n\n");
  }

  close(sockfd);

}while(arr[0]!='-'||arr[1]!='1'||arr[2]!='\0');

return 0;
}
