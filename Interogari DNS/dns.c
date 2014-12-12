/*
	BOGDAN STEFAN 323CA - Aplicatie client pentru interogari DNS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include "dns_message.h"


void error(char *msg)
{
    perror(msg);
    exit(0);
}

//functia care converteste o adresa la formatul DNS;
char * ChangetoDnsNameFormat(unsigned char * qname, unsigned char * host){
	unsigned char a=0, i=0, x=0;
	unsigned char * aux;
	aux=malloc(2*sizeof(host));
	while(i<strlen(host)){
		if(host[i]=='.'){
			aux[x]=i-a;
			x=i+1;
			i++;
			a=i;
		}
		else{
			aux[i+1]=host[i];
			i++;
		}
	}
	aux[x]=i-a;
	memcpy(qname,aux,i+1);
}

//functia care extrage o adresa din formatul ei DNS;
unsigned char * extrageNume(unsigned char ** raspuns, unsigned char * buffer, int *indice){
	unsigned char q=**raspuns;
	unsigned char * name;
	int p=-1, k, j, ref=0;;

	name=malloc(40*sizeof(unsigned char));

	j=*indice;
	while(q!=0){
		if(q>=192){
			j=buffer[j+1]+2;
			q=buffer[j];
			ref=1;
		}
		else{
			for(k=1;k<=q;k++){
				name[++p]=buffer[j+k];
			}
			name[++p]='.';
			j+=k;
			q=buffer[j];
			if(ref==0){
				*indice=*indice+k;
				*raspuns=*raspuns+k;
			}
		}
	}
	if(ref==1){
		*indice=*indice+2;
		*raspuns=*raspuns+2;
	}
	name[p]='\0';
	return name;
}






int main(int argc, char *argv[])
{
	int sockfd, portno, n, i, j, indice, type, ok;
	unsigned char *qname, *raspuns, *dnsformat;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	int soa[4];		//vectorul care va contine numerele corespunzatoare tipului SOA;

	time_t t1,t2;

	dns_resrecord_t answers[20],auth[20],addit[20];	//RR'urile

	unsigned char buffer[65530];	//mesajul
	unsigned short lung, lung2;
	unsigned char mesaj[65532];


	//citirea serverelor de DNS din fisier
	FILE *file;
	file = fopen("dns_servers.conf", "r");
	char servere[20][256];
	if (file == NULL) {
		printf("\nNu exista fisier cu servere DNS\n");
		exit(1);
	}
	else {
		i = -1;
		while(fgets(servere[++i], 256, file) != NULL){	
			if (servere[i][0] == '#' || servere[i][0] == ' ' || strcmp(servere[i], "\n") == 0) {
				i--;
			}		
		}
	}

	fclose(file);
		

	if (argc < 3) {
	fprintf(stderr,"Usage %s hostname option\n", argv[0]);
	exit(0);
	}

	//tipurile de inregistrari	
	char tipuri[17][6];
	strcpy(tipuri[1],"A");	
	strcpy(tipuri[2],"NS");	
	strcpy(tipuri[3],"MD");
	strcpy(tipuri[4],"MF");
	strcpy(tipuri[5],"CNAME");
	strcpy(tipuri[6],"SOA");
	strcpy(tipuri[7],"MB");
	strcpy(tipuri[8],"MG");	
	strcpy(tipuri[9],"MR");
	strcpy(tipuri[10],"NULL");	
	strcpy(tipuri[11],"WKS");	
	strcpy(tipuri[12],"PTR");
	strcpy(tipuri[13],"HINFO");
	strcpy(tipuri[14],"MINFO");
	strcpy(tipuri[15],"MX");
	strcpy(tipuri[16],"TXT");		

	//se cauta tipul introdus in vectorul de tipuri;
	type=0;
	for(i=1;i<=16;i++)
		if(strcmp(argv[2],tipuri[i])==0) type=i;
	if(type==0){ 
		printf("\nOptiune incorecta.\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	
	//conectarea la server; se iau toate serverele la rand, pana este posibila conectarea; in cazul in care nici o adresa nu este buna
	// se afiseaza mesaj de instiintare si se iese din program;
	ok = 0;
	for (j = 0; j < i; j++) {
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(53);
		serv_addr.sin_addr.s_addr = inet_addr(servere[j]);
		if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) >= 0) {
			ok=1;
			break;
		}
	}

	if (ok == 0) {
		printf("\nFisierul 'dns_servers.conf' nu contine servere valide.\n");
		exit(0);	
	}	

	
	//convertirea numelui domeniului la formatul DNS;
	dnsformat=malloc(strlen(argv[1]));
	ChangetoDnsNameFormat(dnsformat,argv[1]);

	lung=sizeof(dns_header_t)+strlen(dnsformat)+sizeof(dns_question_t)-1;

	dns_header_t * dns=NULL;		//headerul mesajului
	dns_question_t * qinfo=NULL;		//query

	dns=(dns_header_t *)& buffer;

	//SETAREA HEADERului
	dns->msglength=htons(lung);
	dns->id=(unsigned short)htons(getpid());			//identificatorul procesului

	dns->rd=1;							//query recursiv
	dns->tc=0;							//mesajul nu a fost truncat
	dns->aa=0;			
	dns->opcode=0;						//standard query
	dns->qr=0;							//mesajul e un query


	dns->rcode=0;						//fara conditii de eroare
	dns->z=0;							//zero
	dns->ra=0;							//fara recursivitate


	dns->qdcount=htons(1);					//o singura intrebare
	dns->ancount=0;							//numarul de RR la raspunsuri
	dns->nscount=0;							//numarul de RR la autoritate
	dns->arcount=0;							//numarul de RR la aditionale

	//numele domeniului din query;
	qname=(unsigned char*)& buffer[sizeof(dns_header_t)];
	strcpy(qname,dnsformat);
	qinfo =(dns_question_t*)& buffer[sizeof(dns_header_t) + strlen(qname)+1];

	//tipul si clasa;
	qinfo->qtype = htons(type);
	qinfo->qclass = htons(1);

	FILE *outFile;
	outFile = fopen("logfile", "a+");
	//se trimite mesajul;
	n = send(sockfd,buffer,lung+2, 0);
	if(n>0){
		fprintf(outFile, "\n\tIp: %s", servere[j]);
		fprintf(outFile, "\n\tQUERY-ul trimis:");
		fprintf(outFile, "\n\t%s\t\t%s\t%s",argv[1],(qinfo->qclass==htons(1))?"IN":" ",argv[2]);
	}
	else{ 
		printf("\n\tConectare esuata.\n");
		exit(0);
	}



	//se primeste raspuns de la server;
	memset(buffer, 0, 256);
   	n = recv(sockfd, buffer, 65530, 0);
	lung=n-2;

	
	
	dns=(dns_header_t*)buffer;
	//afisari generale;
	fprintf(outFile, "\n\t");
	fprintf(outFile, "\n\tRaspunsul contine : ");
	fprintf(outFile, "\n\t %d intrebar%s",ntohs(dns->qdcount),((ntohs(dns->qdcount)==1)?"e.":"i."));
	fprintf(outFile, "\n\t %d raspuns%s",ntohs(dns->ancount),((ntohs(dns->ancount)==1)?".":"uri."));
	fprintf(outFile, "\n\t %d server%sautoritar%s",ntohs(dns->nscount),((ntohs(dns->nscount)==1)?" ":"e "),((ntohs(dns->nscount)==1)?".":"e."));
	fprintf(outFile, "\n\t %d inregistrar%saditional%s\n\n",ntohs(dns->arcount),((ntohs(dns->arcount)==1)?"e ":"i "),((ntohs(dns->arcount)==1)?"a.":"e."));

	
	indice=sizeof(dns_header_t) + strlen(qname) + 1 + sizeof(dns_question_t);
	raspuns=&buffer[indice];

	//citirea + afisarea raspunsurilor
	if(ntohs(dns->ancount)>0){
		
		for(i=0;i<ntohs(dns->ancount);i++){
			answers[i].name=extrageNume(&raspuns,buffer,&indice);
			answers[i].resource=(dns_r_t*)raspuns; 
			
			raspuns+=sizeof(dns_r_t);
			indice+=sizeof(dns_r_t);
	
			int p,q,k,ca,ref;
			
	
		//tipul NS:
			if(type==2){
				answers[i].rdata=(unsigned char*)malloc((2*ntohs(answers[i].resource->rdlength)-1)*sizeof(unsigned char));
				p=-1;  n=*raspuns;  q=*raspuns;  j=indice;
				ca=0,  ref=0;
				while(q!=0){
					if(q>=192){
						j=buffer[j+1]+2;
						q=buffer[j];
						n-=2;
					}
					else{
						for(k=1;k<=q;k++){
							answers[i].rdata[++p]=buffer[j+k];
							n--;
						}
						answers[i].rdata[++p]='.';
						j+=k;
						q=buffer[j];
					}
				}
				//afisarea rezultatului;
				fprintf(outFile, "\n\t%s\t%i\t%s\t%s\t%s",answers[i].name,ntohs(answers[i].resource->ttl1)*256+ntohs(answers[i].resource->ttl2),(qinfo->qclass==htons(1))?"IN":" ",tipuri[ntohs(answers[i].resource->type)],answers[i].rdata);
				raspuns+=ntohs(answers[i].resource->rdlength);
				indice+=ntohs(answers[i].resource->rdlength);
			}
	
	
		//tipul SOA:
			else if(type==6){
				answers[i].rdata=(unsigned char*)malloc((2*ntohs(answers[i].resource->rdlength)-1)*sizeof(unsigned char));
				p=0;
				n=*raspuns; q=*raspuns; j=indice;
				ca=0, ref=0;
				//prima adresa;
				while(n>0 || q!=0){
					if(q>=192){
						j=buffer[j+1]+2;
						q=buffer[j];
						n-=2; 
						ca+=2;
						ref=1;
					}
					else{	if(ref==0) ca+=q;
						for(k=1;k<=q;k++){
							answers[i].rdata[++p]=buffer[j+k];
							n--;
						}
						answers[i].rdata[++p]='.';
						j+=k;
						q=buffer[j];
					}
				}
				ca++;
				answers[i].rdata[++p]=' ';
				//cea de-a doua adresa;
				n=*(raspuns+ca);q=*(raspuns+ca);j=indice+ca;
				ref=0;
				while(n>0 || q!=0){
					if(q>=192){
						j=buffer[j+1]+2;
						q=buffer[j];
						n-=2; 
						ca+=2;
						ref=1;
					}
					else{	if(ref==0) ca+=q;
						for(k=1;k<=q;k++){
							answers[i].rdata[++p]=buffer[j+k];
							n--;
						}
						answers[i].rdata[++p]='.';
						j+=k;
						q=buffer[j];
					}
				}
				ca++;
				//se retin cele 5 numere;
				j=ca-1;
				for(k=0;k<5;k++)
					for(q=0;q<4;q++){
						soa[k]*=256;
						soa[k]+=*(raspuns+(++j));
					}
				//afisarea rezultatului;
				fprintf(outFile, "\n\t%s\t%i\t%s\t%s\t%s",answers[i].name,ntohs(answers[i].resource->ttl1)*256+ntohs(answers[i].resource->ttl2),
					(qinfo->qclass==htons(1))?"IN":" ",tipuri[ntohs(answers[i].resource->type)],answers[i].rdata);
				for(j=0;j<5;j++)
					fprintf(outFile, "%i ",soa[j]);
				raspuns+=ntohs(answers[i].resource->rdlength);
				indice+=ntohs(answers[i].resource->rdlength);
			}
	
	
		//tipul MX:
			else if(type==15){
				answers[i].rdata=(unsigned char*)malloc((2*ntohs(answers[i].resource->rdlength)-1)*sizeof(unsigned char));
				p=0;
				answers[i].rdata[p]=*raspuns * 256+*(raspuns+1);
				n=*raspuns-2; q=*(raspuns+2); j=indice+2;
		
				while(n>0 || q!=0){
					if(q>=192){
						j=buffer[j+1]+2;
						q=buffer[j];
						n-=2;
					}
					else{
						for(k=1;k<=q;k++){
							answers[i].rdata[++p]=buffer[j+k];
							n--;
						}
						answers[i].rdata[++p]='.';
						j+=k;
						q=buffer[j];
					}
				}
				//afisarea rezultatului;
				fprintf(outFile, "\n\t%s\t%i\t%s\t%s\t%i %s",answers[i].name,ntohs(answers[i].resource->ttl1)*256+ntohs(answers[i].resource->ttl2),
					(qinfo->qclass==htons(1))?"IN":" ",tipuri[ntohs(answers[i].resource->type)],*answers[i].rdata,answers[i].rdata+1);
				raspuns+=ntohs(answers[i].resource->rdlength);
				indice+=ntohs(answers[i].resource->rdlength);
			}
			
		//tipul TXT:
			else if(type==16){
				answers[i].rdata=(unsigned char*)malloc((2*ntohs(answers[i].resource->rdlength)-1)*sizeof(unsigned char));
				p=-1;
				n=*raspuns; q=*raspuns; j=indice;
				while(n>0 ){
					if(q>=192){
						j=buffer[j+1]+2;
						q=buffer[j];
						n-=2;
					}
					else{
						for(k=1;k<=q;k++){
							answers[i].rdata[++p]=buffer[j+k];
							n--;
						}
						j+=k;
						q=buffer[j];
					}
				}
				//afisarea rezultatului;
				fprintf(outFile, "\n\t%s\t%i\t%s\t%s\t'%s'",answers[i].name,ntohs(answers[i].resource->ttl1)*256+ntohs(answers[i].resource->ttl2),
					(qinfo->qclass==htons(1))?"IN":" ",tipuri[ntohs(answers[i].resource->type)],answers[i].rdata);
				raspuns+=ntohs(answers[i].resource->rdlength);
				indice+=ntohs(answers[i].resource->rdlength);
			}
	
		//pentru celelalte tipuri:
			else{
				answers[i].rdata=(unsigned char*)malloc((2*ntohs(answers[i].resource->rdlength)-1)*sizeof(unsigned char));
				for(j=0;j<ntohs(answers[i].resource->rdlength);j++) 
					answers[i].rdata[2*j]=*(raspuns+j);
				for(j=1;j<2*ntohs(answers[i].resource->rdlength)-1;j+=2)
					answers[i].rdata[j]='.';
				//afisarea rezultatului
				fprintf(outFile, "\n\t%s\t%i\t%s\t%s\t",answers[i].name,ntohs(answers[i].resource->ttl1)*256+ntohs(answers[i].resource->ttl2),
					(qinfo->qclass==htons(1))?"IN":" ",tipuri[ntohs(answers[i].resource->type)]);
				for(j=0;j<2*ntohs(answers[i].resource->rdlength)-1;j++)
					if(j%2==0) fprintf(outFile, "%i",answers[i].rdata[j]);
					else fprintf(outFile, "%c",answers[i].rdata[j]);
				raspuns+=ntohs(answers[i].resource->rdlength);
				indice+=ntohs(answers[i].resource->rdlength);
			}
		}
	}
	else fprintf(outFile, "");

	
	//citirea autoritatilor
	if(ntohs(dns->nscount)>0){
		fprintf(outFile, "\n\n\n\tInformatiile de autoritate:");
		for(i=0;i<ntohs(dns->nscount);i++){
			auth[i].name=extrageNume(&raspuns,buffer,&indice);fprintf(outFile, "\n\t[%i] %s ",i,auth[i].name);
			auth[i].resource=(dns_r_t*)raspuns; 
			raspuns+=sizeof(dns_r_t);
			indice+=sizeof(dns_r_t);
			auth[i].rdata=(unsigned char*)malloc((ntohs(auth[i].resource->rdlength))*sizeof(unsigned char));
			strcpy(auth[i].rdata,extrageNume(&raspuns,buffer,&indice)); fprintf(outFile, "\t --> %s",auth[i].rdata);
		}
	}
	else fprintf(outFile, "\n\n\tNu s-a primit informatii despre autoritati.");

	//citirea adaugirilor
	if(ntohs(dns->arcount)>0){
		fprintf(outFile, "\n\n\tInformatiile aditionale:");
		for(i=0;i<ntohs(dns->arcount);i++){
			addit[i].name=extrageNume(&raspuns,buffer,&indice);
			fprintf(outFile, "\n\t[%i] %s \t --> ",i,answers[i].name);
			addit[i].resource=(dns_r_t*)raspuns; 
			raspuns+=sizeof(dns_r_t);
			indice+=sizeof(dns_r_t);
			addit[i].rdata=(unsigned char*)malloc((2*ntohs(addit[i].resource->rdlength)-1)*sizeof(unsigned char));
			for(j=0;j<2*ntohs(addit[i].resource->rdlength)-1;j++)
				if(j%2==0) addit[i].rdata[2*j]=*(raspuns+j);
				else addit[i].rdata[j]='.';
			for(j=0;j<2*ntohs(addit[i].resource->rdlength)-1;j++)
				if(j%2==0) fprintf(outFile, "%i",addit[i].rdata[j]);
				else fprintf(outFile, "%c",addit[i].rdata[j]);
			raspuns+=ntohs(addit[i].resource->rdlength);
			indice+=ntohs(addit[i].resource->rdlength);
		}
	}
	else fprintf(outFile, "\n\n\tNu s-au primit informatii aditionale.");
	

	fprintf(outFile, "\n\n\tMesajul este de %i de octeti.",lung);
	fprintf(outFile, "\n\n");


	fclose(outFile);
	return 0;
}


