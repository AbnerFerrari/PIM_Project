CFLAGS =

all: clean_all client server

client : client.o cadastro_funcionario.o menu.o login.o
	gcc -o client.out client.o cadastro_funcionario.o menu.o login.o

server : server.o
	gcc -o server.out server.o

server.o :
	gcc -c $(CFLAGS) ./src/shared/server.c

client.o :
	gcc -c $(CFLAGS) ./src/shared/client.c

menu.o :
	gcc -c $(CFLAGS) ./src/menu/menu.c

login.o :
	gcc -c $(CFLAGS) ./src/login/login.c

cadastro_funcionario.o :
	gcc -c $(CFLAGS) ./src/cadastro_funcionario/cadastro_funcionario.c

clean_all :
	rm client.o cadastro_funcionario.o menu.o login.o server.o server.out client.out

clean_server :
	rm server.o server.out

clean_client :
	rm client.o cadastro_funcionario.o menu.o login.o client.out