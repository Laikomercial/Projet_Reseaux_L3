#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define MAX_DGRAM_SIZE 256

/******************************************************************************
    Ce code n'est pas aussi propre que ce qui vous est demandé dans le TP
    Il a pour objectif de vous montrer comment fonctionne 'select', et comment
écouter plusieurs file descriptor (gestion de plusieurs clients en simultané)
******************************************************************************/

int main(int argc, char * argv[]){
    //Début du  code similaire au code du serveur tcp standard
    int port = 12347;
    if(argc>1){
        port = atoi(argv[1]);
        printf("Port utilisé pour le serveur TCP : %d\n", port);
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sin;
    int i, ret;
    memset((char *) &sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    for (i=0; i<8; i++) {
        sin.sin_zero[i] = 0;
    }

    ret = bind(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
    if (ret<0) {
        printf("error");
    }

    struct sockaddr_in sender_addr;
    int sender_addr_len;
    int recv_out, send_out;
    int client_socket;
    char message[MAX_DGRAM_SIZE];
    sender_addr_len = sizeof(sender_addr);

    listen(s, 3);
    printf("Start listening\n");

    struct sockaddr_in clients_addr[3];
    int clients_addr_len[3];
    int clients_socket[3];

    // Définition de l'ensemble des files descriptors qui seront écoutés (ici,
    // seulement des files descriptor en lecture)
    fd_set rfds;
    for(int i =0; i<3; ++i){
        // Récupération des files descriptor propre à chacun des clients
        clients_socket[i] = accept(s, (struct sockaddr *)&clients_addr[i], &clients_addr_len[i]); 
    }
    int retval; // Variable de stockage du retour du select
    int max_s; // Stockage du plus grand file descriptor de l'ensemble rfds (
    // nécessaire pour le fonctionnement de 'select')

    while(1){
        // Mise à 0 du buffer de stockage de mesage
        memset(message, 0, MAX_DGRAM_SIZE);
        printf("Attente des message entrant\n");
        fflush(stdout);
        // Mise à 0 de l'ensemble des files descriptor écoutés
        FD_ZERO(&rfds);
        // Rajout des files descriptor de chacun des clients
        FD_SET(clients_socket[0], &rfds);
        FD_SET(clients_socket[1], &rfds);
        FD_SET(clients_socket[2], &rfds);
        
        // Récupération du plus grand des files descriptor
        max_s = clients_socket[0];
        for (int i = 1; i < 3; ++i) {
            if (clients_socket[i] > max_s) {
                max_s = clients_socket[i];
            }
        }

        // Définition du temps avant timeout du select
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        // Ecoute active sur tout les clients simultanéments
        retval = select(max_s+1, &rfds, NULL, NULL, &tv);
        printf("retval : %d\n", retval);
        if(retval == -1){
            perror("select()");
        } else {
            // Pour chacun des clients
            for(int i = 0; i<3; ++i){
                // On vérifie si un message a été reçu
                if(FD_ISSET(clients_socket[i],&rfds)) {
                    // Si un message est reçu, on le récupère
                    recv_out = recv(clients_socket[i], &message, 256, 0);
                    printf("Message reçu : %s.\n", message);
                    for(int j = 0;  j<3; ++j){
                        // Pour chacun des autres clients
                        if(j!=i){
                            // On transmet le message reçu
                            send_out = send(clients_socket[j], message, sizeof(message), 0);
                        }
                    }
                }
            }
        }
    }

    return(0);
}