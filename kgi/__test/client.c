#include<stdio.h>
#include<stdlib.h>

#pragma IGNORE_ALL_ERROR

int main(){

    //char inp[1024];
    //scanf("%s", &inp);

    //printf("Hello %s, This Works!\n", inp);
    char cwd[1024];

    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("Current working dir: %s", cwd);
    }

    printf("\nGET: %s", getenv("GET"));
    printf("\nPOST: %s", getenv("POST"));
    //printf("\nCOMSPEC: %s", getenv("COMSPEC"));
    return 0;
}

