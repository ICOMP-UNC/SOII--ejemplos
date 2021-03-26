#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define TAM 256

int main( int argc, char *argv[] ) {
	int sockfd, puerto, tamano_direccion;
	char buffer[ TAM ];
	struct sockaddr_in serv_addr;
	int n;

	if ( argc < 2 ) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfd < 0) { 
		perror("ERROR en apertura de socket");
		exit( 1 );
	}

	memset( &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );
	memset( &(serv_addr.sin_zero), '\0', 8 );

	if( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0 ) {
		perror( "ERROR en binding" );
		exit( 1 );
	}

        printf( "Socket disponible: %d\n", ntohs(serv_addr.sin_port) );

	tamano_direccion = sizeof( struct sockaddr );
	while ( 1 ) {
		memset( buffer, 0, TAM );
		n = recvfrom( sockfd, buffer, TAM-1, 0, (struct sockaddr *)&serv_addr, &tamano_direccion );

		printf( "Recibí: %s", buffer );

		n = sendto( sockfd, (void *)"Obtuve su mensaje", 18, 0, (struct sockaddr *)&serv_addr, tamano_direccion  );
	}
	return 0; 
} 
