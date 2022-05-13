#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


//how to read all the contents of a file

#define BUFSIZE 3

int main()
{
    int fd = open("dog", O_RDWR);

    char buf[BUFSIZE];
    
    int tempBytes;
    int totalBytesRead=0;
    while (1){
        tempBytes = read(fd, buf, BUFSIZE);
        if (tempBytes<=0)
            break;
        totalBytesRead+=tempBytes;
        printf("%.*s", tempBytes, buf);
    }

    close(fd);
}
