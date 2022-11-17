CFLAGS =

all: clean_all client server

client : client.o cadastro_funcionario.o menu.o login.o
	gcc -o client.out client.o cadastro_funcionario.o menu.o login.o

server : server.o data_access.o utils.o
	gcc -o server.out server.o data_access.o utils.o

server.o :
	gcc -c $(CFLAGS) ./src/server/server.c

data_access.o :
	gcc -c $(CFLAGS) ./src/server/data_access/data_access.c

utils.o :
	gcc -c $(CFLAGS) ./src/server/utils/utils.c	

client.o :
	gcc -c $(CFLAGS) ./src/client/server_access/client.c

menu.o :
	gcc -c $(CFLAGS) ./src/client/screens/menu.c

login.o :
	gcc -c $(CFLAGS) ./src/client/screens/login.c

cadastro_funcionario.o :
	gcc -c $(CFLAGS) ./src/client/screens/cadastro_funcionario.c

clean_all :
	rm -f client.o cadastro_funcionario.o menu.o login.o server.o server.out client.out

clean_server :
	rm -f server.o server.out data_access.o utils.o

clean_client :
	rm -f client.o cadastro_funcionario.o menu.o login.o client.out