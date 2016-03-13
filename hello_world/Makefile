target:hello delete
delete:
	rm hello.o
hello.o: hello.c
	gcc hello_world.c -o hello.o -c
hello: hello.o
	gcc -s hello.o -o hello
