//Connor Walsh 2376797
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <time.h>

const int MSG_SIZE = 128;
const int MSG_CAPACITY = 50;

int* setOptionArr(char* in){
     int i = 1;
     int op[4] = {0, 0, 0, 0};
     while(in[i] != '\0'){
          switch(in[i]){//switch to set option flags
               case 'n':
                    op[0] = 1;
                    i++;
                    break;

               case 'w':
                    op[1] = 1;
                    i++;
                    break;

               case 'c':
                    op[2] = 1;
                    i++;
                    break;

               case 'M':
                    op[3] = 1;
                    i++;
                    break;
          }
     }
     return op;
}


int main(int argc, char** argv){
     int *option;//{n, w, c, M}
     int defaultop[4] = {1, 1, 1, 0};
     int BUFFER_SIZE = -1;
     int totalNumOfFiles = 0;
     char** currentFile = argv;
     currentFile++;//Increment because argv[0] is program name
     FILE* fp;
     char buf[BUFFER_SIZE];
     unsigned int prio;
     struct timespec timeout = {0, 0};
     switch (argc) {//switch to determine commandline aguments
          case 1://error not enough arguments
               fprintf(stderr, "Input must follow format \"./MYLASTNAME_MYSTUDENTID.exe [OPTION]... BUFFER_SIZE [INPUT FILE]...\"");
               return 1;
               break;
          case 2://only buffer should be in commandline
               option = defaultop;
               BUFFER_SIZE = atoi(argv[2]);
               if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)//check if BUFFER_SIZE was a valid size
                    return 1;
               *currentFile = "prj1inp.txt";
               totalNumOfFiles = 1;
               break;

          case 3://either buffer and input file or options and buffer in commandline
               if(argv[1][0] == '-'){//check if there are option flags
                    option = setOptionArr(argv[1]);
                    BUFFER_SIZE = atoi(argv[2]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    *currentFile = "prj1inp.txt";
                    totalNumOfFiles = 1;
               }
               else if(atoi(argv[1])){//check if number for BUFFER_SIZE
                    option = defaultop;
                    BUFFER_SIZE = atoi(argv[1]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile++;
                    totalNumOfFiles = 1;

               }
               else return 1;
               currentFile++;
               break;
          default://can have any combination of valid inputs
               if(argv[1][0] == '-'){//check if there are option flags
                    option = setOptionArr(argv[1]);
                    currentFile++;
                    BUFFER_SIZE = atoi(argv[2]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile++;
                    totalNumOfFiles = argc - 3;
               }
               else if(atoi(argv[1])){//check if number for BUFFER_SIZE
                    option = defaultop;
                    BUFFER_SIZE = atoi(argv[1]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile++;
                    totalNumOfFiles = argc - 2;
               }
               else return 1;
               break;
     }
     int n, fd[2];
     pid_t pid = fork();//create processe and fork
     if(pipe(fd) < 0)
          return 2;
     struct mq_attr attr;
     attr.mq_maxmsg = MSG_CAPACITY;
     attr.mq_msgsize = MSG_SIZE;
     mqd_t mqd = mq_open("cqueue",  O_CREAT | O_RDWR, 0644, &attr);//open message queue
     if (mqd == (mqd_t) -1){
          printf("Parent:  mq_open error\n");
          mq_unlink("cqueue");
          return 2;
      }
     if(pid < 0){//check if process was created with no error
          fprintf(stderr, "Parent: Fork Failed\n");
	     return 2;
	}
     else if(pid == 0){//child process
          close(fd[1]);
          char line[BUFFER_SIZE];
          int NEWLINE_COUNT = 0, WORD_COUNT = 0, CHAR_COUNT = 0, MAX_LINE = 0, currentLine = 0;
          while((n = read(fd[0], line, BUFFER_SIZE)) > 0){//read from parent
               for(int i = 0; i < BUFFER_SIZE; i++){//loop through input and do counts
                    CHAR_COUNT++;
                    currentLine++;
                    if(line[i] == '\n'){
                         NEWLINE_COUNT++;
                         if(currentLine > MAX_LINE)
                              MAX_LINE = currentLine;
                         currentLine = 0;
                         WORD_COUNT++;
                    }
                    if(line[i] == ' ' || !line[i])
                         WORD_COUNT++;
               }
          }
          close(fd[0]);
          if(option[0]){//send meesage in message queue if -n
               if (mq_timedsend(mqd, "INPUT_FILE:\t newline count is:\t " + NEWLINE_COUNT, 38, 1, &timeout) == -1){
	  	           printf("Child: mq_send error");
  		           return 4;
  		     }
          }
          if(option[1]){//send meesage in message queue if -w
               if (mq_timedsend(mqd, "INPUT_FILE:\t word count is:\t " + WORD_COUNT, 32, 1, &timeout) == -1){
	  	           printf("Child: mq_send error");
  		           return 4;
  		     }
          }
          if(option[2]){//send meesage in message queue if -c
               if (mq_timedsend(mqd, "INPUT_FILE:\t character count is:\t " + CHAR_COUNT, 38, 1, &timeout) == -1){
	  	           printf("Child: mq_send error");
  		           return 4;
  		     }
           }
          if(option[3]){//send meesage in message queue if -M
               if (mq_timedsend(mqd, "INPUT_FILE:\t maximum line length is:\t " + MAX_LINE, 42, 1, &timeout) == -1){
	  	           printf("Child: mq_send error");
  		           return 4;
  		     }
          }
          write(STDOUT_FILENO, "Child: Terminating.\n", 20);
     }
     else{//Parent process
          for(int i = 0; i < totalNumOfFiles; i++){//loop through input files
               fp = fopen(*currentFile, "r");
               close(fd[0]);
               while(read(fp, buf, BUFFER_SIZE)){//read from input
                    write(fd[1], buf, BUFFER_SIZE);//write to child
               }
               currentFile++;
          }
          close(fd[1]);
          wait(NULL);//wait for child to finish
          ssize_t numRead;
          while(numRead = mq_timedreceive(mqd, buf, attr.mq_msgsize, &prio, &timeout) > 0)//read from message queue
               write(STDOUT_FILENO, buf, (size_t) numRead);//output from child
          write(STDOUT_FILENO, "Parent: Terminating.\n", 21);
     }
     return 0;

}
