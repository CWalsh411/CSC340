#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
int add(int i, int j) {
     printf("Add is %d\n", i+j);
     return i+j;
}
int subtract(int i, int j) {
     printf("Subtract is %d\n", i-j);
     return i-j;
}
int multiply(int i, int j) {
     printf("Multiply is %d\n", i*j);
     return i*j;
}
int divide(int i, int j) {
     printf("Divide is %d\n", i/j);
     return i/j;
}
int modulo (int i, int j) {
     printf("Modulo is %d\n", i%j);
     return i%j;
}
int main(int argc, const char *argv[]) {
     int (*comp[5]) (int i, int j) = {add, subtract, multiply, divide, modulo};
     for (int i = 0; i < 5; ++i)
          printf( "%d\n", (*comp[i]) (2,3));
     return 0;
}
