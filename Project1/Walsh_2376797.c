#include <stdio.h>
#include <stdlib.h>

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
     switch (argc) {
          case 1:
               printf("Input must follow format \"./MYLASTNAME_MYSTUDENTID.exe [OPTION]... BUFFER_SIZE [INPUT FILE]...\"");
               return 1;
               break;
          case 2:
               BUFFER_SIZE = atoi(argv[2]);
               if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)//check if BUFFER_SIZE was a valid size
                    return 1;

          case 3:
               if(argv[1][0] == '-'){//check if there are option flags
                    op = setOptionArr(argv[1]);
                    BUFFER_SIZE = atoi(argv[2]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;
               }
               else if(atoi(argv[1])){//check if number for BUFFER_SIZE
                    BUFFER_SIZE = atoi(argv[1]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;

               }
               else return 1;
               break;
          default:
               if(argv[1][0] == '-'){//check if there are option flags
                    op = setOptionArr(argv[1]);
                    BUFFER_SIZE = atoi(argv[2]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;                    }
               else if(atoi(argv[1])){//check if number for BUFFER_SIZE
                    BUFFER_SIZE = atoi(argv[1]);
                    if(BUFFER_SIZE < 64 || BUFFER_SIZE > 256)
                         return 1;

               }
               else return 1;
               break;
     }
}
