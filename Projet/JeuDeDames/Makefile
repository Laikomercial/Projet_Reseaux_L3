SRC = src/
CC = gcc $(SRC)
COMP = compile/

all : client.o dame.o serveur.o
	./compile/serveur

client.o : $(SRC)client.c
	$(CC)client.c -o $(COMP)client

dame.o : $(SRC)dame.c
	$(CC)dame.c -o $(COMP)dame

serveur.o : $(SRC)serveur.c
	$(CC)serveur.c -o $(COMP)serveur -lpthread


clean : 
	rm -rf $(COMP)*.o

#zip : 
#	zip -r src.zip *c src