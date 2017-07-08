/*
Project: Khudro Web-Server ( Linux )

By: Arman Hossain
CSE 11'th Batch,
Shanto-Mariam University of Creative Technology
Dhaka, Bangladesh

GitHub: https://github.com/arman-bd/khudro-linux
LinkedIn: https://www.linkedin.com/in/armanhossain/
*/

char __ServerName[128] = "Khudro";
char __ServerVersion[16] = "0.1.1";
char __ServerOS[32] = "Linux";

#define FAIL    -1

// Include Required Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <resolv.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

// External Libraries
//#include "lib/sds/sds.h"
//#include "lib/sds/sdsalloc.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/evp.h>

extern char **environ;

// Include Required File
#include "lib/sds/sds.c"
#include "lib/popenRWE/popenRWE.c"
#include "config.c"
#include "mime-list.c"
#include "function.c"
#include "request.c"
#include "response.c"
#include "http.c"
#include "https.c"

int main(){

    printf("::: %s v%s - %s :::\n\n", __ServerName, __ServerVersion, __ServerOS);

    // Declare Socket Parameters
    int bytesRecv = -1;
    int MainSocket = -1, AcceptSocket = -1;
    char strSock[8];
    //int SecureSocket;

    // Request Data
    rqpack request_data;

    // KGI
    pid_t pid;
    int kpipe[3];
    char kbuffer[1024] = "";

    // File
    FILE *fp;
    char *file_type = malloc(128);
    char kgi_path_call[512] = "";
    size_t file_size = 0;



    // Load Default Configuration
    s_conf server_conf;
    server_conf = parse_config("khudro.conf", 1);



    if(server_conf.default_port > 0){
        MainSocket = ListenHTTP(server_conf);
    }

    if(server_conf.ssl_port > 0){
        //SecureSocket = ListenHTTPS(server_conf);
    }

/*
    struct sockaddr_in service, cli_addr;
    service.sin_family = AF_INET; // AF_INET is the Internet address family.
    service.sin_addr.s_addr = inet_addr(server_conf.default_ip); // Local IP
    service.sin_port = htons(server_conf.default_port);// Local Port

    MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(MainSocket < 0){
        printf("Error: Socket Error - [ Invalid Socket ]\n");
        return 0;
    }else{
        printf("Server: Socket Loaded\n");
    }

    // Bind Socket
    if(bind(MainSocket, (struct sockaddr *)&service, sizeof(service)) < 0){
        printf("Error: Binding Failed.\n");
        printf("       Check If Port Is Already In Use.\n");
        close(MainSocket);
        return 0;
    }else{
        printf("Server: Binded Successfully\n");
        printf("Server: IP [ %s ]\n", server_conf.default_ip);
    }

    if(listen(MainSocket, 5) < 0){
       printf("Error: Unable To Listen On Port [ %d ].\n", server_conf.default_port);
    }else{
       printf("Server: Listening On Port [ %d ]\n", server_conf.default_port);
    }

    printf("\n");
*/

    // Listen For Request

    struct sockaddr_in cli_addr;
    socklen_t clilen;
    clilen = sizeof(cli_addr);

    listen_for_client:

    while(1){
        AcceptSocket = -1;

        while(AcceptSocket < 0){
            AcceptSocket = accept(MainSocket, (struct sockaddr*)&cli_addr, &clilen);
        }

        break;
    }

    // Receive Data From Client
    bytesRecv = process_request(AcceptSocket, server_conf, &request_data);


    if(bytesRecv < 0){
        // PING Request
        close(AcceptSocket); // Close Socket
        goto listen_for_client; // Repeat Process
    }

    if((fp = fopen(request_data.file_path, "rb")) != NULL){
        // File Found, Now Get Some Information About File And Send It
        get_mime_type(request_data.file_name, file_type); // Get MIME Type

        if(strcmp(file_type, "text/x-c") == 0 && access(server_conf.kgi_path, F_OK) != -1){
            // Send It To KGI
            printf("\nKGI: Calling...");
            /*
            setEnv("DEFAULT_DIR", server_conf.default_dir);
            setEnv("OBJECT_DIR", server_conf.object_dir);
            setEnv("SCRIPT_PATH", request_data.file_path);
            sprintf(strSock, "%d", AcceptSocket);
            setEnv("CLIENT_SOCKET", strSock);
            printf("\nKGI: Environment Set!");
            // SET MORE ENV!!
            */
            strcpy(kgi_path_call, server_conf.kgi_path);
            //strcat(kgi_path_call, " > yahoo.txt");
            //system(kgi_path_call);

            printf("\nKGI: %s", kgi_path_call);
            printf("\nENV[0]: %s\n", environ[0]);

            pid = popenRWE(kpipe, kgi_path_call);

            sprintf(strSock, "%d", AcceptSocket);

            write(kpipe[0], server_conf.default_dir, strlen(server_conf.default_dir)*sizeof(char));
            write(kpipe[0], " | ", 3*sizeof(char));
            write(kpipe[0], server_conf.object_dir, strlen(server_conf.object_dir)*sizeof(char));
            write(kpipe[0], " | ", 3*sizeof(char));
            write(kpipe[0], request_data.file_path, strlen(request_data.file_path)*sizeof(char));
            write(kpipe[0], " | ", 3*sizeof(char));
            write(kpipe[0], strSock, strlen(strSock)*sizeof(char));

            close(kpipe[0]);

            printf("\nRead [ 01 ]");
            while(read(kpipe[1], &kbuffer, sizeof(kbuffer))){
                printf("%s", kbuffer);
            }

            printf("\nRead [ 02 ]");
            while(read(kpipe[2], &kbuffer, sizeof(kbuffer))){
                printf("%s", kbuffer);
            }
            pcloseRWE(pid, kpipe);
            close(AcceptSocket);

            //fpipe = popen(kgi_path_call, "r");
            //pipenum = fileno(fpipe);
            //fcntl(pipenum, F_SETFL, O_NONBLOCK);
            //pclose(fpipe);
            //pid = fork();
            //if(pid == 0){
            //    execle(kgi_path_call, NULL, environ);
            //    printf("\nKGI__: %s", kgi_path_call);
            //    return 0;
            //}else{
            //    printf("\nKGI--: %s", kgi_path_call);
            //}



            printf("\nKGI: Call Finished!");

            // DO NOT CLOSE SOCKET!!
        }else{
            // Handle It In Static Handler
            file_size = getFileSize(fp); // Get File Size
            send_response_header(AcceptSocket, 200, file_type, file_size); // Send Header
            send_response_file(AcceptSocket, fp, file_size, server_conf.max_buffer); // Send File To Client
            close(AcceptSocket); // Close Connection
        }
        fclose(fp); // Close File
    }else{
        send_response_error(AcceptSocket, 404, server_conf, 0); // No Debug Print
        close(AcceptSocket); // Close Connection
    }

    //close(AcceptSocket); // Close Connection
    goto listen_for_client; // Repeat Process

    return 0;
}
