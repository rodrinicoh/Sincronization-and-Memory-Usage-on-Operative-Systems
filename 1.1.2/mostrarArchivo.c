#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc,char * argv[]){
	if (argc !=2){
		printf("Cantidad incorrecta de parametros para mostrar contenido de archivo\n");
		return 0; //Exec
	}
	int fileDescriptor;
	int size_buf = 1024;
	char buffer[size_buf];
	int i =0;
	char *archivo = argv[1];
	fileDescriptor = open(archivo,O_RDONLY);
	if (fileDescriptor < 0){
		printf("Error al abrir el archivo\n");
		return 0;
	}
	printf("Informacion del archivo '%s':\n",archivo);
	int bytesLeidos = read(fileDescriptor,buffer,size_buf);
	while (buffer[i] !='\0')
		printf("%c",buffer[i++]);
	printf("\n");
	close(fileDescriptor);
	return 0;
	
}
