//clone in ssh linuxlab to avoid file format issues in future

// #include <stdio.h>
// #include <stdlib.h>

// #define BUF_SIZE 1000

// int main() {
//     FILE * file;
//     file = popen("ls", "r");

//     char buf[BUF_SIZE];
//     char* retOfFgets;
//     retOfFgets = fgets(buf, BUF_SIZE, file);
//     if(retOfFgets) {
//         printf("fgets failed, exiting\n");
//         exit(1);
//     }

//     printf("read '%s'\n", buf);

//     pclose(file);
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int main() {
//     printf("A|\n");
//     int forkRet = fork();
//     if(forkRet == 0) {
//         printf("i'm in child|");
//     }
//     else {
//         printf("i'm in parent|");
//     }
//     printf("i'm done|");
// }

// int main() {
//     //printf("A\n");
//     fork();
//     //printf("B\n");
//     fork();
//     //printf("C\n");
//     fork();
//     fork();

//     printf("i'm done\n");
// }

int main() {
    printf("hi\n");
    int execRet;
    // execRet = execl("/usr/bin/ls", "ls", "-la", NULL);

    char *buf[] = {"/usr/bin/ls", "ls", "-la", NULL};
    execRet = execv("/usr/bin/ls", buf);

    if(execRet == -1) {
        printf("execRet failed\n");
    }

    printf("Bye\n");
}