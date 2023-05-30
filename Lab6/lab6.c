/*
    Programa: lab6.c
    Autor: Gabriel Trindade Corrêa
    DRE: 120019606
    Data: 29/05/2023
    Descrição: Este programa implementa o problema de produtores e consumidores utilizando semáforo, de acordo com Lab6.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFERSIZE 5

//Semáforos de slots do buffer preenchido, buffer vazio e de exclusão mútua de produtores e consumidores
sem_t slotFull, bufferEmpty, mutex;

//Variável global do buffer
int buffer[BUFFERSIZE];

//Imprime o estado do buffer e o produtor que o preencheu
void printBuffer (int id) {

    printf("Producer %d filled the buffer: ", id);

    for(int i=0; i<BUFFERSIZE; i++) {
        if(i==0) printf("[%d, ", buffer[i]);
        else if(i==BUFFERSIZE-1) printf("%d]\n", buffer[i]);
        else printf("%d, ", buffer[i]);
    }
}

//Imprime o consumidor e o item retirado
void processItem (int id, int item) {
    printf("Consumer [%d] received the item [%d]\n", id, item);
}

//Função de inserir produtos no buffer
void insertItem (int item) {

    //Espera o buffer estar vazio (contador bufferEmpty: 1 --> 0)
    sem_wait(&bufferEmpty);

    //Exclusão Mútua de produtores e consumidores
    sem_wait(&mutex);

    //Preenche o buffer inteiro (contador slotFull: 0 --> BUFFERSIZE)
    for(int i=0; i<BUFFERSIZE; i++){
        buffer[i] = item;
        sem_post(&slotFull);
    }
    printBuffer(item); //O item de um produtor é seu id

    //Fim da Exclusão Mútua de produtores e consumidores
    sem_post(&mutex);
  
}

//Função de remover produtos do buffer
int removeItem (int id) {

    //Variável global da posição de retirada de itens
    static int out = 0;

    //Espera as posições do buffer estarem preenchidas (contador slotFull: n --> n-1, 0 < n <= BUFFERSIZE )
    sem_wait(&slotFull);

    //Exclusão Mútua de produtores e consumidores
    sem_wait(&mutex);

    //Retira 1 item do buffer e exibe no terminal
    int item = buffer[out];
    out = (out+1) % BUFFERSIZE;
    processItem(id, item);

    //Fim da Exclusão Mútua de produtores e consumidores
    sem_post(&mutex);

    //Avisa se o buffer estiver vazio (contador bufferEmpty: 0 --> 1)
    if(out==0) sem_post(&bufferEmpty);

    //Retorna item retirado
    return item;

}

//Função da thread de produtor
void* producer (void* thread_id) {

    int id = *(int*) thread_id;
    int item = id;

    while(1) {
        insertItem(item);
        //Na função de inserir, o buffer é exibido para verificação
    }

    pthread_exit(NULL);
}

//Função da thread de consumidor
void* consumer (void* thread_id) {

    int item;
    int id = *(int*) thread_id;

    while(1) {
       item = removeItem(id);
       //Na função de remover, o produto é "processado", exibindo na tela para verificação
    }

    pthread_exit(NULL);
}

//Função principal
int main (int argc, char *argv[]) {

    //Verificação dos parâmetros de entrada
    if(argc<3) {
        printf("Please type after program call: <number of producers> <number of consumers>\n");
        return 1;
    }
    int numProducers = atoi(argv[1]);
    int numConsumers = atoi(argv[2]);

    pthread_t threads[numProducers+numConsumers];
    int *id[numProducers+numConsumers];

    sem_init(&mutex, 0, 1);
    sem_init(&slotFull, 0, 0);
    sem_init(&bufferEmpty, 0, 1);

    //aloca espaco para os IDs das threads
    for(int i=0; i<numProducers+numConsumers; i++) {
        id[i] = malloc(sizeof(int));
        if(id[i] == NULL) exit(-1);
        *id[i] = i+1;
    }

    //Cria as threads produtoras ou retorna erro -1
    for(int i=0; i<numProducers; i++) {
        if(pthread_create(&threads[i], NULL, producer, (void *) id[i])) exit(-2);
    }

    //Cria as threads consumidoras ou retorna erro -2
    for(int i=0; i<numConsumers; i++) {
        if(pthread_create(&threads[i+numProducers], NULL, consumer, (void *) id[i])) exit(-3);
    }

    pthread_exit(NULL);
    return 0;
}