#include "servicio-function.h"


/**
 * Obtiene la línea de solicitud y la organiza en una estructura de tipo Request separando método, recurso y protocolo.
 * 
 * @Parametro cliente: contiene la estructura y el socket del cliente.
 * @Parametro *f: referencia a archivo donde obtendremos los datos para las variables método, recurso y protocolo.
 * 
 * @Salida Estructura Request con sus valores correspondientes cargados.
*/
Request readRequest(Host cliente, FILE *f) {

	Request client_request;
	char line[MAXBUF];
	char *metodo = malloc(10);
	char *recurso = malloc(1000);
	char *protocolo = malloc(50);

	// Coloca los datos del socket(file) en la variable line
	fgets(line,MAXBUF,f);
	strcpy(client_request.line, line);

	sscanf(line,"%s %s %s",metodo,recurso,protocolo);
	
	strcpy(client_request.metodo, metodo);
	strcpy(client_request.recurso,recurso);
	strcpy(client_request.protocolo, protocolo);

	free(metodo);
	free(recurso);
	free(protocolo);
	
	return client_request;
}


/**
 * Analiza la petición del cliente y este arma la respuesta que se le debe devolver según las distintas condiciones que podemos tener con la petición del cliente.
 *
 * @Paramtro peticion_cliente: utilizamos los datos guardados en ella para analizar si la petición está armada correctamente.
 *  
 * @Salida: devuelve la respuesta armada que debe devolverse a la petición del cliente.
 */
CR_returns checkRequest(Request peticion_cliente) {
	
	char metodos_validos[][10] = {"GET", "POST", "PUT", "DELETE", "HEAD"};
	int cantidad_metodos = sizeof(metodos_validos) / sizeof(metodos_validos[0]);
	bool metodo_sintaxis = false;

	CR_returns respuesta;
	char pathbuf[MAXBUF]; //path del archivo
	int len; 

	strcpy(respuesta.dir,DIR_TP); //copia en dir el camino a DIR_TP

	strcat(respuesta.dir,peticion_cliente.recurso);
	printf(" [PAGE DIR] %s\n", respuesta.dir);
	printf("---------------------------------\n");

	//analizo si es un metodo valido
    for (int i = 0; i < cantidad_metodos; i++) {
        if (strcmp(metodos_validos[i], peticion_cliente.metodo) == 0) {
            metodo_sintaxis = true;
        }
    }
	//metodo no existe, error 400 bad request
	if(metodo_sintaxis == false){
		respuesta.codigo = 400; 
		strcpy(respuesta.dir,ERR400); 
		strcpy(respuesta.msj_codigo,"Bad Request"); 
		return respuesta;
	}


	//si es un metodo valido pero que no implemento, error 501 not implemented
	if(strcmp(peticion_cliente.metodo, "GET") != 0 && strcmp(peticion_cliente.metodo, "HEAD") != 0){
		respuesta.codigo = 501; 
		strcpy(respuesta.dir,ERR501); 
		strcpy(respuesta.msj_codigo,"Not Implemented"); 
		return respuesta;
	}

	//si el archivo no existe error 404 not found
	if(stat(respuesta.dir, &respuesta.statBuffer) == -1){
		respuesta.codigo = 404; 
		strcpy(respuesta.dir,ERR404); 
		strcpy(respuesta.msj_codigo,"Not Found"); 
		return respuesta;
	}

	//si el archivo es un directorio error 404 not found.
	if(S_ISDIR(respuesta.statBuffer.st_mode)) {
		respuesta.codigo = 404; 
		strcpy(respuesta.dir,ERR404); 
		strcpy(respuesta.msj_codigo,"Not Found"); 
		return respuesta;
	}

	respuesta.codigo = 200; 
	strcpy(respuesta.msj_codigo,"OK"); 
	return respuesta;
}


/**
 * El procedimiento se encarga de obtener el estado del archivo/objeto requerido en la petición, abrirlo solo para lectura, llamar a la función sendFile para que lo envíe al cliente, y por último cerrar dicho archivo.
 *
 * @Paramtro cliente: estructura del cliente donde usaremos el file descriptor para enviarle la respuesta.
 * @Paramtro respuesta: es la respuesta armada que se enviará al cliente.
 * @Paramtro metodo: método pedido por el cliente, según este la respuesta tendrá un comportamiento diferente.
 *  
 * @Salida: al tratarse de un procedimiento, no genera salida.
 */
void sendRequest(Host cliente, CR_returns respuesta, char metodo[10]){
	
	stat(respuesta.dir, &respuesta.statBuffer);
	respuesta.n = open(respuesta.dir, O_RDONLY);
	sendFile(cliente.socket,respuesta,metodo);

	close(respuesta.n);	
}


