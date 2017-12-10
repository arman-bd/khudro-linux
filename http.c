
int ListenHTTP(){
    int MainSocket;
    struct sockaddr_in service;
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
    recon:
    if(bind(MainSocket, (struct sockaddr *)&service, sizeof(service)) < 0){
        printf("Error: Binding Failed at Port [ %d ].\n", server_conf.default_port);
        printf("       Check If Port Is Already In Use.\n");

        server_conf.default_port++;
        service.sin_port = htons(server_conf.default_port);// Local Port
        close(MainSocket);
        MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        goto recon;

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
    return MainSocket;

}
