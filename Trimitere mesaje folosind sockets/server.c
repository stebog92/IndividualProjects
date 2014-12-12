/*
	BOGDAN STEFAN - 323CA 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define UNKNWN	"  Comanda necunoscuta"
#define ID	"Waiting ID ..."
#define NACK 	"Autentificarea nu a reusit. Exista deja un client cu numele '"
#define ACK 		"Autentificarea a reusit."
#define NOTONLINE	"Userul nu este online."
#define NOTHER	"Nu mai sunt alti useri online."
#define MAX_CLIENTS	100
#define STDIN 0

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, j, ind, k;
	
	fd_set read_fds;	//fd_set folosit in select()
	fd_set tmp_fds;		//fd_set folosit temporar
	int fdmax;		//nr maxim de file descriptori
	
	char *noauth;		//mesajul de autentificare gresita
	char *lista;		//lista de useri care se va trimite la apelarea comenzii 'list'
	int gasit, listare;	//variabile boolene;
	char ** nume;		//catastiful de nume;
	char ** adrese;		//catastiful de adrese;
	int porturi[100];	//vectorul de porturi ale clientilor;
	int hostingports[100];	//vectorul de porturi de hosting ale clientilor;
//	time_t t[50];	//vectorul ce contine timpii scursi de la conectarea clientilor la server
//	time_t now;			//variabila auxiliara
	char prt[256];		
	char *hostp;		//string: portul de hosting pentru un client;
	char *auxs;		//string auxiliar
	char aux1[256]; //string auxiliar;
	
	//initializarea vectorilor care retin informatiile in server;
	nume=malloc(100*sizeof(char*));			//max 100 de useri;
	adrese=malloc(100*sizeof(char*));		
	for(i=0;i<100;i++)
		porturi[i]=-1;
	for(i=0;i<100;i++)
		hostingports[i]=-1;

	if (argc < 2) {
		fprintf(stderr,"Usage : %s port\n", argv[0]);
		exit(1);
	}

	//golim read_fds
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	
	//se adauga file-descriptorul citirii de la tastatura;
	FD_SET(STDIN,&read_fds);
	
	//prompterul serverului;
	write(STDIN,"  server> ",strlen("  server> "));
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	
	portno = atoi(argv[1]);
	
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		error("ERROR on binding");
	
	listen(sockfd, MAX_CLIENTS);
	
	//se adauga noul file-descriptorul serverului in multimea read_fds;
	FD_SET(sockfd, &read_fds);
	fdmax = sockfd;
	
	for(i=0;i<=fdmax;i++){
		nume[i]=malloc(30*sizeof(char));
		adrese[i]=malloc(30*sizeof(char));
	}
     
	// main loop
	for(;;){
		tmp_fds = read_fds; 
		if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
			error("ERROR in select");
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockfd) {
					// o noua conexiune
					clilen = sizeof(cli_addr);
					if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
						error("ERROR in accept");
					} else {
						//se adauga in lista de socketi, cu toate ca se asteapta nume pentru a se face autentificarea;
						FD_SET(newsockfd, &read_fds);
						if (newsockfd > fdmax) { 
							for(i=fdmax+1;i<=newsockfd;i++){
								nume[i]=malloc(30*sizeof(char));
								adrese[i]=malloc(30*sizeof(char));
							}
							fdmax = newsockfd;
						}
						//se retin adresa, portul si portul de hosting ale clientului;
						strcpy(adrese[newsockfd],inet_ntoa(cli_addr.sin_addr));
					}

				} else {
					//daca se citeste de la tastatura;
					if(i==STDIN){
						memset(buffer, 0, 256);
						fflush(stdin);
						read(STDIN, buffer, sizeof(buffer));
						write(STDIN,"  server> ",strlen("  server> "));
						//cerere lista de useri conectati: lista se construieste ca un string;
						if(strcmp(buffer,"status\n")==0){
							lista=malloc(40*sizeof(char*)*sizeof(char));
							for(j=0;j<=fdmax;j++)
								if(strcmp(nume[j],"")>0){
									strcat(lista,"\t"); 
									strcat(lista,nume[j]);
									strcat(lista,"\t"); 
									strcat(lista,adrese[j]);
									strcat(lista,"\t"); 
									memset(prt,0,256);
									sprintf(prt,"%d",hostingports[j]);
									strcat(lista,prt);
									strcat(lista,"\n");
								}
							if(strcmp(lista,"")>0)
								printf("\n%s",lista);
							else printf("Nu e nici un user online.\n");
							write(STDIN,"  server> ",strlen("  server> "));
						}
						//serverul se isi anunta clientii ca se va inchide si se inchide
						else if(strcmp(buffer,"quit\n")==0){
							for(k=1;k<=fdmax;k++){
								if(strcmp(nume[k],"")>0){							
									n = send(k, "finish\n", sizeof("finish\n"), 0);
									if (n < 0)
										error("ERROR in send");
									close(k);
								}
							}
							close(sockfd);
							exit(0);
						}
						else if(strspn(buffer,"kick")==4){
							memset(auxs, 0, 256);
							strncpy(auxs,buffer+5,strlen(buffer+5)-1);
							for(k=1;k<=fdmax;k++){
								if(strcmp(nume[k],auxs)==0){
									n = send(k, "finish\n", sizeof("finish\n"), 0);
									if (n < 0)
										error("ERROR in send");
								}
							}
						}
						else {
							//orice altceva scris in consola serverului reprezinta o comanda necunoscuta;
							printf("%s\n",UNKNWN);
							write(STDIN,"  server> ",strlen("  server> "));
						}
					}
 					else{
						memset(buffer, 0, 256);
						// am primit date
						if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
							if (n == 0) {
								//conexiunea s-a inchis
								printf("del user: %s\n", nume[i]);
								write(STDIN,"  server> ",strlen("  server> "));
							} else {
								error("ERROR in recv");
							}
							close(i); 
							FD_CLR(i, &read_fds); // il scoatem din set
							free(nume[i]);
							nume[i]=malloc(30*sizeof(char));
							free(adrese[i]);
							adrese[i]=malloc(30*sizeof(char));
							porturi[i]=-1;
							hostingports[i]=-1;
						} else {
							//trimiterea portului pe care asculta clientul
							if(strspn(buffer,"[PORT]")==6){
								memset(auxs, 0, 256);
								strcat(auxs, buffer+6);
							//	printf("auxs este %s\n", auxs);
								hostingports[fdmax] = atoi(auxs);
							//	printf("fdmax=%d si host=%d\n", fdmax, hostingports[fdmax]);
								n = send(i, ACK, strlen(ACK), 0);
								if (n < 0)
									error("ERROR in send");
								//write(STDIN,"  server> ",strlen("  server> "));	
							}
							//trimiterea numelui: autentificare = verificare daca mai exista un user cu acelasi nume;
							else if(strspn(buffer,"[AUTH]")==6){
								gasit=0;
								//se cauta in vectorul de nume pentru vedea daca mai exista un user cu acelasi nume;
								for(j=0;j<=fdmax;j++)
									if(strcmp(nume[j],buffer+6)==0) gasit =1;
								if(gasit==1){
									noauth=malloc(256*sizeof(char));
									strcpy(noauth, NACK);
									strcat(noauth,buffer+6);
									strcat(noauth,"'\n");
									n = send(i, noauth, strlen(noauth), 0); 
									if (n < 0)
										error("ERROR in send");						
									free(adrese[i]);
									adrese[i]=malloc(30*sizeof(char));
									porturi[i]=-1;
									close(i); 
									FD_CLR(i, &read_fds); // il scoatem din set
								//daca nu mai este un altul, se retine numele si se afiseaza informatiile;
								} else {
									strcpy(nume[i],buffer+6);
									printf("add user: %s %s %i\n",nume[i],inet_ntoa(cli_addr.sin_addr),hostingports[i]);
									n = send(i, ACK, strlen(ACK), 0); 
									if (n < 0)
										error("ERROR in send");
									write(STDIN,"  server> ",strlen("  server> "));	
								}
							}
							//un client care vrea sa trimita un mesaj clientului 'nume', are nevoie de adresa si portul
							//de hosting al acestuia, informatii pe care le primeste de la server;
							else if(strspn(buffer,"message ")==8){
								gasit=0;ind=-1;
								auxs=malloc((strlen(buffer+8)-1)*sizeof(char));
								strncpy(auxs,buffer+8,strlen(buffer+8)-1);
								//se cauta clientul 'nume';
								for(j=0;j<=fdmax;j++)
									if(strcmp(nume[j],auxs)==0){ 
										gasit =1;
										ind=j;
									}
								//daca nu s-a gasit inseamna ca nu e online;
								if(gasit==0){
										memset(buffer, 0, 256);
										strcpy(buffer,NOTONLINE);
										n=send(i,buffer,strlen(buffer),0);
										if(n<0)
											error("ERROR in send");
								}
								//s-a gasit userul cautat, este online, si se vor trimite informatiile necesare;
								else{
										n=send(i,adrese[ind],strlen(adrese[ind]),0);
										if(n<0)
											error("ERROR in send");
										memset(buffer, 0, 256);
										n=recv(i,buffer,strlen(buffer),0);
										if(n<0)
											error("ERROR in reading");
										memset(prt,0,256);
										sprintf(prt,"%d",hostingports[ind]);
										n=send(i,prt,strlen(prt),0);
										if(n<0)
											error("ERROR in send");
								}
							} else {	
								listare=0;
								//clientul cere lista de useri online: lista se creeaza ca un string;
								if(strcmp(buffer,"listclients\n")==0){
									lista=malloc(40*sizeof(char*)*sizeof(char));
									for(j=0;j<=fdmax;j++)
										if(strcmp(nume[j],"")>0 && i!=j){
											strcat(lista,"    "); 
											strcat(lista,nume[j]);
											strcat(lista,"\n");
										}
									//daca mai exista alti useri online, se trimite lista,
									if(strlen(lista)>0)
										n = send(i, lista, strlen(lista), 0);
									//altfel se trimite mesaj;
									else n = send(i, NOTHER, strlen(NOTHER), 0);
									if (n < 0)
										error("ERROR in send");
								}
								//clientul a instiintat serverul ca s-a deconectat;
								else if(strcmp(buffer,"quit\n")==0){
								//	n = send(i, lista, strlen(lista), 0); 
									close(i);
									FD_CLR(i, &read_fds); 	// il scoatem din set
									printf("del user: %s\n", nume[i]);
									write(STDIN,"  server> ",strlen("  server> "));
									free(nume[i]);
									nume[i]=malloc(30*sizeof(char));
									free(adrese[i]);
									adrese[i]=malloc(30*sizeof(char));
									porturi[i]=-1;
									hostingports[i]=-1;
								}

									else if (strspn(buffer,"info ") == 5) {
										gasit=0;ind=-1;
										auxs=malloc((strlen(buffer+5)-1)*sizeof(char));
										strncpy(auxs,buffer+5,strlen(buffer+5)-1);
										//se cauta clientul 'nume';
										for(j=0;j<=fdmax;j++)
											if(strcmp(nume[j],auxs)==0){ 
												gasit =1;
												ind=j;
											}
										//daca nu s-a gasit inseamna ca nu e online
										if(gasit==0){
											memset(buffer, 0, 256);
											strcpy(buffer,NOTONLINE);
											n=send(i,buffer,strlen(buffer),0);
											if(n<0)
												error("ERROR in send");
										}
										//s-a gasit userul cautat, este online, si se vor trimite informatiile necesare;
										else{
											n=send(i,adrese[ind],strlen(adrese[ind]),0);
											if(n<0)
												error("ERROR in send");
											memset(buffer, 0, 256);
											n=recv(i,buffer,strlen(buffer),0);
											if(n<0)
												error("ERROR in reading");
											memset(prt,0,256);
											sprintf(prt,"%d",hostingports[ind]);
											printf("%d\n", hostingports[ind]);
											n=send(i,prt,strlen(prt),0);
											if(n<0)
												error("ERROR in send");
											//time(&now);
											//printf("%.f seconds \n",difftime(now, t[ind]));
										}
									}
							}
						}
					}
				} 
			}
		}
     	}

	close(sockfd);
	
	return 0; 
}