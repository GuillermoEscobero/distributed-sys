all:
	gcc -g -Wall -pthread -o server server.c -lrt
	gcc -g -Wall -c keys.c -lrt
	ar -rv libkeys.a keys.o
	gcc -g -Wall -o client client.c libkeys.a -lrt

clean:
	rm -f server client keys.o libkeys.a
