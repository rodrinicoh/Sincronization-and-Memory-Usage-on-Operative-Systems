#ifndef UNIQUE_ID_FOR_HEADER
#define UNIQUE_ID_FOR_HEADER
	//Tres cocineros
	//Cola de comidas capacidad 10
	//Inicialmente 50 clientes, solo 30 mesas disponibles. Cliente espera hasta que termine la comida y la mesa este limpia.
	//Cocinero no comienza a prereparar comida si no hay espacio en la cola.
	//Cliente pide comida, el camarero ira a la cola, tomara una comida y se la entrega al lciente. El cliente empieza a comer.
	//Cuando no hay pedidos de los clientes, o cando la cola esta vacia, el camarero tiene que esperar
	//Despues de que un cliente termina la commida y se levanta de la mesa, el limpiador limpiara la mesa.
	#define TIPO_CARNE 1L
	#define TIPO_VEGE 2L
	#define TIPO_DISPONIBLE 3L
	#define TIPO_PEDIDO_CARNE 4L
	#define TIPO_PEDIDO_VEGE 5L
	#define TIPO_LIMPIAR_MESA 6L
	#define TIPO_PEDIDO 7L
	#define QUEUE_ERROR 5
	#define KEY_MESAS_DISPONIBLES 1111
	#define KEY_COLA_COMIDAS 1112
	#define KEY_COLA_PEDIDOS 1113
	#define KEY_COLA_COMIDAS_DISPONIBLES 1114
	#define KEY_COLA_MESAS_A_LIMPIAR 1115
	#define mesas_iniciales 30
#define clientes 50
#define cocineros 3
#define SIZE_MSG sizeof(message) - sizeof(long)
struct queue_msg{
	long type;
	int dato;
};

typedef struct queue_msg message;
int time_inicial;

#endif /* UNIQUE_ID_FOR_HEADER */
