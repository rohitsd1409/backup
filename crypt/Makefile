

prog: prog.c sec.o
	gcc -o prog prog.c sec.o -lcrypt
sec.o: sec.c sec.h
	gcc -c sec.c
