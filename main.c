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
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <resolv.h>

// External Libraries
//#include "lib/sds/sds.h"
//#include "lib/sds/sdsalloc.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/evp.h>

// Include Required File
#include "lib/sds/sds.c"
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
    int MainSocket, SecureSocket, AcceptSocket;

    // Request Data
    rqpack request_data;

    // File
    FILE *fp;
    char *file_type = malloc(128);
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
        file_size = getFileSize(fp); // Get File Size
        send_response_header(AcceptSocket, 200, file_type, file_size); // Send Header
        send_response_file(AcceptSocket, fp, file_size, server_conf.max_buffer); // Send File To Client
        fclose(fp); // Close File
    }else{
        send_response_error(AcceptSocket, 404, server_conf, 0); // No Debug Print
    }


    close(AcceptSocket); // Close Connection
    goto listen_for_client; // Repeat Process

    return 0;
}
