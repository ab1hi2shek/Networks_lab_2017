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
#include<signal.h>

//we are keeping control port as 50000
//we shall use semaphores to control SD from SC

#define P(s) semop(s,&pop,1);
#define V(s) semop(s,&vop,1);

int main(void){


  //////////////initializing semaphores//////////////////////////
	int sema1;
	struct sembuf pop,vop;

	pop.sem_num=vop.sem_num=0;
	pop.sem_flg=vop.sem_flg=0;
	pop.sem_op=-1;
	vop.sem_op=1;

  sema1=semget(IPC_PRIVATE,1,0777|IPC_CREAT);
  semctl(sema1,0,SETVAL,0);

  ////////////////////////////////////////////////////////////////

  /////////////////shared memory between SC and SD///////////////////

  int shmid2=shmget(IPC_PRIVATE,100*sizeof(char),0777|IPC_CREAT);//this shall be used for "get/put filename"
  char *put_get_SC,*put_get_SD;
  int shmid3=shmget(IPC_PRIVATE,10*sizeof(char),0777|IPC_CREAT);//this shall be used for port number Y
  int *sh_portY_SC,*sh_portY_SD;

  //////////////////////////////////////////////////////////////////


  if(fork()==0){
    //child process which will be the data process and will be controlled by the control process
    //process SD

    put_get_SD=shmat(shmid2,0,0);
    sh_portY_SD=shmat(shmid3,0,0);


  }

  else{
    //parent process which will be the control process
    //process SC

    put_get_SC=shmat(shmid2,0,0);
    sh_portY_SC=shmat(shmid3,0,0);

    int S1,newsockfd;
    int clilen;
    struct sockaddr_in serv_addr,cli_addr;

    if((S1=socket(AF_INET,SOCK_STREAM,0))<0){
      printf("Unable to create socket\n");
      exit(0);
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port=htons(50000);


    if(bind(S1,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
      printf("Unable to bind\n");
      exit(0);
    }

    listen(S1,1);
///////////////////////variables used ////////////////////////////////////
    char buff[100];
    int i;
    int j;
    char servermsg[20];
    char port_y[20];
    int port_y_int_value;
    int port_flag=0;
    char directory_name[100];
    char*arglist[3];//for cd command
//////////////////////////////////////////////////////////////////////////

  //the server keeps waiting on the control port
    while(1){

      clilen = sizeof(cli_addr);
      newsockfd = accept(S1, (struct sockaddr *) &cli_addr,&clilen) ;
      if (newsockfd < 0) {
          printf("Accept error\n");
          exit(0);
        }

        for(i=0; i < 100; i++) {
          buff[i] = '\0';
        }
        recv(newsockfd, buff, 100, 0); //buff now contains the control command

////////////////////////////PORT COMMAND CHECKING////////////////////////////////////////////

        //we will check whether we received the port command or not
        if(port_flag==0 && buff[0]=='p' && buff[1]=='o'&& buff[2]=='r' && buff[3]=='t'&& buff[4]==' '){
          //we have received port command
          i=5;
          j=0;

          //constructing port number y
          while(buff[i]!='\0'){
            port_y[j]=buff[i];
            j++;
            i++;
          }
          port_y[j]='\0';
          port_y_int_value=atoi(port_y);

          //now checking whether port number is valid or not

          if(port_y_int_value>1024 && port_y_int_value<65535){
            for(i=0; i < 20; i++) {
              servermsg[i] = '\0';
            }
            servermsg[0]='2';
            servermsg[1]='0';
            servermsg[2]='0';

            send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 200 if "port number" is valid

            port_flag=1;//signifies that port command was successful

          }//end of "if port and port number both are valid"

          else{
            for(i=0; i < 20; i++) {
              servermsg[i] = '\0';
            }
            servermsg[0]='5';
            servermsg[1]='5';
            servermsg[2]='0';

            send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 550 if "port number" is invalid

          }//end of "if port number is invalid"

        }//end of "if port command received"

        else{
          for(i=0; i < 20; i++) {
            servermsg[i] = '\0';
          }
          servermsg[0]='5';
          servermsg[1]='0';
          servermsg[2]='3';

          send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 503 if "port" not received


        }//end of "if port command not received"
////////////////////////////////PORT COMMAND CHECKING DONE//////////////////////////////////////////////////

  /////////////all the other commands will check whether port_flag=1 or not //////////////////////////////////


////////////////////////////// cd COMMAND CHECKING///////////////////////////////////////////////////////////

 else if(port_flag==1 && buff[0]=='c' && buff[1]=='d' && buff[2]==' '){
   i=3;
   j=0;
   while(buff[i]!='\0'){
     directory_name[j]=buff[i];
     i++;
     j++;
   }
   directory_name[j]='\0';
   arglist[0] = (char *)malloc(3*sizeof(char)); strcpy(arglist[0],"cd");
   arglist[1] = (char *)malloc((1+strlen(directory_name))*sizeof(char)); strcpy(arglist[1],directory_name);
   arglist[2] = NULL;

   //we will keep a shared flag variable to check whether the command was successful
   int shmid;
   shmid=shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);
   int *a,*b;
   a=(int *)shmat(shmid,0,0);
   *a=0;
//child process will run execvp()
   if(fork()==0){
     execvp("cd",arglist);
     b=(int *)shmat(shmid,0,0);
     *b=1;
     shmdt(b);
     exit(0);
   }

   if(*a==1){
     //means execvp() failed

     for(i=0; i < 20; i++) {
       servermsg[i] = '\0';
     }
     servermsg[0]='5';
     servermsg[1]='0';
     servermsg[2]='1';

     send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 501 if cd command failed


   }//if cd failed

   else{
     //means execvp() worked
     for(i=0; i < 20; i++) {
       servermsg[i] = '\0';
     }
     servermsg[0]='2';
     servermsg[1]='0';
     servermsg[2]='0';

     send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 200 if cd command successful

   }// if cd succeeded

    shmdt(a);
    shmctl(shmid, IPC_RMID, 0);

 }

////////////////////////////// cd COMMAND CHECKING DONE///////////////////////////////////////////////////////////

/////////////////////////////get COMMAND CHECKING//////////////////////////////////////////////////////////////

  else if(port_flag==1 && buff[0]=='g' && buff[1]=='e' && buff[2]=='t' && buff[3]==' '){

  char filename[100];
  i=4;
  j=0;
  while(buff[i]!='\0'){
    filename[j]=buff[i];
    i++;
    j++;
  }
  filename[j]='\0';

  FILE*fp;
  if(fopen(filename,"r")==NULL){
    for(i=0; i < 20; i++) {
      servermsg[i] = '\0';
    }
    servermsg[0]='5';
    servermsg[1]='5';
    servermsg[2]='0';

    send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 550 if file could not be opened

  }//if opening file was unsuccessful
  else{
    for(i=0;i<100;i++){
      put_get_SC[i]='\0';
    }
    strcpy(put_get_SC,buff);//copying  "get filename" to shared memory

    ////////////ACTIVATE SD PROCESS/////////////////////////////////////


  }//if opening file was successful

}


////////////////////////////get COMMAND CHECKING DONE/////////////////////////////////////////

///////////////////////////put COMMAND CHECKING//////////////////////////////////////


  else if(port_flag==1 && buff[0]=='p' && buff[1]=='u' && buff[2]=='t' && buff[3]==' '){

  char filename[100];
  i=4;
  j=0;
  while(buff[i]!='\0'){
    filename[j]=buff[i];
    i++;
    j++;
  }
  filename[j]='\0';

    for(i=0;i<100;i++){
      put_get_SC[i]='\0';
    }
    strcpy(put_get_SC,buff);//copying  "get filename" to shared memory

    ////////////ACTIVATE SD PROCESS/////////////////////////////////////


}

///////////////////////////put COMMAND CHECKING DONE////////////////////////////////////

//////////////////////////quit COMMAND CHECKING/////////////////////////////////////////

  else if(port_flag==1 && buff[0]=='q' && buff[1]=='u' && buff[2]=='i' && buff[3]=='t'){
    for(i=0; i < 20; i++) {
      servermsg[i] = '\0';
    }
    servermsg[0]='4';
    servermsg[1]='2';
    servermsg[2]='1';

    send(newsockfd,servermsg,strlen(servermsg)+1,0);//sending 421 if quit command received

  }



/////////////////////////quit COMMAND CHECKING DONE////////////////////////////////////

    }//end of while
    shmctl(shmid2, IPC_RMID, 0);
    shmctl(shmid2, IPC_RMID, 0);

  }//end of control process SC


}//end of main
