#!/bin/bash
clear
rm ejecutableTareas
gcc -o ejecutableTareas tareas.c -Wall -lpthread
./ejecutableTareas