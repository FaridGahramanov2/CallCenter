CC = gcc
CFLAGS = -pthread

all: echo_server echo_client

echo_server: echo_server.c
	$(CC) $(CFLAGS) -o echo_server echo_server.c

echo_client: echo_client.c
	$(CC) -o echo_client echo_client.c

clean:
	rm -f echo_server echo_client