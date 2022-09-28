#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "tipos.h"

int time_inicial;

void hora(){
	printf("%d-",(int)time(NULL)-time_inicial);
}

int main(){
	time_inicial = time(NULL);
	key_t key = KEY_MESAS_DISPONIBLES;
	key_t key2 = KEY_COLA_COMIDAS;
	key_t key3 = KEY_COLA_PEDIDOS;
	key_t key4 = KEY_COLA_COMIDAS_DISPONIBLES;
	key_t key5 = KEY_COLA_MESAS_A_LIMPIAR;
	int cola_comida, cola_mesas_disponibles, cola_pedidos,cola_comidas_disponibles,cola_mesas_limpiar;
	cola_mesas_disponibles = msgget(key,0666);
	cola_comida = msgget(key2, 0666);
	cola_pedidos = msgget(key3,0666);
	cola_comidas_disponibles = msgget(key4,0666);
	cola_mesas_limpiar = msgget(key5,0666);
	
	if (msgctl(cola_comida, IPC_RMID, NULL) == -1)
		printf("La cola no existia en el sistema\n");
	else
		printf("La cola %d ya existia, la borro y la creo nuevamente\n",key);
	if (msgctl(cola_mesas_disponibles, IPC_RMID, NULL) == -1)
		printf("La cola no existia en el sistema\n");
	else
		printf("La cola %d ya existia, la borro y la creo nuevamente\n",key2);
	if (msgctl(cola_pedidos, IPC_RMID, NULL) == -1)
		printf("La cola no existia en el sistema\n");
	else
		printf("La cola %d ya existia, la borro y la creo nuevamente\n",key3);
	if (msgctl(cola_comidas_disponibles, IPC_RMID, NULL) == -1)
		printf("La cola no existia en el sistema\n");
	else
		printf("La cola %d ya existia, la borro y la creo nuevamente\n",key4);
	if (msgctl(cola_mesas_limpiar, IPC_RMID, NULL) == -1)
		printf("La cola no existia en el sistema\n");
	else
		printf("La cola %d ya existia, la borro y la creo nuevamente\n",key5);
	printf("--------------------------------\n\n\n");
	
	cola_mesas_disponibles = msgget(key, 0666 | IPC_CREAT);
	cola_comida = msgget(key2, 0666 | IPC_CREAT);
	cola_pedidos = msgget(key3, 0666 | IPC_CREAT);
	cola_comidas_disponibles = msgget(key4,0666 | IPC_CREAT);
	cola_mesas_limpiar = msgget(key5,0666 | IPC_CREAT);
	if (cola_mesas_disponibles < 0)
		exit(QUEUE_ERROR);
	if (cola_comida < 0)
		exit(QUEUE_ERROR);
	if (cola_pedidos < 0)
		exit(QUEUE_ERROR);
	if (cola_comidas_disponibles < 0)
		exit(QUEUE_ERROR);
	if (cola_mesas_limpiar < 0)
		exit(QUEUE_ERROR);
	message msj;
	msj.type = TIPO_DISPONIBLE;
	int i;
	//Cargo en la cola de mesas las mesas disponibles.
	for (i = 0 ; i < mesas_iniciales ; i++)
		msgsnd(cola_mesas_disponibles,&msj,SIZE_MSG,IPC_NOWAIT);
	//Cargo en la cola de comidas los lugares disponibles para cocinar.
	for (i = 0 ; i < 10 ; i++){
		//Cargo el tipo de pedido a cocinar inicialmente, si es par cocina de carne, si es impar vegetariano.
		msj.dato = i % 2;
		msgsnd(cola_comidas_disponibles, &msj, SIZE_MSG,IPC_NOWAIT);
	}
	for ( i= 0 ; i < 55 ; i++){
		char *argumentos[] = {NULL,NULL};
		pid_t pid = fork();
		if (pid == 0){
			if (i < 3)
				argumentos[0] = "./cocinero";
			if (i == 3)
				argumentos[0] = "./camarero";
			if (i == 4)
				argumentos[0] = "./limpiador";
			if (i > 4)
				argumentos[0] = "./cliente";
			execvp(argumentos[0],argumentos);
			exit(0);
		}
	}
	wait(NULL);
	return 0;
}
