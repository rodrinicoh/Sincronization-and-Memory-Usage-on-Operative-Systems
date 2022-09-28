#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "tipos.h"
int time_inicial;

void hora(){
	printf("%d-",(int)time(NULL)-time_inicial);
}

int longitudCola(int cola){
    int msqid = cola;
    struct msqid_ds buf;
	msgctl(msqid, IPC_STAT, &buf);
    int toReturn = buf.msg_qnum;
    return toReturn;
}

int main(int argc, char **argv){	
	time_inicial = time(NULL);
	message msj;
	key_t key,key2,key3;
	key = KEY_MESAS_DISPONIBLES;
	key3 = KEY_COLA_PEDIDOS;
	key2 = KEY_COLA_MESAS_A_LIMPIAR;
	int cola_mesas_disponibles = msgget(key, 0666);
	int cola_mesas_limpiar = msgget(key2, 0666);
	int cola_pedidos = msgget(key3, 0666);
	
	int p = getpid() % getppid();
	while(1){
		//Espero por una mesa disponible para sentarme.
		msgrcv(cola_mesas_disponibles, &msj, SIZE_MSG,TIPO_DISPONIBLE, 0);
		msj.type = TIPO_PEDIDO;
		msj.dato = 0;
		int tipo_menu = 0;
		//Genero un menu random, le asigno a msj.dato 0 si el menu es de carne, o un 1 si el menu es vegetariano.
		//msj.dato lo utilizara el camarero para saber que pedido le solicitaron.
		int random = rand() * getpid();
		if (random % 2 == 1){
			msj.dato = 1;
			tipo_menu = 1;
		}
		//Le aviso al camarero sobre mi pedido.
		msgsnd(cola_pedidos, &msj, SIZE_MSG, IPC_NOWAIT);
		int q = longitudCola(cola_mesas_disponibles);
		char *menu = "Vegetariano";
		if (tipo_menu == 0)
			menu = "Carne";
		hora();
		printf("Cliente %d: Me sente (quedan %d mesas), pedi menu %s y estoy esperandola\n",p,q,menu);
		
		long type = TIPO_CARNE;
		if (tipo_menu == 1)
			type = TIPO_VEGE;
		//Espero a que el camarero me traiga la comida
		msgrcv(cola_pedidos, &msj, SIZE_MSG, type,0);
		hora();
		printf("Cliente %d: Comienzo a comer\n",p);
		sleep(4);
		hora();
		printf("Cliente %d: Termino de comer\n",p);
		//Libero la mesa para que el limpiador limpie la mesa.
		msj.type = TIPO_LIMPIAR_MESA;
		msgsnd(cola_mesas_limpiar,&msj, SIZE_MSG, IPC_NOWAIT);
		//El cliente vuelve en 60 segundos.
		sleep(60);
	}
	return 0;
}
