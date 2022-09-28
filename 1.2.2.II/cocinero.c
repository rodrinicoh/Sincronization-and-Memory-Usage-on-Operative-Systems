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
	pid_t tt = getpid();
	time_inicial = time(NULL);
	int q = tt % getppid();
	key_t key2 = KEY_COLA_COMIDAS;
	key_t key4 = KEY_COLA_COMIDAS_DISPONIBLES;
	int cola_comida,cola_comidas_disponibles;
	cola_comida = msgget(key2, 0666);
	cola_comidas_disponibles = msgget(key4,0666);
	int iteracion = 0;
	message msg;
	while(1){
		//Espero que haya lugar en la de comidas para comenzar a cocinar.
		msgrcv(cola_comidas_disponibles,&msg,SIZE_MSG,TIPO_DISPONIBLE,0);
		hora();
		printf("Cocinero %d: Empiezo a cocinar\n",q);
		//Si tipo_pedido es cero, el pedido es de carne. Si es uno, el pedido es vegetariano.
		//Cualquier otro valor para tipo_pedido, el chef cocina vegetariano si el numero es impar, cocina de carne si el numero es par.
		int tipo_pedido = msg.dato;
		sleep(2);
		long type;
		if (tipo_pedido == 0)
			type = TIPO_CARNE;
		else
			type = TIPO_VEGE;
		msg.type = type;
		//Coloco la comida en la cola de comidas.
		msgsnd(cola_comida, &msg, SIZE_MSG, IPC_NOWAIT);
		int r = longitudCola(cola_comida);
		hora();
		char *tipo = "Vegetariano";
		if (tipo_pedido % 2 == 0)
			tipo = "Carne";
		printf("Cocinero %d: Agrego una comida %s a la cola, hay %d comidas en la cola\n",q,tipo,r);
		iteracion++;
	}
	return 0;
}
