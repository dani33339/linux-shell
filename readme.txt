Author: Daniel Markov
Start using custom shell with following steps:

Install readline
sudo apt-get install libreadline-dev

To compile use
gcc -c DemoCmd.c
gcc -c main.c
gcc -o customCmd DemoCmd.o main.o -lreadline

To run shell use
./customCmd

