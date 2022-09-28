#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_QUEUE 12;
#define cant_leches 2
#define cant_leches_iniciales 1
#define TIPO_LECHE 1L
#define TIPO_COMPRANDO 2L
#define TIPO_PUERTA 3L
#define companieros 4

struct heladera{
	long type;
	int leches;
};
typedef struct heladera message;
int time_inicial;

#define SIZE_MSG sizeof(message) - sizeof(long)

void horaActual(){
	printf("%d-",(int)time(NULL)-time_inicial);	
}

int hijo(){
	time_inicial = time(NULL);
	message msj;
	key_t key = KEY_QUEUE;
	pid_t my = getpid() % companieros;
	int queueid = msgget(key, 0666 | IPC_CREAT);
	while(1){
		//Msgrcv devuelve la cantidad de bytes de lo recibido, si no recibe nada da -1.
		msgrcv(queueid,&msj, SIZE_MSG,TIPO_PUERTA,0);//Espero que este la puerta cerrada para fijarme si hay leche.
		if (msgrcv(queueid, &msj, SIZE_MSG,TIPO_LECHE,IPC_NOWAIT) != -1){//Hay leche, consumo.
			horaActual();
			printf("Persona %d: Consumo leche\n", my);
			msj.type = TIPO_PUERTA;
			msgsnd(queueid, &msj, SIZE_MSG, IPC_NOWAIT);//Ya consumi, cierro la puerta.
		}
		else{//No hay leche, me fijo si alguien esta comprando
			msj.type = TIPO_PUERTA;
			msgsnd(queueid, &msj, SIZE_MSG, IPC_NOWAIT);//Cierro la puerta, me voy a fijar si hay alguien comprando.
			if (msgrcv(queueid, &msj, SIZE_MSG, TIPO_COMPRANDO, IPC_NOWAIT ) != -1){//No hay nadie comprando, voy.
				horaActual();
				printf("Persona %d: No hay leche, voy al supermercado\n", my);
				horaActual();
				printf("Persona %d: Llego al supermercado\n", my);
				sleep(1);
				horaActual();
				//Espero a que la puerta este cerrada para llenar la heladera.
				msgrcv(queueid, &msj, SIZE_MSG, TIPO_PUERTA,0);
				printf("Persona %d: Llego a casa y guardo la leche\n", my);
				int i;
				msj.type = TIPO_LECHE;
				//Coloco las leches en la heladera.
				for (i = 0; i < cant_leches ; i++)
					msgsnd(queueid, &msj, SIZE_MSG, IPC_NOWAIT);
				//Ya llene la heladera, cierro la puerta.
				msj.type = TIPO_PUERTA;
				msgsnd(queueid, &msj, SIZE_MSG, IPC_NOWAIT);
				//Informo que no hay nadie comprando.
				msj.type = TIPO_COMPRANDO;
				msgsnd(queueid, &msj, SIZE_MSG, IPC_NOWAIT);
			}
			else{
				horaActual();
				printf("Persona %d: No hay leche, ya hay alguien comprando\n",my);
			}
		}
		sleep(2);
	}
	return 0;
}

//IPC NO WAIT, NO ESPERA A QUE ALGUIEN CONTESTE. SI LE MANDO 0, ESPERA HASTA QUE ALGUIEN RECIBA EL MSJ.
int main(){
	message msj;
	key_t key = KEY_QUEUE;
	int queueid = msgget(key, 0666);
	if (msgctl(queueid, IPC_RMID, NULL) == -1)
		printf("Cola %d borrada\n",key);
	else
		printf("Cola %d no borrada\n",key);
	queueid = msgget(key, 0666 | IPC_CREAT);
	int i;
	msj.type = TIPO_LECHE;
	for (i = 0; i < cant_leches_iniciales ; i++)
		msgsnd(queueid,&msj, SIZE_MSG,IPC_NOWAIT);
	msj.type = TIPO_COMPRANDO;
	msgsnd(queueid, &msj, SIZE_MSG,IPC_NOWAIT);
	//Inicialmente la heladera esta cerrada. Si hay en la cola algo de TIPO_PUERTA, significa que esta cerrada.
	msj.type = TIPO_PUERTA;
	msgsnd(queueid, &msj, SIZE_MSG, IPC_NOWAIT);
	
	for (i = 0 ; i < companieros ; i++){
		pid_t pid = fork();
		if (pid == 0)
			hijo();
	}
	wait(NULL);
}
