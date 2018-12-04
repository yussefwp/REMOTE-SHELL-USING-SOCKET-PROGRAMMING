all: server client

server: server.c util.c
	gcc server.c util.c -o server
client: client.c util.c
	gcc client.c util.c -o client

clean:
	rm -f server 
	rm -f client

