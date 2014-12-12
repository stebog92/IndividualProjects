/*
	BOGDAN STEFAN - 323CA 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define ACK	"ok"
#define NACK	"Autentificarea nu a reusit."
#define NOTONLINE	"Userul nu este online."
#define IND	"\n  Introduceti comanda: listclients / read / message / quit (atentie: case sensitive!)"
#define UNKNWN	"Comanda necunoscuta"
#define NOMSG	"   Nici un mesaj primit."
#define ALTMSG	"   Nu exista un asemenea mesaj."
#define MAX_CLIENTS 100
#define STDIN 0

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr, peer_addr;
	struct hostent *server, *peer;
	
	char buffer[256], adresa[256], port[256], mesaj[256];

	int outsockfd, newsockfd, clilen, univ;
	struct sockaddr_in this_addr, cli_addr;
	int i, j, m, gasit=0;
	
	fd_set read_fds;	//fd_set folosit in select()
	fd_set tmp_fds;		//fd_set folosit temporar
	int fdmax;		//nr maxim de file descriptori
	int hostp;		//portul de hosting al clientului;
	char *prompt;		//promptul clientului;

	char peername[100][30];	//useri conectati la client;
	char mesages[100][30];  //mesaje primite
	int nrmesages = 0;      //numarul de mesaje primite
	
	char auxs[256];		//string auxiliar;
	char aux2[256];     //string auxiliar;
	char aux3[256];     //string auxiliar;
	char aux4[256];		//string auxiliar;
	char aux5[256];		//string auxiliar;
	char *auxstrtok;


	if (argc < 5) {
	fprintf(stderr,"Usage %s name hostname port\n", argv[0]);
	exit(0);
	}	
	
	portno = atoi(argv[4]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(argv[3]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	//clientul se conecteaza la server;
	if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	hostp=atoi(argv[2]);	

	//se trimite portul pe care asculta clientul;
	memset(aux2, 0, 256);
	strcat(aux2,"[PORT]");
	strcat(aux2,argv[2]);
	n = send(sockfd, aux2, strlen(aux2), 0);
	if (n < 0)
		error("ERROR sending to socket");
	//astept mesaj de confirmare a portului
	memset(buffer, 0, 256);
	n = recv(sockfd,buffer,255,0);
	if (n < 0) 
		error("ERROR reading from socket)");

	//se trimite numele pentru autentificare;
	memset(buffer, 0, 256);
	strcpy(buffer,"[AUTH]");	
	strcat(buffer,argv[1]);
	n = send(sockfd,buffer,strlen(buffer), 0);
    	if (n < 0) 
        	 error("ERROR sending to socket");

    	//astept msj de autentificare
    	memset(buffer, 0, 256);
    	n = recv(sockfd,buffer,255, 0);
    	if (n < 0) 
        	 error("ERROR reading from socket");
	//se construieste promptul;
	prompt=malloc(40*sizeof(char));
	strcpy(prompt,"\n  ");
	strcat(prompt,argv[1]);
	strcat(prompt,"> ");
    	write(STDIN,prompt,strlen(prompt));
	//afisarea msjului de autentificare;
	write(STDIN,buffer,strlen(buffer));
    	write(STDIN,IND,strlen(IND));
	write(STDIN,prompt,strlen(prompt));
		
	//daca nu s-a facut autentificarea, se deconecteaza de la server;
	if(strspn(buffer,NACK)==strlen(NACK)+1){
		close(sockfd);
		exit(0);
	}
    	else while(1){

		//golim read_fds
		FD_ZERO(&read_fds);
		FD_ZERO(&tmp_fds);
		
		//se adauga file-descriptorii asociati citirii de la tastatura, respectiv serverului;
		FD_SET(STDIN,&read_fds);
		FD_SET(sockfd,&read_fds);
	
		//clientul devine la randul sau server;
		outsockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (outsockfd < 0) 
			error("ERROR opening socket");
		
		memset((char *) &this_addr, 0, sizeof(this_addr));
		this_addr.sin_family = AF_INET;
		this_addr.sin_addr.s_addr = INADDR_ANY;
		this_addr.sin_port = htons(hostp);

		if (bind(outsockfd, (struct sockaddr *) &this_addr, sizeof(struct sockaddr)) < 0) 
			error("ERROR on binding");
		
		listen(outsockfd, MAX_CLIENTS);
	
		//adaugam noul file-descriptorul de hosting in multimea read_fds;
		FD_SET(outsockfd, &read_fds);
		fdmax = outsockfd;

		
		// main loop
		for(;;){
			tmp_fds = read_fds;
			if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
				error("ERROR in select");
			for(i = 0; i <= fdmax; i++) {
				if (FD_ISSET(i, &tmp_fds)) {
					if (i == outsockfd) {
						// o noua conexiune
						clilen = sizeof(cli_addr);
						if ((newsockfd = accept(outsockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
							error("ERROR in accept");
						} else {
							FD_SET(newsockfd, &read_fds);
							if (newsockfd > fdmax) 
								fdmax = newsockfd;
						}
					} else {
						//se citeste de la tastatura;
						if(i==STDIN){
							memset(buffer, 0, 256);
							fflush(stdin);
							read(STDIN, buffer, sizeof(buffer));
							if(strcmp(buffer,"listclients\n")==0){
								n=send(sockfd,buffer,sizeof(buffer),0);
								if (n < 0)
									error("ERROR writing to socket");
								//astept raspuns
								memset(buffer, 0, 256);
								n = recv(sockfd,buffer,255, 0);
								if (n < 0) 
									error("ERROR reading from socket");
								write(STDIN,buffer,strlen(buffer));
								write(STDIN,IND,strlen(IND));
								write(STDIN,prompt,strlen(prompt));
							}
							//clientul vrea sa trimita un mesaj;
							else if(strspn(buffer,"message ")==8){
								memset(auxs,0,256);
								strncpy(auxs,buffer+8,strlen(buffer+8)-1);
								if(strcmp(auxs,argv[1])!=0){
									gasit=0;
									j=0;
									//se cauta in vectorul de clienti, pentru a se evita o conexiune in plus;
									while(j<100 && gasit==0)
										if(strcmp(peername[j],auxs)==0 && FD_ISSET(j, &read_fds)){
											gasit=1;
											newsockfd=j;memset(buffer,0,256);
										}
										else j++;
									//in cazul in exista conexiune, se creeaza o noua conexiune intre clienti;
									if(gasit==0){
										n=send(sockfd,buffer,sizeof(buffer),0);
										if (n < 0)
											error("ERROR writing to socket");
										//serverul ii va trimite adresa ...
										memset(adresa, 0, 256);
										n = recv(sockfd,adresa,255, 0);
										if (n < 0)
											error("ERROR reading from socket");
										//sau in cazul in care userul nu este online, un mesaj de atentionare,...
										if(strcmp(adresa,NOTONLINE)==0){
											write(STDIN,prompt,strlen(prompt));
											write(STDIN,NOTONLINE,strlen(NOTONLINE));
											
										}
										else{
											n = send(sockfd,adresa,255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
											// ... iaif (FD_ISSET(i, &tmp_fds)) {r in cazul in care userul este online, si portul celui caruia 
											// ii va trimite mesajul;
											memset(port, 0, 256);
											n = recv(sockfd,port,255, 0);
											if (n < 0) 
												error("ERROR reading from socket");
											
											//dupa ce a primit informatiile necesare, clientul se 
											// conecteaza la celalalt client;
											newsockfd = socket(AF_INET, SOCK_STREAM, 0);
											FD_SET(newsockfd, &read_fds);
											if (newsockfd > fdmax) 
												fdmax = newsockfd;
											if (newsockfd < 0) 
												error("ERROR opening socket");
											//se adauga numele in vectorul de nume;
											memset(peername[newsockfd],0,256);
											strcpy(peername[newsockfd],auxs);
											peer = gethostbyname(adresa);
											if (peer == NULL) {
												fprintf(stderr,"ERROR, no such host\n");
												exit(0);
											}
											
											memset((char *) &peer_addr, 0, sizeof(peer_addr));
											peer_addr.sin_family = AF_INET;
											memcpy((char *)&peer_addr.sin_addr.s_addr, (char *)peer->h_addr, peer->h_length);
											peer_addr.sin_port = htons(atoi(port));
											
											if (connect(newsockfd,(struct sockaddr*) &peer_addr,sizeof(peer_addr)) < 0) 
												error("ERROR connecting");
											memset(buffer, 0, 256);
											strcpy(buffer,"[NUME]");
											n = send(newsockfd, buffer, 255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
											memset(buffer, 0, 256);
											n = recv(newsockfd, buffer, 255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
											memset(buffer, 0, 256);
											strcat(buffer,argv[1]);
											n = send(newsockfd, buffer, 255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
										}
									}
									if(strcmp(adresa,NOTONLINE)!=0){
										//dupa ce s-a conectat, trimite mesajul;
										memset(buffer, 0, 256);
										memset(mesaj, 0, 256);
										read(STDIN, mesaj, sizeof(mesaj));
										strcpy(buffer,mesaj);
										memset(mesaj, 0, 256);
										n = send(newsockfd, buffer, sizeof(buffer), 0);
										if (n < 0)
											error("ERROR writing on socket");
										write(STDIN,IND,strlen(IND));
										write(STDIN,prompt,strlen(prompt));
									}
									else{
										write(STDIN,IND,strlen(IND));
										write(STDIN,prompt,strlen(prompt));
									}
								}
							}
							//daca se doreste afisarea history-ului
							else if (strcmp(buffer,"history\n")==0){
								for(j=0;j<nrmesages;j++){
									write(STDIN,mesages[j],strlen(mesages[j]));
								}
								write(STDIN,IND,strlen(IND));
								write(STDIN,prompt,strlen(prompt));
							}
							//daca se doreste trimiterea unui mesaj tuturor clientilor online
							else if(strcmp(buffer,"broadcast\n")==0){
								memset(mesaj, 0, 256);
								read(STDIN, mesaj, sizeof(mesaj));
								n=send(sockfd,"listclients\n",sizeof("listclients\n"),0);
								if (n < 0)
									error("ERROR writing to socket");
								//astept raspuns
								memset(buffer, 0, 256);
								n = recv(sockfd,buffer,255, 0);
								if (n < 0) 
									error("ERROR reading from socket");
								auxstrtok = strtok(buffer," \n");
								while (auxstrtok != NULL) {
									printf("%s\n", auxstrtok);
									memset(aux5, 0, 256);
									strcpy(aux5, auxstrtok);
									memset(buffer, 0, 256);
									sprintf(buffer,"message %s\n",aux5);
									gasit=0;
									j=0;
									//se cauta in vectorul de clienti, pentru a se evita o conexiune in plus;
									while(j<100 && gasit==0)
										if(strcmp(peername[j],auxstrtok)==0 && FD_ISSET(j, &read_fds)){
											gasit=1;
											newsockfd=j;memset(buffer,0,256);
										}
										else j++;
									if(gasit==0){
										n=send(sockfd,buffer,sizeof(buffer),0);
										if (n < 0)
											error("ERROR writing to socket");
										//serverul ii va trimite adresa ...
										memset(adresa, 0, 256);
										n = recv(sockfd,adresa,255, 0);
										if (n < 0)
											error("ERROR reading from socket");
										//sau in cazul in care userul nu este online, un mesaj de atentionare,...
										if(strcmp(adresa,NOTONLINE)==0){
											write(STDIN,prompt,strlen(prompt));
											write(STDIN,NOTONLINE,strlen(NOTONLINE));
											
										}
										else{
											n = send(sockfd,adresa,255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
											memset(port, 0, 256);
											n = recv(sockfd,port,255, 0);
											if (n < 0) 
												error("ERROR reading from socket");					
											//dupa ce a primit informatiile necesare, clientul se 
											// conecteaza la celalalt client;
											newsockfd = socket(AF_INET, SOCK_STREAM, 0);
											FD_SET(newsockfd, &read_fds);
											if (newsockfd > fdmax) 
												fdmax = newsockfd;
											if (newsockfd < 0) 
												error("ERROR opening socket");
											//se adauga numele in vectorul de nume;
											memset(peername[newsockfd],0,256);
											strcpy(peername[newsockfd],auxstrtok);
											peer = gethostbyname(adresa);
											if (peer == NULL) {
												fprintf(stderr,"ERROR, no such host\n");
												exit(0);
											}
											
											memset((char *) &peer_addr, 0, sizeof(peer_addr));
											peer_addr.sin_family = AF_INET;
											memcpy((char *)&peer_addr.sin_addr.s_addr, (char *)peer->h_addr, peer->h_length);
											peer_addr.sin_port = htons(atoi(port));
											
											if (connect(newsockfd,(struct sockaddr*) &peer_addr,sizeof(peer_addr)) < 0) 
												error("ERROR connecting");
											memset(buffer, 0, 256);
											strcpy(buffer,"[NUME]");
											n = send(newsockfd, buffer, 255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
											memset(buffer, 0, 256);
											n = recv(newsockfd, buffer, 255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
											memset(buffer, 0, 256);
											strcat(buffer,argv[1]);
											n = send(newsockfd, buffer, 255, 0);
											if (n < 0) 
												error("ERROR writing from socket");
										}
									}

									if(strcmp(adresa,NOTONLINE)!=0){
										memset(buffer, 0, 256);
										strcpy(buffer,mesaj);
										memset(mesaj, 0, 256);
										n = send(newsockfd, buffer, sizeof(buffer), 0);
										if (n < 0)
											error("ERROR writing on socket");
										write(STDIN,IND,strlen(IND));
										write(STDIN,prompt,strlen(prompt));
									}
									else{
										write(STDIN,IND,strlen(IND));
										write(STDIN,prompt,strlen(prompt));
									}


									auxstrtok = strtok(NULL, "\n");
			
								}
								write(STDIN,IND,strlen(IND));
								write(STDIN,prompt,strlen(prompt));
							}
							//daca se doreste aflarea de informatii despre un anumit client
							else if(strspn(buffer,"info ")==5) {
								memset(auxs,0,256);
								memset(aux2,0,256);
								strncpy(auxs,buffer+5,strlen(buffer+5)-1);
								if (strcmp(auxs,argv[1])!=0) {
									n=send(sockfd,buffer,sizeof(buffer),0);
									if (n < 0)
										error("ERROR writing to socket");
									memset(adresa, 0, 256);
									n = recv(sockfd,adresa,255, 0);
									if (n < 0)
										error("ERROR reading from socket");
									//sau in cazul in care userul nu este online, un mesaj de atentionare,...
									if(strcmp(adresa,NOTONLINE)==0){
										write(STDIN,prompt,strlen(prompt));
										write(STDIN,NOTONLINE,strlen(NOTONLINE));							
									}
									else{
										n = send(sockfd,adresa,255, 0);
										if (n < 0) 
											error("ERROR writing from socket");
											memset(port, 0, 256);
											n = recv(sockfd,port,255, 0);
											if (n < 0) 
												error("ERROR reading from socket");
											sprintf(aux2,"\t%s\t%s\t%s",auxs, adresa, port);
											write(STDIN,aux2,strlen(aux2));
									}
									write(STDIN,IND,strlen(IND));
									write(STDIN,prompt,strlen(prompt));
								}
							}
							//daca se doreste deconectarea;
							else if(strcmp(buffer,"quit\n")==0){
								//se anunta toti userii conectati;
								for(j=outsockfd+1;j<100;j++)
									if (FD_ISSET(j, &read_fds)) {
										send(j,buffer,strlen(buffer),0);
										close(j);
										FD_CLR(j, &read_fds);
								}
								close(outsockfd);
								send(sockfd,buffer,strlen(buffer),0);
								close(sockfd);
								memset((char *) &serv_addr, 0, sizeof(serv_addr));
								memset((char *) &this_addr, 0, sizeof(this_addr));
								FD_ZERO(&read_fds);
								FD_ZERO(&tmp_fds);
								free(prompt);
								exit(0);
								
							}
							//orice altceva scris in consola e doar o comanda necunoscuta;
							else{
								write(STDIN,prompt,strlen(prompt));
								write(STDIN, UNKNWN, strlen(UNKNWN));
								write(STDIN,IND,strlen(IND));
								write(STDIN,prompt,strlen(prompt));
							}
						}
						//s-au primit informatii;
						else{
							memset(buffer, 0, 256);
							if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
								if (n == 0) {
									//conexiunea s-a inchis
									printf("Conexiune inchisa");
								} else {
									error("ERROR in recv");
								}
								close(i);
								FD_CLR(i, &read_fds);
							}else{
								//se primeste numele userului care trimite mesaj;
								if(strcmp(buffer,"[NUME]")==0){
									n = send(i,ACK,sizeof(ACK), 0);
									if (n < 0)
										error("ERROR writing from socket");
									memset(auxs, 0, 256);
									n = recv(newsockfd, auxs, 255, 0);
 									if (n < 0) 
 										error("ERROR writing from socket");
									//se retine numele userului;
									strcpy(peername[i],auxs);
								}
								else if(strcmp(buffer,"quit\n")==0){
									close(i);
									FD_CLR(i, &read_fds);
								}
								else if(strcmp(buffer,"finish\n")==0){
									printf("S-a terminat conexiunea\n");
									close(outsockfd);
									close(sockfd);
									memset((char *) &serv_addr, 0, sizeof(serv_addr));
									memset((char *) &this_addr, 0, sizeof(this_addr));
									FD_ZERO(&read_fds);
									FD_ZERO(&tmp_fds);
									free(prompt);
									exit(0);
								}
								//se primeste mesajul care se va retine; de asemenea se si afiseaza mesajul
								else{
									sprintf(aux4,"[Timp][%s]",peername[i]);
									strcat(aux4, buffer);
									strcpy(mesages[nrmesages++], aux4);
									write(STDIN,aux4,strlen(aux4));
									write(STDIN,IND,strlen(IND));
									write(STDIN,prompt,strlen(prompt));
								}
							}
						}
					}
				}
			}
		}
	}

    return 0;
}


