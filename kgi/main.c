#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <openssl/rand.h>

/*
To Do:
1. Server will provide an argument which is the socket to client



*/
unsigned int random_uint(unsigned int limit);
int random_socket_path(char *path);

int main(int argc, char *argv[]) {

    int client_socket;
    char local_socket[33];

    if(argc == 1){
        // Display Error Message
        printf("Error: Invalid Client Socket\n");
        return 0;
    }else{
        // Get Client Socket
        client_socket = (int) argv[1];
    }

    // Create a Socket path for Program
    random_socket_path(&local_socket);
    printf("%s", local_socket);
/*

  struct sockaddr_un addr;
  char buf[100];
  int fd,cl,rc;

  if (argc > 1) socket_path=argv[1];


  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  if (*socket_path == '\0') {
    *addr.sun_path = '\0';
    strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
  } else {
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);
  }

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(fd, 5) == -1) {
    perror("listen error");
    exit(-1);
  }

  while (1) {
    if ( (cl = accept(fd, NULL, NULL)) == -1) {
      perror("accept error");
      continue;
    }

    while ( (rc=read(cl,buf,sizeof(buf))) > 0) {
      printf("read %u bytes: %.*s\n", rc, rc, buf);
    }
    if (rc == -1) {
      perror("read");
      exit(-1);
    }
    else if (rc == 0) {
      printf("EOF\n");
      close(cl);
    }
  }

*/
  return 0;
}

/*
Source: https://stackoverflow.com/a/31282121
*/
unsigned int random_uint(unsigned int limit) {
    union {
        unsigned int i;
        unsigned char c[sizeof(unsigned int)];
    } u;

    do {
        if (!RAND_bytes(u.c, sizeof(u.c))) {
            fprintf(stderr, "Can't get random bytes!\n");
            exit(1);
        }
    } while (u.i < (-limit % limit)); /* u.i < (2**size % limit) */
    return u.i % limit;
}

int random_socket_path(char *path){
    int i;
    unsigned int j;

    path[0] = '\0';

    for(i = 1; i < 33; i++){
        j = random_uint[25];
        path[i] = "abcdefghijklmnopqrstuvwxyz"[j];
    }

    path[33] = '\0';
}
