#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"


#define MAX(a,b) (((a)>(b))?(a):(b))


int main(int argc, char *argv[])
{
	msg t;
	char *filename;
	int task_index, speed, delay, dimFereastra;

	task_index = atoi(argv[1]);
	filename = argv[2];
	speed = atoi(argv[3]);
	delay = atoi(argv[4]);
	dimFereastra = speed * delay * 1000 / (1400 * 8);
	
	printf("[SENDER] Sender starts.\n");
	printf("[SENDER] Filename=%s, task_index=%d, speed=%d, delay=%d\n", filename, task_index, speed, delay);
	
	init(HOST, PORT1);

	// TASK 0
	if (task_index == 0) {
	
		int fd = open (filename, O_RDONLY, 0666);

		// Trimitem numele fisierului
		t.len = strlen(filename);
		memcpy(t.payload, filename, strlen(filename));
		send_message(&t);

		int count, k = dimFereastra;
		msg p;

		// Implementarea efectiva a fisierului
		while ( (k > 0) && ((count = read (fd, t.payload, MSGSIZE) ) > 0)) {
			t.len = count;			
			send_message(&t);
			k--;
		}

		while ((count = read (fd, t.payload, MSGSIZE) ) > 0) {
			recv_message(&p);
			t.len = count;
			send_message(&t);
		}

		close(fd);

	}

	// TASK 1
	if (task_index == 1) {

		int fd;
		fd = open(filename, O_RDONLY, 0666);
		int count;
		int32_t ack = -1;
		int k = 0, i;
		msg p;
		msg buffer[dimFereastra];

		k = dimFereastra;
		i = 0;
		int res = 0, finished = 0, emptyFile = 0;

		// Trimitem numele fisierului
		memcpy(t.payload, filename, strlen(filename));
		t.len = strlen(filename);
		while (1) {
			send_message(&t);
			res = recv_message_timeout(&p, MAX(1, 2 * delay));

			if (res > 0) {
				break;
			}
		}

		// Implementarea efectiva a protocolului
		while ( (k > 0) && ((count = read (fd, buffer[i].payload, MSGSIZE - 4) ) > 0)) {

			ack++;
			memcpy(buffer[i].payload + count, &ack, 4);

			buffer[i].len = count+4;
			send_message(&buffer[i]);
			k--;
			i++;
		}

		dimFereastra = i;

		while (finished == 0) {

			res = recv_message_timeout(&p, MAX(1, 2 * delay));
			if (res < 0) {

				for (i = 0; i < dimFereastra; i++) {
					if (buffer[i].len == 0) 
						break;
					send_message(&buffer[i]);
				}
			}
			else {
		
				for (i = 0; i < dimFereastra - 1; i++) {
					buffer[i].len = buffer[i + 1].len;
					memcpy(buffer[i].payload, buffer[i+1].payload, buffer[i+1].len);
				}

				count = read(fd, buffer[dimFereastra -1].payload, MSGSIZE - 4);
				if (count == 0) {
					emptyFile = 1;
					dimFereastra--;
				}
				else {
					ack++;
					memcpy(buffer[dimFereastra - 1].payload + count, &ack, 4);
					buffer[dimFereastra - 1].len = count+4;
					send_message(&buffer[dimFereastra - 1]);
				}
			
			}

			if (emptyFile == 1 && dimFereastra == 0) {
				finished = 1;
			}
		
		}

		close(fd);

	}


	// TASK 2
	if (task_index == 2) {

		int fd;
		fd = open(filename, O_RDONLY, 0666);
		int count;
		int32_t ack = -1;
		int k = 0, i;
		msg p;
		msg buffer[dimFereastra];
		msg lastConfirmed;
		memcpy(lastConfirmed.payload, "", 0);
		lastConfirmed.len = 0;
		int confirmat[dimFereastra];

		for (i = 0; i < dimFereastra; i++) {
			confirmat[i] = 0;
		}

		k = dimFereastra;
		i = 0;
		int res = 0, finished = 0, emptyFile = 0, lastPoz, last;

		// Trimitem numele fisierului
		memcpy(t.payload, filename, strlen(filename));
		t.len = strlen(filename);
		while (1) {
			send_message(&t);
			res = recv_message_timeout(&p, MAX(1, 2 * delay));

			if (res > 0) {
				break;
			}
		}

		// Trimitem dimensiunea ferestrei
		memcpy(t.payload, &dimFereastra, strlen(filename));
		t.len = dimFereastra;
		while (1) {
			send_message(&t);
			res = recv_message_timeout(&p, MAX(1, 2 * delay));

			if (res > 0) {
				break;
			}
		}


		// Incarcam fereastra pentru prima data
		while ( (k > 0) && ((count = read (fd, buffer[i].payload, MSGSIZE - 4) ) > 0)) {

			ack++;
			memcpy(buffer[i].payload + count, &ack, 4);

			buffer[i].len = count+4;
			send_message(&buffer[i]);
			k--;
			i++;
		}

		// In cazul in care nu s-a umplut prima fereastra , setam cat s-a umplut din prima fereastra
		dimFereastra = i;

		while (finished == 0) {

			res = recv_message_timeout(&p, MAX(1, 2 * delay));
			if (res < 0) {

				for (i = 0; i < dimFereastra; i++) {
					if (buffer[i].len == 0) 
						break;
					if (confirmat[i] == 0) {
						send_message(&buffer[i]);
					}
				}
			}
			else {

				if ( (p.len == lastConfirmed.len) && (memcmp(lastConfirmed.payload, p.payload, p.len) == 0) ) {
					//Nu facem nimic
				}
				else {
					// Verificam ce cadru a ajuns la destinatie cu succes si a fost confirmat de catre recv
					lastPoz = -1;
					last = -1;

					for (i = 0; i < dimFereastra; i++) {
						if ( (p.len == buffer[i].len) && (memcmp(buffer[i].payload, p.payload, p.len) == 0) ) {
							memcpy(lastConfirmed.payload, buffer[i].payload, buffer[i].len);
							lastConfirmed.len = buffer[i].len;
							//confirmat[i] = 1;
							lastPoz = i;
							break;
						}
					}

					// Eliminam din buffer cadrele ce au ajuns la destinatie cu succes si au fost confirmate de recv
					for (i = lastPoz + 1; i < dimFereastra; i++) {
						last = i - lastPoz - 1;
						memcpy(buffer[last].payload, buffer[i].payload, buffer[i].len);
						buffer[last].len = buffer[i].len;
					}

					// Umplem ce a ramas gol in buffer

					for (i = last + 1; i < dimFereastra; i++) {
						count = read(fd, buffer[i].payload, MSGSIZE - 4);
						if (count == 0) {
							emptyFile = 1;
							dimFereastra = i;	
						}
						else {
							ack++;
							memcpy(buffer[i].payload + count, &ack, 4);
							buffer[i].len = count+4;
							send_message(&buffer[i]);
						}
					}
					// Am terminat de umplut
				}

				if ( (emptyFile == 1) && (dimFereastra == 0) ) {
					finished = 1;
				}

			}

		}


		close(fd);
	}

	printf("[SENDER] Job done.\n");

	return 0;
}