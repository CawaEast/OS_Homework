target:main
$%.o: $%.c
	gcc $%.cpp -o $%.o -c
main: simplesh.o
	gcc  simplesh.o -o simplesh
clean:
	rm -f simplesh *.o
