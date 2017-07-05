/*
07-06-2017:
Adding SDS Library to Parse Headers


*/


typedef struct rqpack {
    // Request Parameter
    char request_method[9];
    int compressed;

    // File Parameter
    char *requested_uri;
    char *file_name;
    char *file_path;
    char *query;
    char *host;
}rqpack;

int process_file_path(rqpack *request_data, s_conf config_store, char *file_name);

int process_request(int AcceptSocket, s_conf config_store, rqpack *request_data){

    int i, j;
    //char file_name[2084]; // Maximum URL Length
    //char file_path[2596];
    int bytesRecv = -1;

    char *buffer_data;
    //char *receive_array;

    // SDS Library
    int  header_count, param_count;
    sds header_sds, param_sds, *header_token, *param_token;

    buffer_data = malloc(config_store.receive_buffer + 1);

    // Receive Data From Client
    bytesRecv = recv(AcceptSocket, buffer_data, config_store.receive_buffer, 0);

    //printf("\n>bytesRecv : %d", bytesRecv);
    //printf("\n>buffer_data : %s", buffer_data);

    if(bytesRecv < 0){
        //printf("Error: Data Receive Error [ 0x01 ] - %ld\n", WSAGetLastError());
        printf("Error: Data Receive Error [ 0x01 ]\n");
    }else{
        if(bytesRecv == 0){ // PING Request
            // Close Connection
            // Repeat Process
            return bytesRecv;
        }

        /* Process Primary Buffer for Hint to Next */

        //delPos1 = (int) strstr(buffer_data, "\n");
        //strncpy(receive_array, buffer_data, delPos1);
        //printf("\n>buffer_data : %s", buffer_data);



        header_sds = sdsnew(buffer_data);
        header_token = sdssplitlen(header_sds, sdslen(header_sds), "\n", 1, &header_count);

        for (i = 0; i < header_count; i++){

            if(strcmp(header_token[i], "") == 0){
                // End of Header
                sdsfreesplitres(header_token, header_count);
                break;
            }

            param_sds = sdsnew(buffer_data);

            if(i == 0){
                // First Line [ GET / POST ]
                param_token = sdssplitlen(param_sds, sdslen(param_sds), " ", 1, &param_count);

                // Process Request Method [ GET, POST, PUT, DELETE ]
                if(strcmp(param_token[0], "GET") == 0 || strcmp(param_token[0], "POST") == 0 ||
                   strcmp(param_token[0], "PUT") == 0 || strcmp(param_token[0], "DELETE") == 0){
                    strcpy(request_data->request_method, param_token[0]);
                }else{
                    // Return Error: Invalid Request Method
                    printf("Error: Invalid Request Method [ 0x02 ]\n");
                }

                // Process Request URI
                if(strcmp(param_token[1], "") != 0){
                    process_file_path(request_data, config_store, param_token[1]);
                }
            }else{
                // Other Headers
                param_token = sdssplitlen(param_sds, sdslen(param_sds), ": ", 1, &param_count);
            }

            sdsfreesplitres(param_token, param_count);

            //param_sds = sdsnew(buffer_data);


        }
    }


    return 1;
}

int process_file_path(rqpack *request_data, s_conf config_store, char *file_name){

    int i, j;
    char file_path[2596];

    request_data->requested_uri = malloc(strlen(file_name) + 1);
    request_data->requested_uri = file_name;

    // Check If ? Exists
    for(i = 0; i < strlen(file_name); i++){
        if(file_name[i] == '?'){
            // Discard [ Query Parameter ]
            for(j = i; j < strlen(file_name); j++){
                file_name[j] = '\0';
            }
        }
    }

    // Check For Basic
    if(strcmp(file_name, "/") == 0){ // For index.html
        strcpy(file_name, "/index.html");
    }

    url_decode((const char*)file_name, file_name);

    request_data->file_name = malloc(strlen(file_name) + 1);
    strcpy(request_data->file_name, file_name);

    // Make File Path
    sprintf(file_path, "%s%s", config_store.default_dir, file_name);

    request_data->file_path = malloc(strlen(file_path) + 1);
    request_data->file_path = file_path;

    // Display Info About Requested File
    printf("> Requested File: %s\n", file_path);

    return 0;
}
