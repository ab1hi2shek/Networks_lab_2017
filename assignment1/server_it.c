/*//////////////////////////////////////////

Names: Somesh Khandelia & Subhranil Mondal
Roll no.: 14/CS/105 & 13/CS/111
Assignment No.1
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
  int sockfd,newsockfd;
  int clilen;
  struct sockaddr_in serv_addr,cli_addr;

  if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
    printf("Unable to create socket\n");
    exit(0);
  }

  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  serv_addr.sin_port=htons(50105);


  if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
    printf("Unable to bind\n");
    exit(0);
  }

  listen(sockfd,10);

  char buff[100];
  int i;

  while (1) {

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
    &clilen) ;
    if (newsockfd < 0) {
        printf("Accept error\n");
        exit(0);
      }

      for(i=0; i < 100; i++) {
        buff[i] = '\0';
      }
      recv(newsockfd, buff, 100, 0);
      printf("\n\nThe received expression in server : %s\n", buff);

/////////////////////////Calculating the Result////////////////////////////////////
      i=0;
      int z=0;
      int y=0,k;
      int result;
      char temp[2];
      while(buff[i]!='\0'){
        if(buff[i]!='+'&&buff[i]!='*'&&buff[i]!='-'&&buff[i]!='/'&&buff[i]!=' '){
          //if the character is a digit
          temp[0]=buff[i];
          temp[1]='\0';
          z=z*10+atoi(temp);

          i++;
        }
        else if(buff[i]=='+'){
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
        else if(buff[i]=='-'){
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
        else if(buff[i]=='*'){
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
        else if(buff[i]=='/'){
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

      char servermsg[20];
      snprintf(servermsg,sizeof(servermsg),"%d",result);
      send(newsockfd,servermsg,strlen(servermsg)+1,0);

      printf("\n\n%d is the result\n\n",result);
      close(newsockfd);
}
}
