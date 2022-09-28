#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[]){  
	if (argc!=2){
		printf("Cantidad incorrecta de parametros: %d",argc);
		return 0; //Aca iria el exec();
	}
	int Path_Max = 1024;
	char cwd[Path_Max];
	char *direccion;
	char *nombreArchivo;
	direccion = getcwd(cwd,sizeof(cwd));
	int i=0; 
	while (direccion[i])
		i++;
		
	direccion[i++]='/';
	nombreArchivo = argv[1];
	int j=0;
	while (nombreArchivo[j])
		direccion[i++]= nombreArchivo[j++];
	direccion[i]= '\0';
	int estado = creat(direccion,0777);
	if (estado == -1)
		printf("Error al crear el archivo %s\n",nombreArchivo);
	else
		printf("Archivo: %s  Creado en la ruta %s\n",nombreArchivo,direccion);
		
	return 0; //Aca iria el exec();
	

}
