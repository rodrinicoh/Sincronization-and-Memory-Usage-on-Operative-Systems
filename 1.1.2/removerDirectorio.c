#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	char *direccion;
	if (argc != 2){
		printf("No se ingreso un directorio");
		return 0; //Aca deberia ir algo del exec();
	}	
	int i=0;
	direccion = argv[1];
	while (direccion[i])
		i++;
	direccion[i] = '\0';
	int estado;
	estado = rmdir(direccion);
	if (estado == -1)
		printf("Error al remover el directorio");
	else
		printf("Directorio removido: %s\n",direccion);
	return 0; //Aca deberia ir algo del exec();

}
