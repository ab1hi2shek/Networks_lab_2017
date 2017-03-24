/*//////////////////////////////////////////

Names: Somesh Khandelia & Subhranil Mondal
Roll no.: 14/CS/105 & 13/CS/111
Assignment No.6
Group no. B24

*////////////////////////////////////////////

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
	int i,j;
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

int checkBoardFull(char**arr,int board_size){
	int flag=0;
	int i,j;
  for(i=0;i<board_size;i++){
    for(j=0;j<board_size;j++){
      if(arr[i][j]=='.'){
        flag=1;
        break;
      }
    }
  }
  if(flag==0){
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

	printf("\n\n*********************************************************\n\n");
  printf("\n\nEnter the board size: ");
  scanf("%d",&k);

  if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
    printf("Socket creation error in client\n");
    exit(0);
  }

  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    printf("Unable to connect to server\n");
    exit(0);
  }

//the client keeps on playing until it receives a board in which someone has won

  while(1){

    char buff[1000];
    for(i=0; i < 1000; i++) {
      buff[i] = '\0';
    }

    printf("\n\nClient ready to receive the board from server\n\n");

		printf("\n\n*********************************************************\n\n");
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

    if(checkGameOver(temp,k)||checkBoardFull(temp,k)){
      printf("\n\nGAME OVER !!!\n\nClient Terminated!\n");
      exit(0);
    }

    ////////freeing the temporary storage/////////////
    for(i=0;i<k;i++){
      free(temp[i]);
    }
    free(temp);
    int x,y;
    do{
      printf("\n\n************************Make your valid move*************************\n\nWrite space separated row and column number: ");

      scanf("%d %d",&x,&y);
      if(x>k||x<1||y>k||y<1){
        printf("\n\nInvalid move!!\n\n");
        continue;
      }
      x--;
      y--;
      if(buff[k*x+y]=='.'){
        buff[k*x+y]='q';
      }
      else{
        printf("\n\nInvalid move!!\n\n");
      }


    }while(buff[k*x+y]!='q');

    send(sockfd,buff,strlen(buff)+1,0);

  }

}
