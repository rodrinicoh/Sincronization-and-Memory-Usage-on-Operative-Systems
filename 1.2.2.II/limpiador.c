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
	key_t key, key2;
	int cola_mesas_limpiar ,cola_mesas_disponibles;
	key = KEY_COLA_MESAS_A_LIMPIAR;
	key2 = KEY_MESAS_DISPONIBLES;
	cola_mesas_limpiar= msgget(key, 0666);
	cola_mesas_disponibles = msgget(key2, 0666);
	message msg;
	while(1){
		//Espero a que haya una mesa para limpiar.
		msgrcv(cola_mesas_limpiar,&msg,SIZE_MSG,TIPO_LIMPIAR_MESA,0);
		sleep(2);
		int p = longitudCola(cola_mesas_disponibles);
		p++;
		hora();
		printf("Limpiador limpia la mesa, quedan %d mesas disponibles\n",p);
		//Termine de limpiar la mesa, ya esta disponible para que se siente un cliente.
		msg.type = TIPO_DISPONIBLE;
		msgsnd(cola_mesas_disponibles, &msg, SIZE_MSG, IPC_NOWAIT);
	}
	return 0;
}
