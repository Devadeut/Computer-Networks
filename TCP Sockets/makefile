all: server client

server: doencfileserver.c
	gcc -Wall -o server doencfileserver.c

client : retriveencfileclient.c
	gcc -Wall -o client retriveencfileclient.c

run-server: server
	./server

run-client: client
	./client

clean:
	rm -f server client
