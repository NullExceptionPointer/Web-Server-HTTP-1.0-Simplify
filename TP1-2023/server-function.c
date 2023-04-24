#include "server-function.h"


/**
 * Función que se encarga de crear y configurar un socket. 
 * 
 * @Parametro *servidor: la estructura que guardará el servidor creado y configurado.
 * @Parametro puerto: el número de puerto donde el socket estará escuchando.
 * 
 * @Salida: devuelve 1 si hubo un error al setear el socket, retorna 3 si el error ocurrió al nombrarlo, un 4 si se trata de un error con el listen del socket, y finalmente un 0 si termina exitosamente.
 */
int setServer(Host *servidor, int puerto){
	Host aux;
	bool yes = true;

	if (setSocket(&aux.socket) == 1)
		return 1;

	setsockopt(aux.socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	memset(&aux.destino, 0, sizeof(aux.destino));
	aux.destino.sin_family = AF_INET;
	aux.destino.sin_addr.s_addr = htonl(INADDR_ANY);
	aux.destino.sin_port = htons(puerto);

	if (setBind(&aux) == 1)
		return 3;

	if (setListen(aux) == 1)
		return 4;

	*servidor = aux;

	return 0;
}


/**
 * Crea un socket de la familio TCP y lo guarda en lo apuntado por sock, controla los errores devolviendo 1 si fallo la creación.
 * 
 * @parametro *sock: hace referencia a donde se guardará el file descriptor de la función socket.
 * 
 * @Salida: devuelve 0 si se creó el socket exitosamente sino 1.
*/
int setSocket(int *sock){
	*sock = socket(AF_INET, SOCK_STREAM, 0);

	if ( &sock < 0 )
		return 1;

	return 0;
}


/**
 * Esta función se encarga de nombrar a un socket asignándole un puerto y dirección IP.
 * 
 * @Parametro *host: socket que se le asigna IP y PUERTO..
 * 
 * @Salida: si logra realizar el bind() retorna 0, si no retorna 1.
 */
int setBind(Host *host){
	Host aux = *host;

	if ( bind(aux.socket, (struct sockaddr*)&aux.destino, sizeof(aux.destino)) != 0 )
		return 1;

	*host = aux;
	return 0;
}


/**
 * Asigna a un socket la cantidad de clientes pendientes que podrá encolar para atender.
 * 
 * @Parametro host: socket que se le asigna la cantidad de clientes pendientes.
 * 
 * @Salida: devuelve 0 si listen() se ejecutó exitosamente sino 1.
 */
int setListen(Host host){
	if ( listen(host.socket, PENDIENTES) != 0 )
		return 1;

	return 0;
}


/**
 * La función se encarga de esperar una petición de un cliente y establecer la conexión entre el cliente y el servidor. 
 * 
 * @Parametro *servidor: estructura que guarda información del servidor, el cual escuchará peticiones.
 * @Parametro *cliente: estructura que guarda información del cliente, guardará el file descriptor del cliente.
 * 
 * @Salida: Si el cliente no puede conectarse al devuelve false, si no devuelve true.
 */
bool initAccept(Host *servidor, Host *cliente){

	Host servidorAux = *servidor;
	Host clienteAux = *cliente;

	int addrlen = sizeof(servidorAux.destino);
	memset(&clienteAux.destino, 0, sizeof(clienteAux.destino));

	/* Espera conexion*/
	clienteAux.socket  = accept(servidorAux.socket,(struct sockaddr *)&clienteAux.destino, &addrlen);
	if(clienteAux.socket < 0)
		return false;

	printf("\n [CONNECTION ACCEPTED]");
	fflush(stdout);
	*servidor = servidorAux;
	*cliente = clienteAux;

	return true;
}


/**
 * Imprime el error indicando el nombre del servidor y el mensaje. Se indica que hubo un estado de salida fallido (la función terminó con alguna falla). 
 * 
 * @Parametro *msj: Mensaje de error.
 * 
 * @Salida no tiene.
 */
void salidaError(char *msj){
	fprintf(stderr, "%s: %s\r\n", SERVERNAME, msj);
	exit(EXIT_FAILURE);
}


/**
 * Atiende la señal SIGCHLD para completar la destrucción del proceso hijo y envía un mensaje por consola del servidor y se muestra su PID.
 * 
 * @Parametros no tiene.
 * 
 * @Salida no tiene.
*/
void sig_chld(){
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("\n\n ******Murio Hijo [ %d ]*****\n ",pid);
}