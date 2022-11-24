#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



void *workerThread(void *targv){
     printf("%d\n", targv);
}

int main(int argc, char** argv){
     int InpArray[12000];
     pthread_t pthread_main;
     pthread_t pthreads[12];
     FILE *fp;
     fp =  fopen(argv[1], "r");
     int i = 0;
     for(int i = 0; i < 12000; i++){
          InpArray[i] = getw(fp);
          printf("%d\n", InpArray[i]);
     }
     for(int j = 0; j < 12; j++){
          int param = j*1000;
          pthread_create(pthreads[j], NULL, workerThread,  &param);
          pthread_join(pthreads[j], NULL);
     }

     return 0;
}
