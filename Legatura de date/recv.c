#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

int main(int argc, char *argv[])
{
	msg r, t;
	int task_index;
	char *filename;

	task_index = atoi(argv[1]);	
	printf("[RECEIVER] Receiver starts.\n");
	printf("[RECEIVER] Task index=%d\n", task_index);
		
	init(HOST, PORT2);


	// TASK 0
	if (task_index == 0) {

		// Primim numele fisierului
		recv_message(&r);
		filename = (char *) calloc (5 + r.len, sizeof(char));
		sprintf(filename, "recv_%s", r.payload);
		int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		free(filename);

		int ackN = 0;	

		// Implementarea efectiva a fisierului
		while (recv_message(&r) > 0) {
			write(fd, r.payload, r.len);
			sprintf(t.payload, "ACK(%d)", ackN++);
			t.len = strlen(t.payload + 1);
			send_message(&t);
		}

		close(fd);
	}

	// TASK 1
	if (task_index == 1) {

		// Primim numele fisierului
		recv_message(&r);
		filename = (char *) calloc (5 + r.len, sizeof(char));
		sprintf(filename, "recv_%s", r.payload);

		int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		free(filename);

		// Confirmarea primirii numelui
		sprintf(t.payload, "Nume fisier primit");
		t.len = strlen(t.payload + 1);
		send_message(&t);


		int ackAsteptat = 0;
		int n;
		char ackPrimit[4];
		char ackAst[4];

		// Implementarea efectiva a protocolului;
		while (recv_message(&r) > 0) {

			memcpy(ackAst, &ackAsteptat, 4);
			memcpy(ackPrimit, r.payload + (r.len - 4), 4);
	
			n = memcmp(ackAst, ackPrimit, 4);
	
			if (n == 0) {
				write(fd, r.payload, r.len - 4);
				sprintf(t.payload, "ACK E BUN");
				t.len = strlen(t.payload + 1);
				send_message(&t);
				ackAsteptat++;
			}

		}	

		close(fd);
	}

	if (task_index == 2) {

		// Primim numele fisierului
		recv_message(&r);
		filename = (char *) calloc (5 + r.len, sizeof(char));
		sprintf(filename, "recv_%s", r.payload);

		int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		free(filename);

		// Confirmarea primirii numelui
		sprintf(t.payload, "Nume fisier primit");
		t.len = strlen(t.payload + 1);
		send_message(&t);

		// Primim dimensiunea ferestrei
		recv_message(&r);
		int dimFereastra = r.len;

		// Confirmarea primirii dimensiunii ferestrei
		sprintf(t.payload, "Dimensiune fereastra primita");
		t.len = strlen(t.payload + 1);
		send_message(&t);

		msg buffer[dimFereastra];
		msg lastMessage;
		memcpy(lastMessage.payload, "", 0);
		lastMessage.len = 0;

		int confirmat[dimFereastra];
		int ackAsteptat = 0;
		int n, c, lastPoz, last, i;
		char ackPrimit[4];
		char ackAst[4];
		int32_t aux;

		for (i = 0; i < dimFereastra; i++) {
			confirmat[i] = 0;
		}

		while (recv_message(&r) > 0) {

			memcpy(ackAst, &ackAsteptat, 4);
			memcpy(ackPrimit, r.payload + (r.len - 4), 4);
	
			n = memcmp(ackAst, ackPrimit, 4);

			if (n == 0) {
				memcpy(buffer[0].payload, r.payload, r.len);
				buffer[0].len = r.len;
				confirmat[0] = 1;
			}
			else {
				for (i = ackAsteptat + 1; i < ackAsteptat + dimFereastra; i++) {
					aux=i;
					memcpy(ackAst, &aux, 4);
					c = memcmp(ackAst, ackPrimit, 4);

					if (c == 0) {
						memcpy(buffer[i - ackAsteptat].payload, r.payload, r.len);
						buffer[i - ackAsteptat].len = r.len;
						confirmat[i - ackAsteptat] = 1;
						break;
					}
				}
			}

			lastPoz = -1;
			for (i = 0; i < dimFereastra; i++) {
				if (confirmat[i] == 1) {
					lastPoz = i;
					write(fd, buffer[i].payload, buffer[i].len - 4);
				}
				else {
					break;
				}
			}


			if (lastPoz >= 0) {

				ackAsteptat = ackAsteptat + lastPoz + 1;

				memcpy(lastMessage.payload, buffer[lastPoz].payload, buffer[lastPoz].len);
				lastMessage.len = buffer[lastPoz].len;

				send_message(&buffer[lastPoz]);

				if (lastPoz + 1 == dimFereastra) {
					for (i = 0; i < dimFereastra; i++) {
						confirmat[i] = 0;
					}
				}

				for (i = lastPoz + 1; i < dimFereastra; i++) {
					last = i - lastPoz - 1;
					memcpy(buffer[last].payload, buffer[i].payload, buffer[i].len);
					buffer[last].len = buffer[i].len;
					confirmat[last] = confirmat[i];
				}
				for (i = last + 1; i < dimFereastra; i++) {
					confirmat[i] = 0;
				}

			}
			else {
				send_message(&lastMessage);
			}

		}		

		close(fd);
	}

	return 0;
}