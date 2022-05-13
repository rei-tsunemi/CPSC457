#include "stdio.h"
//demonstrating undefined behaviour and how to debug it

char* bad(){
    char buf[2];
    return buf;
}

int main(){
    char* buf = "hii";
    char buf2[] = "hii";

    printf("before\n");
    fflush(stdout);
    buf = bad();
    buf[0]='g';
    printf("after\n");
    fflush(stdout);
}
