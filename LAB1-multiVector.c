/*
    Programa: Lab1-multiVector.c
    Autor: Gabriel Trindade Corrêa
    DRE: 120019606
    Data: 10/04/2023
    Descrição: Este programa utiliza threads para dobrar os valores de um vetor.
*/

#include <stdio.h>
#include <pthread.h>
#define SIZE 10000

//Criando vetor como variável GLOBAL
short vector[SIZE];

//"Main" das threads: multiplica os valores do vetor por 2 
void *multiply_vector(void *thread_id) {
  long id = (long) thread_id;
  for(long i = id; i < SIZE; i=i+2) {
    vector[i] *= 2;
    //printf("Thread %ld: Position %ld\n", id, i);
  }
  pthread_exit(NULL);
};

//Imprime o vetor no terminal
void print_vector(){
  for(int i = 0; i < SIZE; i++) {
    if(i%10==0) printf("\n%i ", vector[i]);
    else printf("%i ", vector[i]);
  }
  printf("\n");
}

//Inicializa o vetor com valor 1
void initialize_vector(){
  for(int i = 0; i < SIZE; i++) {
    vector[i] = 1;
  }
}

//Verifica se todos os valores do vetor foram dobrados
int verify_vector(){
  int num_errors = 0;
  for(int i = 0; i < SIZE; i++) {
    if(vector[i] != 2) num_errors++;
  }
  return num_errors;
}

int main(void){
  initialize_vector();

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

  printf("Program finished with %i error(s)!\n", verify_vector());

  //print_vector();

  pthread_exit(NULL);
  return 0;
}
