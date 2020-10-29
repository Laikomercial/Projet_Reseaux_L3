
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

typedef struct User
{
    char pseudo[20];
    char psswd[20];
}User;

int main(int argc, char const *argv[])
{
    int socketClient = socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(12345);
    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("connecté\n");

    User user;
    char msg[47];
    recv(socketClient, msg, 46, 0);
    printf("%s\n", msg);
    scanf("%s %s", user.pseudo, user.psswd);
    for(int i = 0; i < 10000 ; i++)
    printf("\n%s %s", user.pseudo, user.psswd);
    send(socketClient, &user, sizeof(user), 0);

    /*
    User user;
    recv(socketClient, &user, sizeof(User), 0);
    printf("Pseudo :%s and Password : %s\n", user.pseudo, user.psswd);
    */

    close(socketClient);
    printf("Close\n");
    return 0;
}
