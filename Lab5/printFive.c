/*
    Programa: printFive.c
    Autor: Gabriel Trindade Corrêa
    DRE: 120019606
    Data: 11/05/2023
    Descrição: Este programa implementa Atividade 4 do Lab5, testando ordenação de threads com variáveis de condição.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

/* Variáveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond1, x_cond2;

/* Função das 2 primeiras linhas: "Olá José/Maria!" */
void *hello (void *t) {
    long id = (long) t;
    
    if (id == 0) printf("Oi Jose!\n");
    if (id == 1) printf("Oi Maria!\n");

    /* Exclusão mútua e teste condicional para garantir ordem de impressão */
    pthread_mutex_lock(&x_mutex);
    x++;
    if (x==2) {
        //Libera linha intermediária (usa signal)
        pthread_cond_signal(&x_cond1);
    }

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Função da linha intermediária: "Sente-se, por favor." */
void *sit (void *t) {

    /* Exclusão mútua e teste condicional para garantir ordem de impressão */
    pthread_mutex_lock(&x_mutex);
    if (x < 2){
        //Espera 2 linhas iniciais
        pthread_cond_wait(&x_cond1, &x_mutex);
    }

    x++;
    printf("Sente-se, por favor.\n");
    //Libera linhas finais (usa broadcast)
    pthread_cond_broadcast(&x_cond2);

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Função das 2 linhas finais: "Até mais José/Maria!" */
void *bye (void *t) {
    long id = (long) t;
    
    /* Exclusão mútua e teste condicional para garantir ordem de impressão */
    pthread_mutex_lock(&x_mutex);
    if (x < 3) {
        //Espera linha intermediária
        pthread_cond_wait(&x_cond2, &x_mutex);
    }

    if (id == 3) printf("Ate mais Jose!\n");
    if (id == 4) printf("Ate mais Maria!\n");

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Função principal */
int main(int argc, char *argv[]) {
 pthread_t threads[NTHREADS];

 /* Inicializa o mutex (lock de exclusão mútua) e as variáveis de condição 1 e 2 */
 pthread_mutex_init(&x_mutex, NULL);
 pthread_cond_init (&x_cond1, NULL);
 pthread_cond_init (&x_cond2, NULL);

 /* Cria 5 threads */
 pthread_create(&threads[0], NULL, hello, (void *)0);
 pthread_create(&threads[1], NULL, hello, (void *)1);
 pthread_create(&threads[2], NULL, sit, (void *)2);
 pthread_create(&threads[3], NULL, bye, (void *)3);
 pthread_create(&threads[4], NULL, bye, (void *)4);

 /* Espera todas as 5 threads completarem */
 for (int i = 0; i < NTHREADS; i++) {
   pthread_join(threads[i], NULL);
 }
 printf("\nFIM\n");

 /* Desaloca variáveis e termina */
 pthread_mutex_destroy(&x_mutex);
 pthread_cond_destroy(&x_cond1);
 pthread_cond_destroy(&x_cond2);
}

