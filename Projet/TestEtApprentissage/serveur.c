#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

typedef struct User
{
    char pseudo[20];
    char psswd[20];
}User;

void *function(void *arg){
    int socket = *(int*)arg;
    char msg[] = "Quel est votre pseudo et votre mot de passe ?";
    User user;
    send(socket, msg, strlen(msg)+1, 0);
    recv(socket, &user, sizeof(user), 0);
    printf("Pseudo : %s Mot de passe : %s\n", user.pseudo, user.psswd);
    close(socket);
    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t clientThread;
    
    int socketServ = socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(12345);

    bind(socketServ,(const struct sockaddr *)&addrServer, sizeof(addrServer));
    printf("bind : %d\n", socketServ);

    listen(socketServ, 5);
    printf("Listen\n");

    for(int i = 0; i < 4; i++)
    {
        struct sockaddr_in addrClient;
        socklen_t csize = sizeof(addrClient);
        int socketClient = accept(socketServ, (struct sockaddr *)&addrClient, &csize);
        printf("accept\n");
        printf("Client: %d\n", socketClient);

        int *arg = malloc(sizeof(int));
        *arg = socketClient;

        pthread_create(&clientThread, NULL, function, arg);
    };
    /*
    User user = {
        .pseudo = "Laiko",
        .psswd = "password"
    };
    send(socketClient,&user, sizeof(User), 0);
    */
    
    close(socketServ);
    printf("close\n"); 
    
    
    return 0;

}
