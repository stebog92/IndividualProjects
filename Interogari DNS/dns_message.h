/*
    BOGDAN STEFAN 323CA - Aplicatie client pentru interogari DNS
*/

/* -- Query & Resource Record Type: -- */

#define	A		1	/* IPv4 address */
#define	NS		2	/* Authoritative name server */
#define	CNAME		5 	/* Canonical name for an alias */
#define	MX		15 	/* Mail exchange */
#define SOA		6	/* Start Of a zone of Authority */
#define	TXT		16	/* Text strings */



/* -- Define DNS message format -- */

/* Header section format */
/**
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
typedef struct {
	
    unsigned short msglength;

	// schimba (LITTLE/BIG ENDIAN) folosind htons/ntohs
	unsigned short id; // identification number
	
	// LITTLE -> BIG ENDIAN: inversare 'manuala' ptr byte-ul 1 din flag-uri
	unsigned char rd :1; // recursion desired
	unsigned char tc :1; // truncated message
	unsigned char aa :1; // authoritive answer
	unsigned char opcode :4; // purpose of message
	unsigned char qr :1; // query/response flag: 0=query; 1=response
	
	// LITTLE -> BIG ENDIAN: inversare 'manuala' ptr byte-ul 2 din flag-uri
	unsigned char rcode :4;
	unsigned char z :3;
	unsigned char ra :1;
	
	// schimba (LITTLE/BIG ENDIAN) folosind htons/ntohs
	unsigned short qdcount;
	unsigned short ancount;
	unsigned short nscount;
	unsigned short arcount;

} dns_header_t;

/* Question section format */
/**
                                     1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
typedef struct
{
	
	//qname variabil
	unsigned short qtype;
	unsigned short qclass;

} dns_question_t;

/* Resource record format */
/**
                                     1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
typedef struct {
	
	//name variabil
	unsigned short type;
	unsigned short class;
	unsigned short ttl1;
    unsigned short ttl2;
	unsigned short rdlength;
	//rdata variabil;

} dns_r_t;

typedef struct
{
    unsigned char *name;
    dns_r_t *resource;
    unsigned char *rdata;
} dns_resrecord_t;


typedef struct
{ 
    unsigned char *name; 
    dns_question_t *ques; 
} dns_query_t;

typedef struct
{ 
    unsigned bla; 
    dns_question_t *ques; 
} dns_message_t;