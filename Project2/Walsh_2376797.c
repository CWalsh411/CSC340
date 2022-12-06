#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int sumResults[12];
int productResults[12];
int geometricResults[12];
typedef struct PointerStruct{
     int *inarr;
     int *sumarr;
     int *prodarr;
     int *geoarr;
}PointerStruct;

void *sum(void *targv){
     PointerStruct *ps = (PointerStruct *)targv;
     int *index =  ps.inarr;
     int *result = ps.sumarr;
     int partialSum = 0;
     for(int i = 0; i < 1000; i++){
          partialSum += *index;
          index++;
     }
     *result = partialSum;

}

void *product(void *targv){
     PointerStruct *ps = (PointerStruct *)targv;
     int *index =  ps.inarr;
     int *result = ps.proarr;
     int partialprod = 0;
     int partialSum = 0;
     for(int i = 0; i < 1000; i++){
          partialSum += *index;
          partialprod *= *index;
          index++;
     }
     *result = partialprod / partialSum; 
}

void *geometric(void *targv){
     int *index = (int *)targv;
     printf("Geometric %d\n", *index);
}

void *workerThread(void *targv){
     int *index = (int *)targv;
     pthread_t sumThread, productThread, geometricThread;
     pthread_create(&sumThread, NULL, sum, targv);
     pthread_create(&productThread, NULL, product, targv);
     pthread_create(&geometricThread, NULL, geometric, targv);
     pthread_join(sumThread, NULL);
     pthread_join(productThread, NULL);
     pthread_join(geometricThread, NULL);
}

int main(int argc, char** argv){
     int InpArray[12000];
     pthread_t pthread_main;
     pthread_t pthreads[12];
     PointerStruct ps;
     ps.inarr = InpArray;
     ps.sumarr = sumResults;
     ps.prodarr = productResults;
     ps.geoarr = geometricResults;
     FILE *fp;
     fp =  fopen(argv[1], "r");
     int i = 0;
     for(int i = 0; i < 12000; i++){
          fscanf(fp, "%d", &InpArray[i]);
     }
     for(int j = 0; j < 12; j++){
          ps.inarr += (i * 1000 * sizeof(InpArray));
          ps.sumarr += (i * 1000 * sizeof(sumResults));
          ps.prodarr += (i * 1000 * sizeof(productResults));
          ps.geoarr += (i * 1000 * sizeof(geometricResults));
          pthread_create(&pthreads[j], NULL, workerThread, (void *)&ps);
          pthread_join(pthreads[j], NULL);
     }

     return 0;
}
