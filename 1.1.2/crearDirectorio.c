#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	char cwd[1024];
	char *direccion;
	direccion = getcwd(cwd,sizeof(cwd));
	int i=0;
	if (argc != 2){
		printf("El directorio a crear no es valido");
		return 0; //Aca deberia ir algo del exec();
	}	
	while (direccion[i])
	i++;
	direccion[i++]= '/';
	char *f;
	f = argv[1]; //Es el nombre de la carpeta, ver si requerir barras o ke
	while (*f)
		direccion[i++] = *f++;
	direccion[i] = '\0';
	int estado;
	estado = mkdir(direccion,0700);
	if (estado == -1)
		printf("Error al crear el directorio");
	else
		printf("Directorio creado:%s\n",direccion);
	return 0; //Aca deberia ir algo del exec();

}
