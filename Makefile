
all: Server Klient UlozeneMapy

Server: server.c mapa.c mapa.h bunka.c bunka.h
	$(CC) -o Server server.c mapa.c bunka.c

Klient: klient.c klient.h mapa.c mapa.h bunka.c bunka.h
	$(CC) -pthread -o Klient klient.c mapa.c bunka.c -lncurses

UlozeneMapy:
	mkdir UlozeneMapy
	touch UlozeneMapy/UlozeneMapy.txt
	touch UlozeneMapy/ServerMapy.txt

clean:
	rm *.o -f Server Klient
	rm -r UlozeneMapy