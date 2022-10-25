//Connor Walsh 2376797
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int[] setOptionArr(char[] in){
     int i = 1;
     int op[4] = {0, 0, 0, 0}
     while(in[i] != '\0'){
          switch(in[i]){
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
     int option[4] = {1, 1, 1, 0};//{n, w, c, M}
     int BUFFER_SIZE = -1;
     char** currentFile = argv;
     currentFile++;
     switch (argc) {
          case 1:
               fprintf(stderr, "Input must follow format \"./MYLASTNAME_MYSTUDENTID.exe [OPTION]... BUFFER_SIZE [INPUT FILE]...\"");
               return 1;
               break;
          case 2:
               BUFFER_SIZE = atoi(argv[2]);
               if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)//check if BUFFER_SIZE was a valid size
                    return 1;
               currentFile = "prj1inp.txt";
               break;

          case 3:
               if(argv[1][0] == '-'){//check if there are option flags
                    op = setOptionArr(argv[1]);
                    BUFFER_SIZE = atoi(argv[2]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile = "prj1inp.txt";
               }
               else if(atoi(argv[1])){//check if number for BUFFER_SIZE
                    BUFFER_SIZE = atoi(argv[1]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile++;

               }
               else return 1;
               currentFile++;
               break;
          default:
               if(argv[1][0] == '-'){//check if there are option flags
                    op = setOptionArr(argv[1]);
                    currentFile++;
                    BUFFER_SIZE = atoi(argv[2]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile++;
               }
               else if(atoi(argv[1])){//check if number for BUFFER_SIZE
                    BUFFER_SIZE = atoi(argv[1]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
                    currentFile++;
               }
               else return 1;
               break;
     }
     int n, fd[2];
     char line[BUFFER_SIZE];
     pid_t pid = fork();
     if(pid < 0){
          fprintf(stderr, "Parent: Fork Failed");
	return 2;
	}
     else if(pid == 0){
          close(fd[1]);
          n = read(fd[0], line, BUFFER_SIZE);
     }

}
