#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define cant_leches 3
#define cant_leches_iniciales 2
sem_t leches; //Hay leches en la heladera
sem_t compradores; //Una persona puede ir a comprar leche
sem_t puerta;
int time_inicial;

void hora(){
	printf("%d-",(int)time(NULL)-time_inicial);
}

void *operar(void *dato){
	int persona = (int ) dato;
	while(1){
			sem_wait(&puerta);//Espero mientras la puerta este cerrada
			if (sem_trywait(&leches) == 0){ //Trywait exitoso.
				int cant;
				sem_getvalue(&leches,&cant);
				hora();
				printf("Persona %d: Consumo leche, quedan: %d\n",persona,cant);
				sem_post(&puerta);
				sleep(4);
			}
			else{
				hora();
				printf("Persona %d: No hay leche\n",persona);
				sem_post(&puerta); //Cierro la puerta y me fijo si hay alguien comprando.
				if (sem_trywait(&compradores) == 0){ //Si compradores = 1, no hay nadie comprando, entonces trywait devuelve 0.
					hora();
					printf("Persona %d: Voy al supermercado\n",persona);
					hora();
					printf("Persona %d: Llego al supermercado\n",persona);
					hora();
					printf("Persona %d: Compro la leche\n",persona);
					sleep(1);
					sem_wait(&puerta); //Espero a que no haya nadie en la heladera para colocar las leches, es decir, que este la puerta cerrada.
					hora();
					printf("Persona %d: Llego a casa y guardo la leche\n",persona);
					int i;
					for (i = 0; i < cant_leches ; i++)
						sem_post(&leches);
					sem_post(&puerta); //Cierro la puerta
					sem_post(&compradores); //Notifico que no hay nadie comprando una vez que ya compre y repuse las leches.
				}
				else{//No hay leche, ya hay alguien comprando
					hora();
					printf("Persona %d: No hay leche, ya hay alguien comprando\n",persona);
					sleep(2);
				}
			}
	}
}

int main(){
	time_inicial = time(NULL);
	sem_init(&leches,0,cant_leches_iniciales);
	sem_init(&compradores,0,1);
	sem_init(&puerta,0,1);
	pthread_t hilo[2];
	int i; 
	for (i = 0 ; i < 2 ; i++)
		pthread_create(&hilo[i],NULL,operar,(void*)i+1);
	for (i = 0; i < 2 ; i++)
		pthread_join (hilo[i],NULL);
	exit(0);
}
