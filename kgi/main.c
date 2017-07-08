#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
//#include <openssl/rand.h>

//extern char **environ;

#include "lib/sds/sds.c"
#include "lib/popenRWE/popenRWE.c"

/*
To Do:
1. Server will provide an argument which is the socket to client



*/
//unsigned int random_uint(unsigned int limit);
//int random_socket_path(char *path);

int main() {

    // Test Values
    /*
    putenv("DEFAULT_DIR=../htdocs");
    putenv("OBJECT_DIR=../objects");
    putenv("SCRIPT_PATH=../htdocs/hello.c");
    putenv("CLIENT_SOCKET=9");
    */
    char DEFAULT_DIR[512] = "htdocs", OBJECT_DIR[512] = "objects", SCRIPT_PATH[512] = "htdocs/hello.c";
    int CLIENT_SOCKET = -1;

    scanf("%s | %s | %s | %d", &DEFAULT_DIR, &OBJECT_DIR, &SCRIPT_PATH, &CLIENT_SOCKET);

    printf("\n==>DEFAULT_DIR: %s", DEFAULT_DIR);
    printf("\n==>OBJECT_DIR: %s", OBJECT_DIR);
    printf("\n==>SCRIPT_PATH: %s", SCRIPT_PATH);
    printf("\n==>CLIENT_SOCKET: %d", CLIENT_SOCKET);
    //return 0;

    /*
    if(getenv("DEFAULT_DIR") == NULL || getenv("SCRIPT_PATH") == NULL ||
       getenv("OBJECT_DIR") == NULL || getenv("CLIENT_SOCKET") == NULL){
        // Display Error Message
        printf("Error: Invalid Environment\n");
        return 0;
    }
    */


    //printf("\nKGI-CONNECTED! %s %s %s %d", DEFAULT_DIR, SCRIPT_PATH, OBJECT_DIR, CLIENT_SOCKET);

    char compiler_command[10240] = "", object_path[10240] = "";

    pid_t compiler_pid, object_pid;
    int compiler_pipe[3], object_pipe[3];
    char compiler_buffer[10240] = "", object_buffer[10240] = "";

    int param_count;
    sds param_sds, *param_token;

    // Compile Script
    param_sds = sdsnew(SCRIPT_PATH);
    param_token = sdssplitlen(param_sds, sdslen(param_sds), "/", 1, &param_count);

    // Compiled Object Path
    strcpy(object_path, OBJECT_DIR);
    strcat(object_path, "/");
    strcat(object_path, param_token[param_count-1]);
    strcat(object_path, ".kro");

    strcpy(compiler_command, "gcc -o ");
    strcat(compiler_command, object_path);
    strcat(compiler_command, " ");
    strcat(compiler_command, SCRIPT_PATH);

    printf("\nCompiler Command: %s\n", compiler_command);

    compiler_pid = popenRWE(compiler_pipe, compiler_command);
    close(compiler_pipe[0]);
    while(read(compiler_pipe[1], &compiler_buffer, sizeof(compiler_buffer))){
        printf("%s", compiler_buffer);
    }
    pcloseRWE(compiler_pid, compiler_pipe);



    // Check If Compiled File Exists
    if(access(object_path, F_OK) == -1){
        // File Does Not Exists
        printf("Error: Compilation Error!");
        return 0;
    }

    // Process Local Script
    //chdir(getenv("DEFAULT_DIR"));

    object_pid = popenRWE(object_pipe, object_path);

    // In Case of POST
    /*
    write(object_pipe[0], "Hello World", 11);
    close(object_pipe[0]);
    */
    close(object_pipe[0]);
    while(read(object_pipe[1], &object_buffer, sizeof(object_buffer))){
        printf("%s", object_buffer);
        send(CLIENT_SOCKET, object_buffer, strlen(object_buffer), 0);
    }
    close(CLIENT_SOCKET);
    pcloseRWE(object_pid, object_pipe);


    // Connect To Client

    // Run Local Script ( Application )

    // Send To Client

    return 0;
}

/*
Source: https://stackoverflow.com/a/31282121
*/
/*
unsigned int random_uint(unsigned int limit) {
    union {
        unsigned int i;
        unsigned char c[sizeof(unsigned int)];
    } u;

    do {
        if (!RAND_bytes(u.c, sizeof(u.c))) {
            fprintf(stderr, "Can't get random bytes!\n");
            exit(1);
        }
    } while (u.i < (-limit % limit));
    return u.i % limit;
}

int random_socket_path(char *path){
    int i;
    unsigned int j;

    path[0] = '\0';

    for(i = 1; i < 33; i++){
        j = random_uint[25];
        path[i] = "abcdefghijklmnopqrstuvwxyz"[j];
    }

    path[33] = '\0';
}
*/
