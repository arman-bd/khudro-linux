/*
Khudro Gateway Interface
*/

void *khudro_gateway_interface() {

    char *DEFAULT_DIR, *OBJECT_DIR, *SCRIPT_PATH, *SCRIPT_QUERY;
    int CLIENT_SOCKET = -1;

    DEFAULT_DIR = malloc(128);
    OBJECT_DIR = malloc(128);
    SCRIPT_PATH = malloc(512);
    SCRIPT_QUERY = malloc(512);

    strcpy(DEFAULT_DIR, "htdocs");
    strcpy(OBJECT_DIR, "objects");
    strcpy(SCRIPT_PATH, GlobalFilePath);
    strcpy(SCRIPT_QUERY, GlobalQuery);
    CLIENT_SOCKET = GlobalSock;


    char compiler_command[1024] = "", object_path[1024] = "", runtime_path[1024] = "";

    pid_t compiler_pid, object_pid;
    int compiler_pipe[3], object_pipe[3];
    char compiler_buffer[1024] = "", object_buffer[1024] = "";

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

    if(access(object_path, F_OK) == -1){
        // Not Copiled Yet!

        strcpy(compiler_command, "g++ ");
        strcat(compiler_command, SCRIPT_PATH);
        strcat(compiler_command, " -o ");
        strcat(compiler_command, object_path);

        printf("\nCompiler Command: %s\n", compiler_command);

        compiler_pid = popenRWE(compiler_pipe, compiler_command);
        close(compiler_pipe[0]);

        while(read(compiler_pipe[1], &compiler_buffer, sizeof(compiler_buffer))){
            printf("> Compiler: %s\n", compiler_buffer);
        }
        pcloseRWE(compiler_pid, compiler_pipe);
    }



    // Check If Compiled File Exists
    if(access(object_path, F_OK) == -1){
        // File Does Not Exists
        printf("Error: Compilation Error!");
        return 0;
    }

    // Process Local Script
    //chdir(getenv("DEFAULT_DIR"));


    strcat(runtime_path, object_path);
    strcat(runtime_path, " \"");
    strcat(runtime_path, SCRIPT_QUERY);
    strcat(runtime_path, "\"");

    object_pid = popenRWE(object_pipe, runtime_path);


    //printf("> Executing: %s\n\n", runtime_path);

    // In Case of POST
    /*
    write(object_pipe[0], "Hello World", 11);
    close(object_pipe[0]);
    */
    close(object_pipe[0]);


    while(read(object_pipe[1], &object_buffer, sizeof(object_buffer))){
        //printf("%s", object_buffer);
        send(CLIENT_SOCKET, object_buffer, strlen(object_buffer), 0);
        memset(object_buffer, 0, sizeof object_buffer);
    }
    close(CLIENT_SOCKET);
    pcloseRWE(object_pid, object_pipe);

    // Connect To Client

    // Run Local Script ( Application )

    // Send To Client
    sdsfree(param_sds);
    free(DEFAULT_DIR);
    free(OBJECT_DIR);
    free(SCRIPT_PATH);
    free(SCRIPT_QUERY);

    return 1;
}
