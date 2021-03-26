#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#define TAM 80

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, servlen, clilen, n, buf, pid;
  struct sockaddr_un  cli_addr, serv_addr;
  char buffer[TAM];
  
  /* Se toma el nombre del socket de la línea de comandos */
  if( argc != 2 ) {
    printf( "Uso: %s <nombre_de_socket>\n", argv[0] );
    exit( 1 );
  }
  
  if ( ( sockfd = socket( AF_UNIX, SOCK_STREAM, 0) ) < 0 ) {
    perror( "creación de  socket");
    exit(1);
  }
  
  /* Remover el nombre de archivo si existe */
  unlink ( argv[1] );
  
  memset( &serv_addr, 0, sizeof(serv_addr) );
  serv_addr.sun_family = AF_UNIX;
  strcpy( serv_addr.sun_path, argv[1] );
  servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
  
  if( bind( sockfd,(struct sockaddr *)&serv_addr,servlen )<0 ) {
    perror( "ligadura" ); 
    exit(1);
  }
  
  printf( "Proceso: %d - socket disponible: %s\n", getpid(), serv_addr.sun_path );
  
  listen( sockfd, 5 );
  clilen = sizeof( cli_addr );
  
  while ( 1 )
    {
      newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
      
      
      pid = fork();
      
      if ( pid == 0 )
	{        // proceso hijo
	  close( sockfd );
	  
	  while(1) { 
	    memset( buffer, 0, TAM );
	    
	    n = read( newsockfd, buffer, TAM-1 );
	    
	    printf( "PROCESO: %d. ", getpid() );
	    printf( "Recibí: %s", buffer );
	    
	    n = write( newsockfd, "Obtuve su mensaje", 18 );
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
