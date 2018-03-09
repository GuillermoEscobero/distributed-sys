all:
	gcc -g -Wall -lrt -pthread -o server server.c
	gcc -g -Wall -lrt -c keys.c
	ar -rv libkeys.a keys.o
	gcc -g -Wall -o client client.c libkeys.a

clean:
	rm -f server client keys.o libkeys.a
