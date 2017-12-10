/*
Project: Khudro Web-Server ( Linux )

By: Arman Hossain
CSE 11'th Batch,
Shanto-Mariam University of Creative Technology
Dhaka, Bangladesh

GitHub: https://github.com/arman-bd/khudro-linux
LinkedIn: https://www.linkedin.com/in/armanhossain/
*/



#define FAIL    -1

// Include Required Headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <resolv.h>
#include <pthread.h>
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

#include "lib/map_lib/map_lib.h"
#include "lib/map_lib/map_lib.c"

char __ServerName[128] = "Khudro";
char __ServerVersion[16] = "0.1.1";
char __ServerOS[32] = "Linux";
int GlobalSock = -1;
char GlobalFileName[1024];
char GlobalFilePath[4094];
char GlobalQuery[4096];
char GlobalMethod[8];
char GlobalFileType[64];

// Include Required File
#include "lib/sds/sds.c"
#include "lib/popenRWE/popenRWE.c"

#include "config.c"
s_conf server_conf;

#include "mime-list.c"
#include "function.c"
#include "request.c"
#include "response.c"
#include "http.c"
#include "https.c"
#include "kgi.c"



void handle_static_file();

int main(){

    printf("::: %s v%s - %s :::\n\n", __ServerName, __ServerVersion, __ServerOS);

    // Declare Socket Parameters
    int bytesRecv = -1;
    int MainSocket = -1, AcceptSocket = -1;
    //char strSock[8];


    // Cleanup Globals
    GlobalSock = -1;


    // Request Data
    //rqpack request_data;

    // KGI
    //pid_t pid;
    //int kpipe[3];
    //char kbuffer[1024] = "";

    // Khudro Gateway Interface
    pthread_attr_t tattr;
    pthread_t kgi_thread, static_thread;

    pthread_attr_init(&tattr);
    pthread_attr_setstacksize(&tattr, 1 * 1024);
    pthread_attr_setdetachstate(&tattr, 1);


    // File
    //FILE *fp;
    //char *file_type = malloc(128);
    //char kgi_path_call[512] = "";
    //size_t file_size = 0;

    // Load Default Configuration
    //s_conf server_conf;
    server_conf = parse_config("khudro.conf", 1);

    if(server_conf.default_port > 0){
        MainSocket = ListenHTTP();
    }

    if(server_conf.ssl_port > 0){
        //SecureSocket = ListenHTTPS(server_conf);
    }

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
    //bytesRecv = process_request(AcceptSocket, server_conf, &request_data);
    bytesRecv = process_request(AcceptSocket);


    if(bytesRecv < 0){
        // PING Request
        close(AcceptSocket); // Close Socket
        goto listen_for_client; // Repeat Process
    }

    //if(access(request_data.file_path, F_OK) != -1){
    if(access(GlobalFilePath, F_OK) != -1){
        // File Found, Now Get Some Information About File And Send It

        //get_mime_type(request_data.file_name, file_type); // Get MIME Type
        get_mime_type(GlobalFileName, GlobalFileType); // Get MIME Type

        if(strcmp(GlobalFileType, "text/x-c") == 0 || strcmp(GlobalFileType, "text/x-cpp") == 0){
            // Send It To KGI

            //printf("\nKGI: Calling... ");

            //strcpy(GlobalFilePath, request_data.file_path);
            //strcpy(GlobalQuery, request_data.query);
            //strcpy(GlobalFileType, file_type);
            GlobalSock = AcceptSocket;
            if(pthread_create(&kgi_thread, &tattr, &khudro_gateway_interface, NULL)){
                printf("\nError: Creating Thread");
                return 1;
            }

            //pthread_detach(kgi_thread);

            //printf("\nKGI: Call Finished!\n");

            // DO NOT CLOSE SOCKET!!
        }else{
            // Handle It In Static Handler
            //strcpy(GlobalFilePath, request_data.file_path);
            //strcpy(GlobalQuery, request_data.query);
            //strcpy(GlobalFileType, file_type);
            GlobalSock = AcceptSocket;
            if(pthread_create(&static_thread, &tattr, &handle_static_file, NULL)){
                printf("\nError: Creating Thread");
                return 1;
            }
            //pthread_detach(static_thread);
        }

        //GlobalSock = -1;
        //free(GlobalFileName);
        //free(GlobalFilePath);
        //free(GlobalQuery);
    }else{
        send_response_error(AcceptSocket, 404, server_conf, 0); // No Debug Print
        close(AcceptSocket); // Close Connection

    }

    //free(request_data.file_name);
    //free(request_data.file_path);
    //free(request_data.query);
    //printf("-> %u", &request_data->requested_uri);


    //GlobalSock = -1;
    //free(GlobalFileName);
    //free(GlobalFilePath);
    //free(GlobalQuery);

    fflush(stdin);
    fflush(stdout);

    //close(AcceptSocket); // Close Connection
    goto listen_for_client; // Repeat Process

    return 0;
}

void handle_static_file(){
    FILE *fp;
    size_t file_size;

    int LocalSock = GlobalSock;
    char LocalFilePath[4096];
    char LocalFileType[64];

    strcpy(LocalFilePath, GlobalFilePath);
    strcpy(LocalFileType, GlobalFileType);


    fp = fopen(LocalFilePath, "rb");
    file_size = getFileSize(fp); // Get File Size
    send_response_header(LocalSock, 200, LocalFileType, file_size); // Send Header
    send_response_file(LocalSock, fp, file_size, server_conf.max_buffer); // Send File To Client
    close(LocalSock); // Close Connection
    fclose(fp); // Close File


}
