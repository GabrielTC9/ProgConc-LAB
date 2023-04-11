/*
    Programa: multiVector.c
    Autor: Gabriel Trindade Corrêa
    DRE: 120019606
    Data: 10/04/2023
    Descrição: Este programa utiliza threads para dobrar os valores de um vetor.
*/

#include <stdio.h>
#include <pthread.h>
#define SIZE 10000

//Criando vetor como variável GLOBAL
long vector[SIZE];

//"Main" das threads: multiplica os valores do vetor por 2 
void *multiply_vector(void *thread_id) {
  for(long i = (long) thread_id; i < SIZE; i=i+2) {
    vector[i] *= 2;
    //printf("Thread %ld: Casa %ld\n", (long) thread_id, i);
  }
  pthread_exit(NULL);
};

//Imprime o vetor no terminal para verificação
void print_vector(){
  for(int i = 0; i < SIZE; i++) {
    if(i!=0 & i%10==0) printf("\n%i", vector[i]);
    else printf("%i ", vector[i]);
  }
  printf("\n");
}

int main(void){

  //Preenche o vetor original
  printf("Original vector:\n");
  for(int i = 0; i < SIZE; i++) vector[i] = i+1;

  print_vector();

  int num_threads = 2;
  pthread_t threads[num_threads];

  printf("Generating threads...\n");
  for (long i = 0; i < num_threads; i++) {
	  pthread_create(&threads[i], NULL, multiply_vector, (void *) i);
  }

  for (int i = 0; i < num_threads; i++) {
	  pthread_join(threads[i], NULL);
  }
  printf("Threads done!\n");

  print_vector();

  pthread_exit(NULL);
  return 0;
}
