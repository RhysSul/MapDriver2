
CC=gcc
DEBUG=-g -D_DEBUG
INCLUDE=-I. -I..
FLAGS=-Wall -Wextra

SERVER_EXE=mapserver
SERVER_C=mapserver.c
OBJ=mapserver.o


CLIENT_EXE=mapclient
CLIENT_C=mapclient.c

all: $(SERV_EXE) $(CLIENT_EXE) $(SERVER_EXE) $(CLI_G_EXE)

$(SERV_EXE): $(OBJ)
	$(CC) $(DEBUG) $(INCLUDE) $(FLAGS) $(OBJ) -o $(SERV_EXE)

$(CLIENT_EXE): $(CLI_C)
	$(CC) $(DEBUG) $(INCLUDE) $(FLAGS) $(CLI_C) -o $(CLIENT_EXE)

$(SERVER_EXE): $(SERVER_C)
	$(CC) $(DEBUG) $(INCLUDE) $(FLAGS) $(SERVER_C) -o $(SERVER_EXE)

$(CLI_G_EXE): $(CLIENT_C)
	$(CC) $(DEBUG) $(INCLUDE) $(FLAGS) $(CLIENT_C) -o $(CLI_G_EXE)

%.o: %.c
	$(CC) -c $(DEBUG) $(INCLUDE) $(FLAGS) $<

start:
	./$(SERV_EXE) &
	netstat -vatn | grep 23032

kill:
	killall ./$(SERV_EXE)

kill-client:
	kill `ps -ef | grep tmpid | grep -v grep | awk '{print $$2}'`

test:
	for i in `pgrep -f tmpid`; do kill -10 $$i; sleep 1; done;

clean:
	rm -f $(SERV_EXE) $(CLIENT_EXE)
