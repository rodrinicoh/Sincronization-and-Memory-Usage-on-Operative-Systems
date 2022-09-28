#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
	if (argc == 1 || argc == 2){
		printf("Cantidad incorrecta de parametros para permisos de archivo\n");
		return 0;
	}
	
	char *archivo;
	archivo = argv[1];
	char *mode;
	mode = argv[2]; 
	int i = atoi(mode);
	i = strtol(mode,0,8);
	int estado = chmod(archivo,i);
	if (estado < 0)
		printf("Error al cambiar permiso de archivo");
	return 0;
}
