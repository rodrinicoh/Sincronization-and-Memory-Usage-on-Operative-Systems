#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//Tres cocineros
//Cola de comidas capacidad 10
//Inicialmente 50 clientes, solo 30 mesas disponibles. Cliente espera hasta que termine la comida y la mesa este limpia.
//Cocinero no comienza a prereparar comida si no hay espacio en la cola.
//Cliente pide comida, el camarero ira a la cola, tomara una comida y se la entrega al lciente. El cliente empieza a comer.
//Cuando no hay pedidos de los clientes, o cando la cola esta vacia, el camarero tiene que esperar
//Despues de que un cliente termina la commida y se levanta de la mesa, el limpiador limpiara la mesa.

#define mesas_iniciales 30
#define clientes 50
#define cocineros 3

sem_t cola_disp;
sem_t cola_comidas;
sem_t mesas_disp, pido_comida,recibo_comida,limpiar_mesa;
int time_inicial;

//Funcion para imprimir el tiempo actual.
void hora(){
	printf("%d-",(int)time(NULL)-time_inicial);
}

void *limpiador(void *dato){
	while(1){
		//Espero a que haya una mesa para limpiar.
		sem_wait(&limpiar_mesa);
		sleep(2);
		int p;
		sem_getvalue(&mesas_disp,&p);
		p++;
		hora();
		printf("Limpiador limpia la mesa, quedan %d mesas disponibles\n",p);
		//Termine de limpiar la mesa, ya esta disponible para que se siente un cliente.
		sem_post(&mesas_disp);	
	}
}

void *camarero (void *dato){
	while(1){
		//Camarero espera hasta que alguien realice un pedido.
		sem_wait(&pido_comida);
		hora();
		printf("Camarero busca comida, el cliente pidio comida\n");
		sleep(1);
		//Espero que haya comida en la cola
		sem_wait(&cola_comidas);
		//Saque una comida de la cola, nuevo lugar disponible en la cola de comidas.
		sem_post(&cola_disp);
		int p;
		sem_getvalue(&cola_comidas,&p);
		hora();
		printf("Camarero entrega la comida al cliente, quedan %d comidas\n",p);
		//Le entrego la comida al cliente
		sem_post(&recibo_comida);
		
	}
}

void *clienteConWhile(void *dato){
	int p = (int) dato;
	while(1){
		//Me fijo si hay mesas disponibles
		sem_wait(&mesas_disp);
		//Le aviso al camarero sobre mi pedido.
		sem_post(&pido_comida);
		int q;
		sem_getvalue(&mesas_disp,&q);
		hora();
		printf("Cliente %d: Me sente (quedan %d mesas), pedi comida y estoy esperandola\n",p,q);
		//Espero a que el camarero me traiga la comida
		sem_wait(&recibo_comida);
		hora();
		printf("Cliente %d: Comienzo a comer\n",p);
		sleep(4);
		hora();
		printf("Cliente %d: Termino de comer\n",p);
		//Libero la mesa para que el limpiador limpie la mesa.
		sem_post(&limpiar_mesa);
	}
}

void *cliente(void *dato){
	int p = (int) dato;
	//Me fijo si hay mesas disponibles.
	sem_wait(&mesas_disp);
	//Le aviso al camarero sobre mi pedido.
	sem_post(&pido_comida);
	int q;
	sem_getvalue(&mesas_disp,&q);
	hora();
	printf("Cliente %d: Me sente (quedan %d mesas), pedi comida y estoy esperandola\n",p,q);
	//Espero a que el camarero me traiga la comida.
	sem_wait(&recibo_comida);
	hora();
	printf("Cliente %d: Comienzo a comer\n",p);
	sleep(4);
	hora();
	printf("Cliente %d: Termino de comer\n",p);
	//Libero la mesa para que el limpiador limpie la mesa.
	sem_post(&limpiar_mesa);
	return 0;
}

void *cocinero(void *dato){	
	int q = (int )dato;
	while(1){
		//Si hay lugar en la cola de comidas, comienzo a cocinar.
		sem_wait(&cola_disp);
		hora();
		printf("Cocinero %d: Empiezo a cocinar\n",q);
		sleep(2);
		//Coloco la comida en la cola de comidas.
		sem_post(&cola_comidas);
		int r;
		sem_getvalue(&cola_comidas,&r);
		hora();
		printf("Cocinero %d: Agrego una comida a la cola, hay %d comidas en la cola\n",q,r);
	}
}

int main(){
	time_inicial = time(NULL);
	sem_init(&cola_disp,0,10);
	sem_init(&cola_comidas, 0, 0);
	sem_init(&pido_comida,0,0);
	sem_init(&recibo_comida,0,0);
	sem_init(&limpiar_mesa, 0,0);
	sem_init(&mesas_disp,0,mesas_iniciales);
	pthread_t hilos_cocineros[3];
	pthread_t hilo_camarero;
	pthread_t hilo_limpiador;
	pthread_t hilos_clientes[clientes];
	int i;
	for (i = 0; i < cocineros ; i++)
		pthread_create(&hilos_cocineros[i],NULL,cocinero,(void *)i);
	for (i = 0; i < clientes ; i++)
		pthread_create(&hilos_clientes[i],NULL,cliente,(void *)i);

	pthread_create(&hilo_camarero,NULL,camarero,(void *)1);
	pthread_create(&hilo_limpiador, NULL, limpiador, NULL);
	
	pthread_join(hilo_limpiador, NULL);
	pthread_join(hilo_camarero,NULL);
	for (i = 0; i < cocineros ; i++)
		pthread_join(hilos_cocineros[i],NULL);		
	for (i = 0; i < clientes ; i++)
		pthread_join(hilos_clientes[i],NULL);
	return 0;
}