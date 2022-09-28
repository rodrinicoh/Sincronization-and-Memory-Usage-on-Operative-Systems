#!/bin/bash
clear
rm ejecutableLeches
gcc -o ejecutableLeches leches.c -Wall -lpthread
./ejecutableLeches