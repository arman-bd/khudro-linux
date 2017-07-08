#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "popenRWE.c"

int main(){
    pid_t pid;
    int mypipe[3];
    char buffer[1024];

    putenv("GET=WORLD");

    pid = popenRWE(mypipe, "./client");

    write(mypipe[0], "Yahoo", 5);
    write(mypipe[0], "\n", 1);
    close(mypipe[0]);

    while(read(mypipe[1], &buffer, 1024)){
        printf("%s", buffer);
    }

    return 0;
}
