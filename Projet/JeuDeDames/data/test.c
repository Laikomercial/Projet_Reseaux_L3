#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char nom[25];
    int nombre;
}User;



int main(int argc, char const *argv[])
{
    char nom[50];
    char mdp[50];
    char text[] = {"momo"};
    FILE* fp = fopen("data.dat","a+");
    if(!fp){
        perror("Fichier non ouvert");
    }
    //fputs(text,fp);
    //fprintf(fp,"%s,%d\n",user1.nom, user1.nombre);
    //rewind(fp);
    while(fscanf(fp,"%s %s",nom,mdp) == 2){
        printf("%s %s\n",nom , mdp);
    }
    
    fclose(fp);
    return 0;
}
