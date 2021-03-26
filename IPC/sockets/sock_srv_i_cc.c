#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define TAM 256

int main( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, clilen, pid;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	if ( argc < 2 ) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_STREAM, 0);

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		exit( 1 );
	}

        printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );

	listen( sockfd, 5 );
	clilen = sizeof( cli_addr );

	while( 1 ) {
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );

		pid = fork(); 

		if ( pid == 0 )
		  {  // Proceso hijo
		    close( sockfd );
		    
		    while ( 1 )
		      {
			memset( buffer, 0, TAM );
			
			n = read( newsockfd, buffer, TAM-1 );
			if ( n < 0 ) {
			  perror( "lectura de socket" );
			  exit(1);
			}
			
			printf( "PROCESO %d. ", getpid() );
			printf( "Recibí: %s", buffer );
			
			n = write( newsockfd, "Obtuve su mensaje", 18 );
			if ( n < 0 ) {
			  perror( "escritura en socket" );
			  exit( 1 );
			}
			// Verificación de si hay que terminar
			buffer[strlen(buffer)-1] = '\0';
			if( !strcmp( "fin", buffer ) ) {
			  printf( "PROCESO %d. Como recibí 'fin', termino la ejecución.\n\n", getpid() );
			  exit(0);
			}
		      }
		  }
		else {
		  printf( "SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid );
		  close( newsockfd );
		}
	}
	return 0; 
} 
