#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<sys/sem.h>

int checkRow(char**arr,int rownum,int board_size){
	int i;
	int flago=0,flagx=0;
	for(i=0;i<board_size;i++){

		if(arr[rownum][i]=='O'&&flagx==0){
			flago=1;
		}
		else if(arr[rownum][i]=='O'&&flagx==1){
			return 0;
		}
		else if(arr[rownum][i]=='X'&&flago==0){
			flagx=1;
		}
		else if(arr[rownum][i]=='X'&&flago==1){
			return 0;
		}
		else if(arr[rownum][i]=='.'){
			return 0;
		}

	}
	return 1;
}


int checkColumn(char**arr,int colnum,int board_size){
	int i;
	int flago=0,flagx=0;
	for(i=0;i<board_size;i++){
		if(arr[i][colnum]=='O'&&flagx==0){
			flago=1;
		}
		else if(arr[i][colnum]=='O'&&flagx==1){
			return 0;
		}
		else if(arr[i][colnum]=='X'&&flago==0){
			flagx=1;
		}
		else if(arr[i][colnum]=='X'&&flago==1){
			return 0;
		}
		else if(arr[i][colnum]=='.'){
			return 0;
		}
	}
	return 1;
}


int checkRightDiagonal(char**arr,int board_size){
	int i;
	int flago=0,flagx=0;
	for(i=0;i<board_size;i++){
		if(arr[i][i]=='O'&&flagx==0){
			flago=1;
		}
		else if(arr[i][i]=='O'&&flagx==1){
			return 0;
		}
		else if(arr[i][i]=='X'&&flago==0){
			flagx=1;
		}
		else if(arr[i][i]=='X'&&flago==1){
			return 0;
		}
		else if(arr[i][i]=='.'){
			return 0;
		}
	}
	return 1;
}


int checkLeftDiagonal(char**arr,int board_size){
	int i;
	int flago=0,flagx=0;
	for(i=0;i<board_size;i++){
		if(arr[i][board_size-1-i]=='O'&&flagx==0){
			flago=1;
		}
		else if(arr[i][board_size-1-i]=='O'&&flagx==1){
			return 0;
		}
		else if(arr[i][board_size-1-i]=='X'&&flago==0){
			flagx=1;
		}
		else if(arr[i][board_size-1-i]=='X'&&flago==1){
			return 0;
		}
		else if(arr[i][board_size-1-i]=='.'){
			return 0;
		}
	}
	return 1;
}


int checkGameOver(char**arr,int board_size){
	int i;
	//checking row winner///////////
	for(i=0;i<board_size;i++){
		if(checkRow(arr,i,board_size)==1){
			return 1;
		}
	}
	//checking column winner////////
	for(i=0;i<board_size;i++){
		if(checkColumn(arr,i,board_size)==1){
			return 1;
		}
	}
	//checking diagonal winner////////
	if(checkRightDiagonal(arr,board_size)||checkLeftDiagonal(arr,board_size)){
		return 1;
	}
	return 0;
}

int main(void){
  int sockfd,i,j,k;
  struct sockaddr_in serv_addr;

  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  serv_addr.sin_port=htons(6000);

  printf("Enter the board size: ");
  scanf("%d",&k);

//the client keeps on playing until it receives a board in which someone has won

  while(1){
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
      printf("Socket creation error in client\n");
      exit(0);
    }

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
      printf("Unable to connect to server\n");
      exit(0);
    }
    char buff[1000];
    for(i=0; i < 1000; i++) {
      buff[i] = '\0';
    }
    recv(sockfd, buff, 1000, 0);

    printf("\n\nThe current board :\n\n");

    char**temp=(char**)malloc(k*sizeof(char*));
    for(i=0;i<k;i++){
      temp[i]=(char*)malloc(k*sizeof(char));
    }

    for(i=0;i<k;i++){
      for(j=0;j<k;j++){
        temp[i][j]=buff[k*i+j];
        printf("%c  ",temp[i][j]);
      }
      printf("\n\n");
    }

    if(checkGameOver(temp,k)){
      printf("\n\nGAME OVER !!!\n\nClient Terminated!\n");
      exit(0);
    }

    ////////freeing the temporary storage/////////////
    for(i=0;i<k;i++){
      free(temp[i]);
    }
    free(temp);

    printf("\n\nMake your move:\n\nWrite space separated row and column number: ");
    int x,y;
    scanf("%d %d",&x,&y);
    x--;
    y--;
    buff[k*x+y]='q';

    send(sockfd,buff,strlen(buff)+1,0);

  }

}
