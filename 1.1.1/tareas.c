#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define UNIDAD_T 1

struct opciones{
	int color;
	int parcial_total;
	int verificacion_reparacion;
	int llanta_balanceo;
	int cantA;
	int cantB;
	int cantC;
};

int pipeA[2], pipeB[2], pipeC[2], finpipeA[2], finpipeB[2], finpipeC[2];
sem_t semA,semB,semC, fin_semB, fin_semA, fin_semC;
int time_inicial;

void hora(){
	printf("%d-",(int)time(NULL)-time_inicial);
}

void *subtareaA(void *dato){
	while(1){
		sem_wait(&semA);
		hora();
		printf("Tarea A: Comienza trabajo\n");
		struct opciones *arg = (struct opciones *) dato;
		int parcial_total = arg->parcial_total;
		if (parcial_total == 1){ //Es una tarea total, demanda tres unidades de tiempo
			hora();
			printf("Tarea A: Realizando tarea total (3 unidades de tiempo)\n");
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
		}
		else{
			hora();
			printf("Tarea A: Realizando tarea parcial (1 unidad de tiempo) \n");
			sleep(UNIDAD_T);
		}
		hora();
		printf("Tarea A: Fin trabajo\n");
		sem_post(&fin_semA);
	}
}

//Proceso que maneja la tarea A.
void tareaA(){
	sem_init(&semA,0,0);
	sem_init(&fin_semA,0,0);
	struct opciones arg;
	pthread_t hilo[2];
	pthread_create(&hilo[0],NULL,subtareaA,&arg);
	pthread_create(&hilo[1],NULL,subtareaA,&arg);
	while(1){
		close(pipeA[1]);
		read(pipeA[0],&arg,sizeof(struct opciones));
		int tareas = arg.cantA;
		int i;
		for (i = 0 ; i < tareas ; i++)
			sem_post(&semA);
		for (i = 0 ; i < tareas ; i++)
			sem_wait(&fin_semA);
		close(finpipeA[0]);
		write(finpipeA[1],&tareas,sizeof(tareas));
		hora();
		printf("Tarea A le comunica al coordinador que ya termino\n");
	}
	exit(0);
}

void *subtareaB(void *dato){
	while(1){
		sem_wait(&semB);
		hora();
		printf("Tarea B: Comienza trabajo\n");
		struct opciones *arg = (struct opciones *) dato;
		int verificacion_reparacion = arg->verificacion_reparacion;
		if (verificacion_reparacion == 1){ //Es una reparacion, demanda 2 unidades
			hora();
			printf("Tarea B: Realizando reparacion (2 unidades de tiempo) \n");
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
		}
		else{
			hora();
			printf("Tarea B: Realizando verificacion (1 unidad de tiempo) \n");
			sleep(UNIDAD_T);
		}
		hora();
		printf("Tarea B: Fin trabajo\n");
		sem_post(&fin_semB);
	}
}

void tareaB(){
	sem_init(&semB,0,0);
	sem_init(&fin_semB,0,0);
	struct opciones arg;
	pthread_t hilo[2];
	pthread_create(&hilo[0],NULL,subtareaB,&arg);
	pthread_create(&hilo[1],NULL,subtareaB,&arg);
	while(1){
		close(pipeB[1]);
		read(pipeB[0],&arg,sizeof(struct opciones));
		int tareas = arg.cantB;
		int i;
		for (i = 0 ; i < tareas ; i++)
			sem_post(&semB);
		for (i = 0 ; i < tareas ; i++)
			sem_wait(&fin_semB);
		close(finpipeB[0]);
		write(finpipeB[1],&tareas,sizeof(tareas));
		hora();
		printf("Tarea B le comunica al coordinador que ya termino\n");
	}
}

void *subtareaC(void *dato){
	while(1){
		sem_wait(&semC);
		hora();
		printf("Tarea C: Comienza trabajo\n");
		struct opciones *arg = (struct opciones *) dato;
		int llanta_balanceo = arg->llanta_balanceo;
		if (llanta_balanceo == 1){ //Es un balanceo.
			hora();
			printf("Tarea C: Realizando Balanceo y Rotacion (3 unidades de tiempo) \n");
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
		}
		else{
			hora();
			printf("Tarea C: Realizando reparacion 4 ruedas (4 unidades de tiempo) \n");
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);
			sleep(UNIDAD_T);		
		}
		hora();
		printf("Tarea C: Fin trabajo\n");
		sem_post(&fin_semC);
	}
}

