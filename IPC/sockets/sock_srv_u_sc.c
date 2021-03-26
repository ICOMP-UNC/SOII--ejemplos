#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define TAM 100

int main( int argc, char *argv[] ) {
  int socket_server, resultado;
  struct sockaddr_un struct_servidor;
  socklen_t tamano_direccion;
  char buffer[ TAM ];
  
  /* Se toma el nombre del socket de la línea de comandos */
  if( argc < 2 ) {
    printf( "Uso: %s <nombre_de_socket>\n", argv[0] );
    exit( 1 );
  }
  
  /* Creacion de socket */
  if(( socket_server = socket(AF_UNIX, SOCK_DGRAM, 0) ) < 0 ) {
    perror("socket" );
    exit(1);
  }
  
  /* Remover el nombre de archivo si existe */
  unlink ( argv[1] );
  
  /* Inicialización y establecimiento de la estructura del servidor */
  memset( &struct_servidor, 0, sizeof( struct_servidor ) );
  struct_servidor.sun_family = AF_UNIX;
  strncpy( struct_servidor.sun_path, argv[1], sizeof( struct_servidor.sun_path ) );
  
  /* Union del socket de servidor a una dirección */
  if( ( bind( socket_server, (struct sockaddr *)&struct_servidor, SUN_LEN(&struct_servidor ))) < 0 ) {
    perror( "bind" );
    exit(1);
  }
  
  printf( "Socket disponible: %s\n", struct_servidor.sun_path );
  
  tamano_direccion = sizeof( struct_servidor );
  /* Mantenimiento de un lazo infinito, aceptando conexiones */
  while( 1 ) {
    resultado = recvfrom ( socket_server, (void *)buffer, TAM, 0, (struct sockaddr *) &struct_servidor, &tamano_direccion );
    printf( "Recibí: %s\n", buffer );
  }
  return 0;
}
