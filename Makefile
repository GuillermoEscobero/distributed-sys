all:
	gcc -Wall -pthread -lrt -o server server.c avltree.c
	gcc -Wall -lrt -c keys.c
	ar -rv libkeys.a keys.o
	gcc -Wall -lrt -o client client.c libkeys.a

clean:
	rm -f server client keys.o libkeys.a
