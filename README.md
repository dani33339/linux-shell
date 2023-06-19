
Start using custom shell with following steps:

Install readline
sudo apt-get install libreadline-dev

To compile use \n
gcc -c DemoCmd.c \n
gcc -c main.c \n
gcc -o customCmd DemoCmd.o main.o -lreadline \n

To run shell use
./customCmd

