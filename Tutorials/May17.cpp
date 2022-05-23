//check cpu info with lscpu

/* PIPING
 * 0 is stdin
 * 1 is stdout
 * 2 is stderr
 * 
 * we can do > to redirect stdout and 2> to redirect stderr
 * also < redirects stdin and >> appends stdout
 * use &> or >& to put stdout and stderr to a file
 * in Unix/Linux everything is a file
 * a special file is provided in order to make I/O devices look like files
 * thus they can be 
 * 
 * can do < file > file2
 * can do command1 | command2
 */

/*
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFF_SIZE 13

int main() {
    int fd = open("dog", O_RDWR);

    char buf[BUFF_SIZE];
    //char* buf2 = malloc(BUFF_SIZE*sizeof(char));

    int bytesRead;

    //sizeof(buf)/sizeof(buf[0]) trick only works for stack allocated 1D arrays
    //bytesRead = read(fd, buf, sizeof(buf)/sizeof(buf[0]));
    bytesRead = read(fd, buf, BUFF_SIZE);
    printf("bytes read = %d, string read = %.*s\n", bytesRead, buf);

    bytesRead = read(fd, buf, BUFF_SIZE);
    printf("bytes read = %d, string read = %.*s\n", bytesRead, buf);

    off_t lseekRet;
    int myOffset = 3;
    lseekRet = lseek(fd, myOffset, SEEK_SET);

    printf("lseekRet = %d\n", lseekRet);

    ssize_t writeRet;
    char buf1[] = "hii";
    writeRet = write(fd, buf1, sizeof(buf));

    close(fd);
}
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define TRIVIAL_DIR(buf) (((buf)[0] == '.' && (buf)[1] == '\0') || ((buf)[0] == '.' && (buf)[1] == '.' && (buf)[2] == '\0'))

int main() {
    DIR* dir;
    //open the current directory
    dir = opendir(".");

    struct dirent* dirContent;
    //struct dirent dirContentStack;
    //dirContentStack.d_name;

    int b;
    int chdirRet;

    //
    while((dirContent = readdir(dir)) != NULL) {
        b = dirContent->d_type == DT_DIR && !TRIVIAL_DIR(dirContent->d_name);
        if(b) {
            //closedir(dir);
            chdirRet = chdir(dirContent->d_name);
            if(chdirRet != 0) {
                printf("changing to directory %s fails\n", dirContent->d_name);
                break;
            }

            printf("changed directory to '%s'\n", dirContent->d_name);
            closedir(dir);  //close directory here to avoid double freeing
            dir = opendir(".");
        }
    }

    closedir(dir);
}