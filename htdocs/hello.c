#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("HTTP/1.1 200 OK");
    printf("\nContent-Type: text/html");
    printf("\n\n");
    printf("Hello World!");
    printf("<br>Once I was C");
    printf("<br>Now I can See");

    int i;
    for(i = 0; i < 10000; i++){
        printf(" %d", i);
    }
    return 0;
}
