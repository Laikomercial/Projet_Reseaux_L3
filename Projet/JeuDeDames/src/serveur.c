
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
#define N 20
#define TAILLEMAXPARTIE 10




typedef struct User
{
    char pseudo[N];
    char psswd[N];
}User;

typedef struct Partie
{
    User player[2];
    User spectateur[10];
    int idPartie;
    int nbSpectateur;
    int nbPlayer;
}Partie;
Partie listePartie[TAILLEMAXPARTIE];
int listePartieBool[TAILLEMAXPARTIE]={0};

int verifUserName(User user);
int verifCompte(User user);
void choixPartie(void *arg, User user);


/*----------------------PARTIE----------------------*/
int creerPartie(User user){
    printf("creer partie");
    Partie partie;
    
    for(int i = 0 ; i < TAILLEMAXPARTIE ; i++){
        if(listePartieBool[i] == 0){ 
            partie.player[0] = user;
            partie.nbSpectateur = 0;
            partie.nbPlayer = 1;
            partie.idPartie = i;
            partie.player[0]=user;
            listePartie[i] = partie;
            listePartieBool[i] = 1;
            
            return 1;
        }
    }
    return 0;
}
int regarderPartie(User user , int idPartie ){
    printf("regarder partie");
    if(listePartieBool[idPartie] == 0){
        printf("Partie Existe pas\n");
        return 0;
    }
    listePartie[idPartie].spectateur[listePartie[idPartie].nbSpectateur + 1] = user;
    listePartie[idPartie].nbSpectateur++;
    return 1 ;
}

int rejoindrePartie(int idPartie, User user){
    if(listePartieBool[idPartie] == 0 || listePartie[idPartie].nbPlayer == 2){
        printf("Partie Existe pas\n");
        return 0;
    }
    listePartie[idPartie].player[1] = user;
    listePartie[idPartie].nbPlayer++;
    return 1;
}


/*----------------------LOGIN----------------------*/
User creationDeCompte(void *arg){
    User user;
    int socket = *(int*)arg;
    char msg[] = "Pseudo et Mot de passe ";
    char exit[] = "exit";
    do {
        send(socket, msg, strlen(msg)+1, 0);
        recv(socket, &user, sizeof(user), 0);
        printf("Pseudo : %s et mot de passe : %s\n", user.pseudo, user.psswd);
    }while(verifUserName(user) == 1);
    send(socket, exit, strlen(exit)+1, 0);
    return user;
}

User connexion(void *arg){
    User user;
    int socket = *(int*)arg;
    char exit[] = "exit";
    char msg[] = "Pseudo et Mot de passe ";
    printf(" ");
    do {
        send(socket, msg, strlen(msg)+1, 0);
        recv(socket, &user, sizeof(user), 0);
        printf("Pseudo : %s et mot de passe : %s\n", user.pseudo, user.psswd);
    }while(verifCompte(user) == 0);

    send(socket, exit, strlen(exit)+1, 0);

    printf("Nom d'utilisateur choisis : %s", user.pseudo);
    return user;
}



User compteInvite(void *arg){
    User user = {
        "Invite", 
        "Invite"
        };
    
    int socket = *(int*)arg;
    char msg[] = "Vous etes connecté sur un compte invité\n";
    send(socket, msg, strlen(msg)+1, 0);
    return user;
}

int verifUserName(User user){
    char nom[N],mdp[N];
    FILE* fp = fopen("src/data.txt","a+");
    if(!fp){
        perror("Fichier non ouvert");
    }
    rewind(fp);
    while(fscanf(fp,"%s %s",nom,mdp) == 2){
        if(strcmp(nom,user.pseudo) == 0){
            printf("Le pseudo est deja utiliser");
            return 1;
        }
    }
    fprintf(fp,"%s %s\n",user.pseudo,user.psswd);
    printf("Le pseudo %s a ete ajoute\n",user.pseudo);
    rewind(fp);
    fclose(fp);
    return 0;
}

int verifCompte(User user){
    char nom[N],mdp[N];
    FILE* fp = fopen("src/data.txt","r");
    if(!fp){
        perror("Fichier non ouvert");
    }
    rewind(fp);
    while(fscanf(fp,"%s %s",nom,mdp) == 2){
        if(strcmp(nom,user.pseudo) == 0){
            //strcat(mdp, "\n");
            if(strcmp(mdp,user.psswd) == 0){
                printf("Le Passeword est bon\n");
                return 1;
            }
            printf("Le Passeword n'est bon pas \n");
            printf(" ");
            return 0;
        }
    }
    rewind(fp);
    fclose(fp);
    return 0;
}


void *authentification(void *arg){
    int socket = *(int*)arg;
    char msg[] = "1 Creation de compte\n2 Connexion\n3 Compte Invité";
    int choose;
    User user;
    send(socket, &msg, strlen(msg)+1, 0);
    recv(socket, &choose, sizeof(choose), 0);
    printf("Choix : %d\n", choose);
    switch (choose)
    {
        case 1:
            user = creationDeCompte(&socket);
            break;
        case 2:
            user = connexion(&socket);
            break;
        case 3:
            user = compteInvite(&socket);
            break;
        default:
            break;
    }
    choixPartie(&socket, user);
    close(socket);
    free(arg);
}

void choixPartie(void *arg, User user){
    // Tout mettre dans un while(1), idem client
    int socket = *(int*)arg;
    char msg[] = "1 - Créer une partie\n2 - Rejoindre une partie\n3 - Regarder une partie\n4 - Quitter";
    int choose;
    send(socket,&msg, strlen(msg)+1, 0);
    recv(socket, &choose, sizeof(choose), 0);
    printf("%d", choose);
    switch (choose)
    {
    case 1:
        /* Creer Partie */
        if(creerPartie(user) == 0){
            printf("PAS Possible de creee une partie");
        }
        break;
    case 2:
        int check;
        /* Rejoindre Partie */
        do{
            send(socket, listePartieBool, sizeof(listePartieBool), 0);
            recv(socket, &check, sizeof(check), 0);
            if(check == 1){
                send(socket, listePartie, sizeof(listePartie), 0);
            }
            recv(socket, &check, sizeof(check), 0);
        }while(rejoindrePartie(check, user) == 0);
        //lancerPartie();
        break;
    case 3:
        /* Spectateur */
        /*char msg[]="Voici les partie disponible:\n\tpartie : |";
        
        for(int i = 0 ; i < TAILLEMAXPARTIE ; i++){
            if(listePartieBool[i] == 1){
                strncar(msg ," %d |",i );
                
            }
        }
        strncat(msg, "\ntaper sont numeros :\n" );*/
        do{
            send(socket,&msg, strlen(msg)+1, 0);
            recv(socket, &choose, sizeof(choose), 0);
        }while(regarderPartie(user,choose) == 0);
        break;

    case 4:
        close(socket);
        free(arg); 
        pthread_exit(NULL);
    default:
        break;
    }
}




int main(int argc, char const *argv[])
{
    pthread_t clientThread;
    
    int socketServ = socket(AF_INET, SOCK_STREAM,0);
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(12347);

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

        pthread_create(&clientThread, NULL, authentification, arg);
    };

    
    close(socketServ);
    printf("close\n"); 
    
    
    return 0;

}
