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
int longitudCola(int cola){
    int msqid = cola;
    struct msqid_ds buf;
	msgctl(msqid, IPC_STAT, &buf);
    int toReturn = buf.msg_qnum;
    return toReturn;
}

void hora(){
	printf("%d-",(int)time(NULL)-time_inicial);
}

int main(int argc, char **argv){
	time_inicial = time(NULL);
	key_t key2,key3,key4;
	key2 = KEY_COLA_COMIDAS;
	key3 = KEY_COLA_PEDIDOS;
	key4 = KEY_COLA_COMIDAS_DISPONIBLES;
	int cola_comida, cola_pedidos,cola_comidas_disponibles;
	cola_comida = msgget(key2, 0666);
	cola_pedidos = msgget(key3, 0666);
	cola_comidas_disponibles = msgget(key4, 0666);
	message msj;
	while(1){
		//Camarero espera hasta que alguien realice un pedido.
		msgrcv(cola_pedidos, &msj, SIZE_MSG,TIPO_PEDIDO,0);
		hora();
		printf("Camarero busca comida, el cliente realizo un pedido\n");
		sleep(3);
		//Si msj.dato es 1, el cliente solicito un menu vegetariano. Si es 0, el cliente solicito un menu de carne.
		int tipo_pedido = msj.dato;
		long type = TIPO_VEGE;
		if (tipo_pedido == 0)
			type = TIPO_CARNE;
		//Espero que haya comida en la cola
		msgrcv(cola_comida, &msj, SIZE_MSG, type, 0);
		//Saque una comida de la cola, nuevo lugar disponible en la mesa de comidas.
		msj.type = TIPO_DISPONIBLE;
		msj.dato = tipo_pedido;
		msgsnd(cola_comidas_disponibles, &msj, SIZE_MSG, IPC_NOWAIT);
		int p = longitudCola(cola_comida);
		hora();
		printf("Camarero: entrega la comida al cliente, quedan %d comidas\n",p);
		//Le entrego la comida al cliente
		msj.type = type;
		msgsnd(cola_pedidos, &msj, SIZE_MSG, IPC_NOWAIT);		
	}
}
