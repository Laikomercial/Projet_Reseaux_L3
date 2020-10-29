#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* EXEMPLE UTILISATION THREAD
- Include le fichier pthread.h
- Ajouter -lpthread à la fin de la commande de compilation
*/


void *func1(void *arg)
{
    for(int i = 0; i < 100; i++)
    {
        fprintf(stderr, "Thread 1: %d\n", i);
    }
    pthread_exit(NULL);
}

void *func2(void *arg)
{
    for(int i = 0; i < 100; i++)
    {
        fprintf(stderr, "Thread 2: %d\n", i);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t t1; // Creer le thread
    pthread_t t2;

    pthread_create(&t1, NULL, func1, NULL); //Allou le thread à une fonction. 
    pthread_create(&t2, NULL, func2, NULL); //Le 3eme paramêtre est la fonction, le 4eme est le (ou les) paramètre de la fonction

    pthread_join(t1, NULL); //attendre que les thread soient fini avant de passer à la suite du main
    pthread_join(t2, NULL); //Le 1er paramètre est le thread, et le second est le paramètre à récupérer à la fin
    return 0;
}