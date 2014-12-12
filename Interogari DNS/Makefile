CC=gcc
LIBSOCKET=-lnsl
DNS=dns
CLIENT=my_dns_client

$(DNS):	$(DNS).c
	$(CC) -o $(CLIENT) $(LIBSOCKET) $(DNS).c

clean:
	rm $(CLIENT)