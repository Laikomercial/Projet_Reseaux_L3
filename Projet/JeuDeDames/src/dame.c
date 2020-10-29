#include <stdio.h>
#include <stdlib.h>
#define N 10

void affichePlateau(char tab[][N]);
void initPlateau(char tab[][N]);
void promotionDame(char tab[][N],int posX , int posY);

int main(){
    char tab[N][N];
    initPlateau(tab);
    //affichePlateau(tab);
    promotionDame(tab,2,3);
    affichePlateau(tab);
    return 0;
}

void affichePlateau(char tab[][N]){
    printf("entrer affiche Plateau\n\n");
    int i ;
    printf("\t\t    ");
    for(i = 0 ; i < N ;i++){
        printf("   %d",i);
    }
    printf("\n\t\t    ");
    for(i = 0 ; i < N ;i++){
        printf("----");
    }
    printf("---\n");
    for(i = 0 ; i < N ; i++){
        printf("\t\t%d  |",i);
        for(int j = 0 ; j < N ; j++){
            printf("   %c",tab[i][j]);
        }
        printf("   |  %d\n\t\t   |\t\t\t\t\t       |\n",i);
    }
    printf("\t\t    ");
    for(i = 0 ; i < N ;i++){
        printf("----");
    }
    printf("---\n");
    printf("\t\t    ");
    for(i = 0 ; i < N ;i++){
        printf("   %d",i);
    }
    printf("\n\n");
}

void initPlateau(char tab[N][N]){
    printf("entrer init Plateau\n\n");
    int i  , j ;
    for(i = 0 ; i < N ; i++ ){
        for(j = 0 ; j < N ; j++){
            tab[i][j]='.';
        }
    }
    for(i = 0 ; i < 4 ; i++ ){
        for(j = 1 ; j < N ; j++){
            if(j%2 == 1){
                tab[i][j+(i%2)*-1]='n';
            }
        }
    }
    for(i = N-4 ; i < N ; i++ ){
        for(j = 0 ; j < N ; j++){
            if(j%2 == 1){
                tab[i][j+(i%2)*-1]='b';
            }
        }
    }
}


void promotionDame(char tab[N][N],int posX , int posY){      //promotion du jetton en Dame à la Xième colonne, et Yième ligne
    if(tab[posX][posY] = 'n'){
        tab[posX][posY]='N';
    }
    else if(tab[posX][posY] = 'b'){
        tab[posX][posY]='B';
    }
    else {
        printf("Case Vide");
    }
}

/*void deplacementPion(char tab[N][N],int positionActuelX , int positionActuelY , int positionNextX , int positionNextY){
    
}
*/

