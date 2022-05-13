#include "stdio.h"

static void f(){
    printf("in f\n");
    static int a;
}

static int a;

//int main(){}
