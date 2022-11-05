CFLAGS =

all: clean tela server

tela : client.o cadastro_funcionario.o menu.o login.o
	gcc -o tela.out client.o cadastro_funcionario.o menu.o login.o

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

clean :
	rm client.o cadastro_funcionario.o menu.o login.o server.o server.out tela.out