target:cat delete
delete:
	rm *.o
cat.o: cat.c
	gcc cat.c -o cat.o -c
cat: cat.o
	gcc -s cat.o -o cat
