CFLAGS =

all: clean_all client server

client : client.o users.o orders.o menu.o login.o
	gcc -o client.out server_access.o users.o orders.o menu.o login.o

server : server.o data_access.o utils.o
	gcc -o server.out server.o data_access.o utils.o

server.o :
	gcc -c $(CFLAGS) ./src/server/server.c

data_access.o :
	gcc -c $(CFLAGS) ./src/server/data_access/data_access.c

utils.o :
	gcc -c $(CFLAGS) ./src/server/utils/utils.c	

client.o :
	gcc -c $(CFLAGS) ./src/client/server_access/server_access.c

menu.o :
	gcc -c $(CFLAGS) ./src/client/screens/menu.c

login.o :
	gcc -c $(CFLAGS) ./src/client/screens/login.c

users.o :
	gcc -c $(CFLAGS) ./src/client/screens/users.c

orders.o :
	gcc -c $(CFLAGS) ./src/client/screens/orders.c	

clean_all :
	rm -f server_access.o users.o orders.o menu.o login.o server.o server.out client.out

clean_server :
	rm -f server.o server.out data_access.o utils.o

clean_client :
	rm -f server_access.o users.o orders.o menu.o login.o client.out