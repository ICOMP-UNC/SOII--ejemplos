#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define TAM 100

int main( int argc, char *argv[] ) {
	int descriptor_socket, resultado, cantidad, i;
	struct sockaddr_un struct_cliente;
	socklen_t tamano_direccion;
	char buffer[ TAM ];

	/* Se toma el nombre del socket de la línea de comandos */
	if( argc != 2 ) {
		printf( "Uso: %s <nombre_de_socket>\n", argv[0] );
		exit( 1 );
	}

	/* Creacion de socket */
	if(( descriptor_socket = socket(AF_UNIX, SOCK_DGRAM, 0) ) < 0 ) 
		perror("socket" );

	/* Inicialización y establecimiento de la estructura del cliente */
	memset( &struct_cliente, 0, sizeof( struct_cliente ) );
	struct_cliente.sun_family = AF_UNIX;
	strncpy( struct_cliente.sun_path, argv[1], sizeof( struct_cliente.sun_path ) );

	/* Lectura de mensaje a enviar, se reemplaza el LineFeed por 0 ASCII */
	printf( "Mensaje a enviar: " );
	fgets( buffer, sizeof(buffer), stdin);
	for( i=0; i<TAM; i++ ) {
         	if( buffer[i] == 10 ) buffer[i] = 0;
	}
	cantidad = strlen( buffer );

	/* Envío de datagrama al servidor */
	resultado = sendto( descriptor_socket, buffer, TAM, 0, (struct sockaddr *)&struct_cliente, sizeof(struct_cliente) );
	if( resultado < 0 ) {
 		perror( "sendto" );
		exit( 1 );
	}

	printf( "Enviados %d bytes\n", cantidad );
	return 0;
}
