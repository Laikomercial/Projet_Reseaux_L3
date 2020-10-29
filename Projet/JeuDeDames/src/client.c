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

#define TAILLEMAXPARTIE 10

typedef struct User
{
    char pseudo[20];
    char psswd[20];
}User;
typedef struct Partie
{
    User player[2];
    User spectateur[10];
    int idPartie;
    int nbSpectateur;
    int nbPlayer;
}Partie;

int main(int argc, char const *argv[])
{
    /*--------------------CONNEXION AU SERVEUR----------------------*/

    int socketClient = socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(12347);
    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
    printf("connecté\n");
    
    /*--------------------INITIALISATION DES VARIABLES----------------------*/

    User user;
    int choose;
    char msg[51];
    char msg1[100];
    char msg2[85];
    int endCond = 0;
    
    /*--------------------AUTHENTIFICATION----------------------*/

    recv(socketClient, &msg, 50, 0);
    printf("%s\nChoisissez ce que vous voulez faire : ", msg);
    scanf("%d", &choose);
    printf("Choose : %d\n", choose);
    send(socketClient, &choose, sizeof(choose), 0);

    switch (choose)
    {
    case 1:
        recv(socketClient, msg1, 100, 0);
        do {
            printf("%s\n", msg1);
            scanf("%s %s", user.pseudo, user.psswd);
            send(socketClient, &user, sizeof(user), 0);
            recv(socketClient, msg1, 100, 0);
        }while(strcmp(msg1,"exit") != 0);
        printf("Le compte a été créer\n");
        break;
    
    case 2:
        recv(socketClient, msg1, 100, 0);
        do{
            printf("%s", msg1);
            scanf("%s %s", user.pseudo, user.psswd);
            send(socketClient, &user, sizeof(user), 0);
            recv(socketClient, msg1, 100, 0);
        }while(strcmp(msg1,"exit") != 0);

        printf("Vous êtes connecté sur le compte %s\n", user.pseudo);
        break;
    case 3:
        recv(socketClient, msg1, 42, 0);
        printf("%s\n", msg1);
        break;
        
    default:
        break;
    }  
    
    /*--------------------CHOIX PARTIE----------------------*/
    //Mettre tout ça dans un while(1) pour que ça boucle à l'infini tant qu'on ne choisis pas de quitter ?
    while(endCond != 1){
        recv(socketClient, &msg2, 84, 0);
        printf("%s\nChoisissez ce que vous voulez faire : ", msg2);
        scanf("%d", &choose);
        send(socketClient, &choose, sizeof(choose), 0);
        switch (choose)
        {
        case 1:
            /* Creer */
            //Le serv envois : "En attente d'un autre joueur"
            int accptGame;
            recv(accptGame);


            if(accptGame == 1){
                printf("En attente d'un joueur\n");
                recv(plateau);
            }
            else{
                printf("Il y a trop de partie en cours, veuillez faire un autre choix\n");
            }
            break;
        case 2:
            int check = 1;
            int listePartieBool[TAILLEMAXPARTIE];
            int choix;
            Partie listePartie[TAILLEMAXPARTIE];
            recv(socketClient, listePartieBool, TAILLEMAXPARTIE, 0);
            send(socketClient, &check, sizeof(check), 0);
            recv(socketClient, listePartie, TAILLEMAXPARTIE, 0);
            
            /* Rejoindre */
            printf("Liste des parties disponibles :\n");
            for(int i = 0; i<TAILLEMAXPARTIE; i++){
                if(listePartieBool[i] == 1 && listePartie[i].nbPlayer == 1){
                    printf("- %d(%s)\n", i, listePartie[i].player[0].pseudo);
                }
            }
            printf("Veuillez faire votre choix : \n");
            scanf("%d", choix);
            send(socketClient, &choix, sizeof(choix), 0);
            break;
        case 3:
            /* Regarder */
            
            break;
        case 4:
            printf("Vous quittez le jeu\n");
            endCond = 1;
            break;
        default:
            printf("La valeur rentré est incorrecte, veuillez recommencer");
            break;
        }
    }    
    
    
    close(socketClient);
    printf("Close\n");
    return 0;
}
