
void get_mime_type(char *file_name, char *result){
    /*
        Get MIME Type of File from file extension
        @file_name : File Name with File Extension
        @result : [ Return Value ]
    */
    char *name_array;
    char extension[16];

    name_array = strtok(file_name, ".");
    while(name_array != NULL){
        if(strlen(name_array) < 16){
            strcpy(extension, name_array);
        }
        name_array = strtok(NULL, ".");
    }

    //result = malloc(128);
    strcpy(result, "text/plain");


    if(strcmp(extension, "zip") == 0){strcpy(result, "multipart/x-zip"); return;}
    if(strcmp(extension, "xml") == 0){strcpy(result, "text/xml"); return;}
    if(strcmp(extension, "txt") == 0){strcpy(result, "text/plain"); return;}
    if(strcmp(extension, "mp3") == 0){strcpy(result, "video/x-mpeg"); return;}
    if(strcmp(extension, "log") == 0){strcpy(result, "text/plain"); return;}
    if(strcmp(extension, "jpeg") == 0){strcpy(result, "image/pjpeg"); return;}
    if(strcmp(extension, "jpg") == 0){strcpy(result, "image/pjpeg"); return;}
    if(strcmp(extension, "ico") == 0){strcpy(result, "image/x-icon"); return;}
    if(strcmp(extension, "inf") == 0){strcpy(result, "application/inf"); return;}
    if(strcmp(extension, "htm") == 0){strcpy(result, "text/html"); return;}
    if(strcmp(extension, "html") == 0){strcpy(result, "text/html"); return;}
    if(strcmp(extension, "htmls") == 0){strcpy(result, "text/html"); return;}
    if(strcmp(extension, "gz") == 0){strcpy(result, "application/x-gzip"); return;}
    if(strcmp(extension, "gzip") == 0){strcpy(result, "multipart/x-gzip"); return;}
    if(strcmp(extension, "gif") == 0){strcpy(result, "image/gif"); return;}
    if(strcmp(extension, "exe") == 0){strcpy(result, "application/octet-stream"); return;}
    if(strcmp(extension, "c") == 0){strcpy(result, "text/x-c"); return;}
    if(strcmp(extension, "c++") == 0){strcpy(result, "text/plain"); return;}
    if(strcmp(extension, "cpp") == 0){strcpy(result, "text/x-cpp"); return;}
    if(strcmp(extension, "bmp") == 0){strcpy(result, "image/x-windows-bmp"); return;}

    return;
}
