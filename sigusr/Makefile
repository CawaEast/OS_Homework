target:sigusr
delete:
	rm -f *.o
cat.o: cat.c
	gcc sigusr.c -o sigusr.o -c
cat: cat.o
	gcc -s sigusr.o -o sigusr
clean: 
	rm -f sigusr *.o