/**
 * Determina el tamaño del archivo, crea la cabecera del header de respuesta y envía el archivo al cliente.
 * 
 * @Parametro cliente_sicket:	Socket del cliente a enviar el archivo.
 * @Parametro respuesta:		Estructura donde están guardados los datos del requisito validado anteriormente que se enviara.
 * @Parametro metodo[10]:		Método de solicitud (POST, GET, HEAD, etc), se obtiene de la estructura Request.
 * 
 * @Salida: No tiene.
*/
void sendFile(int cliente_socket, CR_returns respuesta, char metodo[10]){
	char datos;
	int i;

	int tamanio = S_ISREG(respuesta.statBuffer.st_mode) ? respuesta.statBuffer.st_size : -1;

	httpHeader(cliente_socket,get_mime_type(respuesta.dir),tamanio ,respuesta.statBuffer.st_mtime, respuesta.codigo, respuesta.msj_codigo);
	
	//Enviando
	if(strcmp(metodo,"HEAD") != 0){
		while ((i = read(respuesta.n, &datos,1)))
			write(cliente_socket, &datos, 1);
	}
	
	close(cliente_socket);
}


/**
 * A Través de la información por parámetros, se encarga de armar la cabecera correcta a devolver al file descriptor del cliente cada vez que arma una línea este se la envía al cliente.
 * 
 * @Parametro cliente_socket:		el file descriptor del cliente  al cual se enviaran la cabecera.
 * @Parametro tipo_mime: 			el tipo mime del recurso que solicitó el cliente.
 * @Parametro tamanio: 				el tamaño del recurso en bytes solicitado por el cliente.
 * @Parametro ultima_actualizacion:	la fecha y hora de la última modificación al recurso solicitado.
 * @Parametro codigo_respuesta:		el código de respuesta de la petición del cliente.
 * @Parametro msj_codigo:			mensaje correspondiente al código de respuesta.
 * 
 * @Salida:	No tiene.
 */
void httpHeader(int cliente_socket,char *tipo_mime, int tamanio, time_t ultima_actualizacion, int codigo_respuesta, char *msj_codigo) {

	time_t ahora;
	ahora = time(NULL);
	struct tm *info;
	info = gmtime(&ahora);

	char bufHora[128];
	char buffer[150];

	sprintf(buffer, "%s %d %s\r\n", PROTOCOLO, codigo_respuesta, msj_codigo);	//linea de estado de cabecera
	write(cliente_socket, buffer, strlen(buffer));
	printf(" %s",buffer);

	sprintf(buffer, "Server: %s\r\n", SERVERNAME);								//server de cabecera
	write(cliente_socket, buffer, strlen(buffer));
	printf(" %s",buffer);

	info->tm_hour += -3;	//Hora de argentina
	mktime(info);	
	strftime(bufHora, sizeof(bufHora), RFC1123, info);
	sprintf(buffer, "Date: %s\r\n", bufHora);									//dia y hora actual
	write(cliente_socket, buffer, strlen(buffer));
	printf(" %s",buffer);

	sprintf(buffer, "Content-Type: %s\r\n", tipo_mime);							//tipo mime
	write(cliente_socket, buffer, strlen(buffer));
	printf(" %s",buffer);

	if (tamanio >= 0) {
		sprintf(buffer, "Content-Length: %d\r\n", tamanio);						//tamaño del recurso
		write(cliente_socket, buffer, strlen(buffer));
		printf(" %s",buffer);
	}else{
		sprintf(buffer, "\r\n");
		write(cliente_socket, buffer, strlen(buffer));
		printf(" %s",buffer);
	}

	if (ultima_actualizacion != -1){
		strftime(bufHora, sizeof(bufHora), RFC1123, gmtime(&ultima_actualizacion));
		sprintf(buffer, "Last-Modified: %s\r\n\r\n", bufHora);					//ultima actualizacion
		write(cliente_socket, buffer, strlen(buffer));
		printf(" %s",buffer);
   	}

}

/**
 * Realiza un corte de la cadena a la ruta del archivo obteniendo el tipo de dato del archivo y se compara con los tipos de datos que acepta el servidor para obtener el tipo MIME.
 * 
 * @Parametro *nombre: recibe por parámetro la ruta a la dirección del archivo.
 * 
 * @Salida: Función que devuelve una cadena de caracteres con el tipo MIME (tipos de datos de archivos).
*/
char *get_mime_type(char *nombre){
	char *ext = strrchr(nombre, '.');
	if (!ext) return NULL;	//en caso que no tenga extension.
	
	if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
	if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";

	if (strcmp(ext, ".txt") == 0) return "text/plain";
	if (strcmp(ext, ".gif") == 0) return "image/gif";
	if (strcmp(ext, ".pdf") == 0) return "application/pdf";

	//extensiones extras reconocidas
	if (strcmp(ext, ".png") == 0) return "image/png";
	if (strcmp(ext, ".css") == 0) return "text/css";
	if (strcmp(ext, ".au") == 0) return "audio/basic";
	if (strcmp(ext, ".wav") == 0) return "audio/wav";
	if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
	if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";
	if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";
	
	return "application/octet-stream";	//si no reconocio ninguna, por defecto.
}