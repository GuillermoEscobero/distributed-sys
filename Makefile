all:
	gcc -g -Wall -pthread -o server server.c -lrt
	gcc -g -Wall -c keys.c -lrt
	ar -rv libkeys.a keys.o
	gcc -g -Wall -o client client.c libkeys.a -lrt
	gcc -g -Wall -c keys2.c -lrt
	ar -rv libkeys2.a keys2.o
	gcc -g -Wall -o client2 client2.c libkeys2.a -lrt

clean:
	rm -f server client client2 keys.o keys2.o libkeys.a libkeys2.a