void tareaC(){
	sem_init(&semC,0,0);
	sem_init(&fin_semC,0,0);
	struct opciones arg;
	pthread_t hilo[2];
	pthread_create(&hilo[0],NULL,subtareaC,&arg);
	pthread_create(&hilo[1],NULL,subtareaC,&arg);
	while(1){
		close(pipeC[1]);
		read(pipeC[0],&arg,sizeof(struct opciones));
		int tareas = arg.cantC;
		int i = arg.cantC;
		for (i = 0 ; i < tareas ; i++)
			sem_post(&semC);
		for (i = 0 ; i < tareas ; i++)
			sem_wait(&fin_semC);
		close(finpipeC[0]);
		write(finpipeC[1],&tareas,sizeof(tareas));
		hora();
		printf("Tarea C le comunica al coordinador que ya termino\n");
	}
}


int main(){
	time_inicial  = time(NULL);
	pipe(pipeA);
	pipe(finpipeA);
	pipe(finpipeB);
	pipe(finpipeC);
	pipe(pipeB);
	pipe(pipeC);
	int pid = fork();
	if (pid == 0)//Estoy en tarea A
		tareaA();
	else{
		if (fork() == 0)//Estoy en Tarea B
			tareaB();
		else{
			if (fork() ==0)//Estoy en Tarea C
				tareaC();
			else{//Aca estoy en el padre: El Coordinador de Procesos.
				while(1){
					struct opciones arg;
					char reader[20];
					int cant_tareas;
					int iteracion = 0;
					do{
						if (iteracion > 0)
							printf("----------------------------\nOpcion Incorrecta\n----------------------------\n");
						printf("----------------------------\nEscriba la cantidad de tareas que desea ejecutar\n----------------------------\n");
						scanf("%s",reader);
						cant_tareas = atoi(reader);
						iteracion = 1;
					}while(cant_tareas < 4 || cant_tareas > 6);
					printf("----------------------------\n");
					hora();
					printf("Ingreso: %d\n----------------------------\n",cant_tareas);
					close(pipeA[0]);
					close(pipeB[0]);
					close(pipeC[0]);
					arg.cantA = 2;
					arg.cantB = 2;
					arg.parcial_total = (int) rand() % 2; //Trabajo perteneciente a la tarea A.
					arg.verificacion_reparacion = (int) rand() % 2; //Trabajo perteneciente a la tarea B.
					if (cant_tareas != 4){//Si son 5 o 6 tareas, se ejecutan dos tareas de C.
						arg.cantC = 2;
						arg.llanta_balanceo = (int) rand() % 2; //Trabajo perteneciente a la tarea C.
						if (cant_tareas == 5){
							printf("----------------------------\n");
							printf("Se ejecutaran 5 tareas: dos instancias de Tarea A, una instancia de Tarea B y dos instancias de Tarea C\n");
							printf("----------------------------\n");
							arg.cantB = 1;
						}
						else{
							printf("----------------------------\n");
							printf("Se ejecutaran 6 tareas: dos instancias de Tarea A, dos instancias de Tarea B y dos instancias de Tarea C\n");
							printf("----------------------------\n");
						}
					}
					else{						
						printf("----------------------------\n");
						printf("Se ejecutaran 4 tareas: dos instancias de Tarea A y dos instancias de Tarea B\n");
						printf("----------------------------\n");
					}
					write(pipeA[1],&arg,sizeof(struct opciones));
					write(pipeB[1],&arg,sizeof(struct opciones));
					if (cant_tareas != 4)
						write(pipeC[1],&arg,sizeof(struct opciones));
					int c;
					//Con estos tres read de finPipeX, espero a que los hijos terminen de ejecutar
					read(finpipeA[0],&c,sizeof(c));
					hora();
					printf("COORDINADOR: Tarea A termino\n");
					read(finpipeB[0],&c,sizeof(c));
					hora();
					printf("COORDINADOR: Tarea B termino\n");
					if (cant_tareas != 4){
						read(finpipeC[0],&c,sizeof(c));
						hora();
						printf("COORDINADOR: Tarea C termino\n");
					}
					hora();
					printf("Fin del ciclo\n");
				}
				exit(0);
			}
		}
	}
return 0;
}
