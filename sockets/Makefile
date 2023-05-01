
CC=gcc
DEBUG=-g -D_DEBUG
INCLUDE=-I. -I..
FLAGS=-Wall -Wextra


SERVER_EXE=mapserver
SERVER_C=mapserver.c

CLIENT_EXE=mapclient
CLIENT_C=mapclient.c


all: $(SERVER_EXE) $(CLIENT_EXE)


$(SERVER_EXE): $(SERVER_C)
	$(CC) $(DEBUG) $(INCLUDE) $(FLAGS) $(SERVER_C) -o $(SERVER_EXE)

$(CLIENT_EXE): $(CLIENT_C)
	$(CC) $(DEBUG) $(INCLUDE) $(FLAGS) $(CLIENT_C) -o $(CLIENT_EXE)