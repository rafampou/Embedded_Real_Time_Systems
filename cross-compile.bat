del compiler.log
del compliler_error.log
del Timers.o
arm-linux-gnueabihf-g++.exe -O3 main.c queue.c queue.h timer.c timer.h -o Timers.o -pthread 2>compiler_error.log
winscp.com root:espx2019@192.168.1.7 /command "put Timers.o /home/pi/"
