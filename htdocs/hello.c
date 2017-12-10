#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int main(){
    printf("HTTP/1.1 200 OK");
    printf("\nContent-Type: text/html");
    printf("\n\n");
    fflush(stdout);sleep(1);
    printf("Hello World!");
    fflush(stdout);sleep(1);
    printf("<br>Once I was C");
    fflush(stdout);sleep(1);
    printf("<br>Now I can See");

    int i;
    for(i = 0; i < 10; i++){
        printf(" %d", i);
        fflush(stdout);sleep(1);
    }
    return 0;
}
