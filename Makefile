tela : client.o tela_login.o
	gcc -o tela.out client.o tela_login.o

server : server.o
	gcc -o server.out server.o

server.o :
	gcc -c ./src/shared/server.c

client.o :
	gcc -c ./src/shared/client.c

tela_login.o :
	gcc -c ./src/tela_login/tela_login.c

clean :
	rm client.o tela_login.o server.o