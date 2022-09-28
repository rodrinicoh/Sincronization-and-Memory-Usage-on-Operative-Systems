#!/bin/bash
clear
rm ej
gcc -o cocinero cocinero.c -Wall -lpthread
gcc -o limpiador limpiador.c -Wall -lpthread
gcc -o cliente cliente.c -Wall -lpthread
gcc -o camarero camarero.c -Wall -lpthread
gcc -o ej main.c -Wall -lpthread
./ej