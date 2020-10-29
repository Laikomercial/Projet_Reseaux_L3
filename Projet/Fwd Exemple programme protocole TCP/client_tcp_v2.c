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
écouter plusieurs file descriptor (gestion de communication avec le serveur et
de récupération de message saisie dans le terminal en simultané)
******************************************************************************/

int main(int argc, char * argv[]){
    // Début du code similaire au client TCP standard
    int port = 12347;
    char * ip_addr;
    ip_addr = argv[1];
    printf("Adresse IP utilisée pour le serveur TCP : %s;\n", ip_addr);
    port = atoi(argv[2]);
    printf("Port utilisé pour le client TCP : %d.\n", port);

    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sin;
    int i, ret;
    memset((char *) &sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if(inet_aton(ip_addr, &sin.sin_addr) == 0){
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    for (i=0; i<8; i++) {
        sin.sin_zero[i] = 0;
    }

    ret = connect(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
    if (ret<0) {
        printf("error");
    }
    printf("connect : %d\n", ret);

    char recv_message[MAX_DGRAM_SIZE];

    int send_out, recv_out;
    printf("Start connexion\n");

    // Définition de l'ensemble des files descriptors qui seront écoutés (ici,
    // seulement des files descriptor en lecture)
    fd_set rfds;
    int retval; // Variable de stockage du retour du select
    int max_s; // Stockage du plus grand file descriptor de l'ensemble rfds (
    // nécessaire pour le fonctionnement de 'select')

    char message[MAX_DGRAM_SIZE];

    while(1){
        // Mise à 0 du buffer de récupération des messages
        memset(recv_message, 0, MAX_DGRAM_SIZE);
        // FD_CLR(s, &rfds); // Clear du file descriptor du serveur TCP
        // FD_CLR(0, &rfds); // Clear du file descriptor de l'entrée standard (stdin)
        FD_ZERO(&rfds);
        FD_SET(0, &rfds); // On rajoute le file descriptor du serveur
        FD_SET(s, &rfds); // On rajoute le file descriptor de stdin
        
        // Récupération du plus grand file descriptor entre celui du serveur et
        // celui de l'entrée standard
        if(s > 0) {
            max_s = s;
        } else {
            max_s = 0;
        }

        // Structure servant à stocker le temps avant le timeout de 'select'
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;

        // Ecoute de stdin et du serveur
        retval = select(max_s+1, &rfds, NULL, NULL, &tv);
        printf("retval : %d\n", retval);
        if(retval == -1){
            perror("select()");
        } else {
            // Si stdin ou le serveur à un message pour le client
            if(FD_ISSET(s,&rfds)){ // Si c'est le serveur
                //affichage du message reçu
                recv_out = recv(s, &recv_message, MAX_DGRAM_SIZE, 0);
                printf("$> %s", recv_message);
            } else if(FD_ISSET(0,&rfds)) { // Si c'est stdin
                // Récupération du message saisi dans stdin
                recv_out = read(0, recv_message, MAX_DGRAM_SIZE);
                printf("Message envoyé : \"%s\"", recv_message);
                // Envoie du message au serveur
                send_out = send(s, recv_message, sizeof(recv_message), 0);
            } else { //Erreur lors du select
                printf("Error FD_ISSET");
            }
        }
    }

    return(0);
}