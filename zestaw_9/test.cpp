#include<stdlib.h>
#include<stdio.h>
// buffer must have length >= sizeof(int) + 1
// Write to the buffer backwards so that the binary representation
// is in the correct order i.e.  the LSB is on the far right
// instead of the far left of the printed string
char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);
    
    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';
        
        a >>= 1;
    }
    
    return buffer;
}

#define BUF_SIZE 33

int main() {
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';
    
    int b = 1<<31;
    b -=1;
    int2bin( b, buffer, BUF_SIZE - 1);
    
    printf("a = %s", buffer);
}