CPP=gcc
CFLAGS:=-Wall -Werror -Wextra -g -c -std=c11
TARGET_FOLDER=../build/
OBJS_SRV=$(addprefix $(TARGET_FOLDER),server.o clients.o queue.o)
OBJS_CLT=$(addprefix $(TARGET_FOLDER),client.o)

all: build_client build_server

build_server: server.o clients.o queue.o
	$(CPP) $(OBJS_SRV) -o $(TARGET_FOLDER)Server
	$(MAKE) clean_objs

build_client: client.o
	$(CPP) $(OBJS_CLT) -o $(TARGET_FOLDER)Client
	$(MAKE) clean_objs

server.o: server/server.c
	$(CPP) $(CFLAGS) $< -o $(TARGET_FOLDER)$@

clients.o: server/clients.c
	$(CPP) $(CFLAGS) $< -o $(TARGET_FOLDER)$@

queue.o: queue/queue.c
	$(CPP) $(CFLAGS) $< -o $(TARGET_FOLDER)$@

client.o: client/client.c
	$(CPP) $(CFLAGS) $< -o $(TARGET_FOLDER)$@

clean_objs:
	rm -rf $(OBJS_CLT) $(OBJS_SRV)

clean: clean_objs
	rm -rf $(TARGET_FOLDER)*