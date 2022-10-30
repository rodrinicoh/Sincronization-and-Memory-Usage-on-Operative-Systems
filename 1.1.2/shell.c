#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){


	if (argc !=1){
		printf("Cantidad incorrecta de parametros\n");
		return -1;
	}
	printf("--------------------------SHELL---------------------------\n");
	printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	printf("Para visualizar la ayuda escriba '6'\n");
	printf("Ingrese el numero de comando deseado:\n");
	printf("$");
	int opcion = 0;
	char *f = malloc(sizeof(char)*1024);
	while(1){
		scanf("%s",f);
        	opcion = atoi(f);
		if (opcion == 9)
			return 0;
		pid_t pid = fork();
		if (pid == 0){ //ESTOY EN EL HIJO, LLAMO A ALGUN COMANDO
			if (opcion!=0)
				printf("Usted selecciono la opcion: %d\n",opcion);
			char *nombreArchivo = malloc(sizeof(char)*256);
			switch(opcion){
				case 1: {//Crear un directorio
					printf("--------COMANDO PARA CREAR UN DIRECTORIO------------------\n");
					printf("--------Ingrese un nombre para el nuevo Directorio--------\n$");
					scanf("%s",nombreArchivo);
					char *argumentos[] = {"./crearDirectorio",nombreArchivo,NULL};
					int res = execvp(argumentos[0],argumentos);
					break;
				}
				case 2:{//Remover un directorio
					printf("--------COMANDO PARA REMOVER UN DIRECTORIO----------------\n");
					printf("--------Ingrese un nombre del Directorio a remover--------\n$");
					scanf("%s",nombreArchivo);
					char *argumentos[] = {"./removerDirectorio",nombreArchivo,NULL};
					int res = execvp(argumentos[0],argumentos);
					break;
				}
				case 3:{ //Crear un archivo
					printf("--------COMANDO PARA CREAR UN ARCHIVO---------------------\n");
					printf("--------Ingrese un nombre para el nuevo archivo-----------\n$");
					scanf("%s",nombreArchivo);
					char *argumentos[] = {"./crearArchivo",nombreArchivo,NULL};
					int res = execvp(argumentos[0],argumentos);
					break;
				}
				case 4:{//Mostrar el contenido de un archivo
					printf("--------COMANDO PARA MOSTRAR CONTENIDO DE UN ARCHIVO------\n");
					printf("--------Ingrese el nombre de archivo a visualizar---------\n$");
					scanf("%s",nombreArchivo);
					char *argumentos[] = {"./mostrarArchivo",nombreArchivo,NULL};
					int res = execvp(argumentos[0],argumentos);
					break;

				}
				case 5:{//Modificar los permisos de un archivo
					printf("--------COMANDO PARA MODIFICAR LOS PERMISOS DE UN ARCHIVO-----\n");
					printf("--------Ingrese el nombre del archivo para modificar los permisos---------\n$");
					scanf("%s",nombreArchivo);
					char * permiso = malloc(sizeof(char)*256);
					scanf("%s",permiso);
					char *argumentos[] = {"./cambiarPermisos",nombreArchivo,permiso,NULL};
					int res = execvp(argumentos[0],argumentos);
					break;
				}
				case 6:{
					char *argumentos[] = {"./mostrarAyuda",NULL};
					execvp(argumentos[0],argumentos);
					break;
				}
				case 9:
					printf("---------------------FINALIZANDO SHELL--------------------\n");
					printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
					return 0;
				default:
					printf("Opcion incorrecta");
					break;
			}
			exit(1);
		}
		else{
			wait(NULL);
            		printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
			printf("Para visualizar la ayuda escriba '6'\n");
			printf("Ingrese el numero de comando deseado:\n$");
		}
	}
	return 0;
}
