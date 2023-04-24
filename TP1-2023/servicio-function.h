#include "includes-defines.h"
#include "estructuras.h"


/**
 * Obtiene la línea de solicitud y la organiza en una estructura de tipo Request separando método, recurso y protocolo.
 * 
 * @Parametro cliente: contiene la estructura y el socket del cliente.
 * @Parametro *f: referencia a archivo donde obtendremos los datos para las variables método, recurso y protocolo.
 * 
 * @Salida Estructura Request con sus valores correspondientes cargados.
*/
Request readRequest(Host cliente, FILE *f);


/**
 * Analiza la petición del cliente y este arma la respuesta que se le debe devolver según las distintas condiciones que podemos tener con la petición del cliente.
 *
 * @Paramtro peticion_cliente: utilizamos los datos guardados en ella para analizar si la petición está armada correctamente.
 *  
 * @Salida: devuelve la respuesta armada que debe devolverse a la petición del cliente.
 */
CR_returns checkRequest(Request peticion_cliente);


/**
 * El procedimiento se encarga de obtener el estado del archivo/objeto requerido en la petición, abrirlo solo para lectura, llamar a la función sendFile para que lo envíe al cliente, y por último cerrar dicho archivo.
 *
 * @Paramtro cliente: estructura del cliente donde usaremos el file descriptor para enviarle la respuesta.
 * @Paramtro respuesta: es la respuesta armada que se enviará al cliente.
 * @Paramtro metodo: método pedido por el cliente, según este la respuesta tendrá un comportamiento diferente.
 *  
 * @Salida: al tratarse de un procedimiento, no genera salida.
 */
void sendRequest(Host cliente, CR_returns respuesta, char metodo[10]);


/**
 * Determina el tamaño del archivo, crea la cabecera del header de respuesta y envía el archivo al cliente.
 * 
 * @Parametro cliente_sicket:	Socket del cliente a enviar el archivo.
 * @Parametro respuesta:		Estructura donde están guardados los datos del requisito validado anteriormente que se enviara.
 * @Parametro metodo[10]:		Método de solicitud (POST, GET, HEAD, etc), se obtiene de la estructura Request.
 * 
 * @Salida: No tiene.
*/
void sendFile(int cliente_socket, CR_returns respuesta, char metodo[10]);


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
void httpHeader(int cliente_socket,char *tipo_mime, int tamanio, time_t ultima_actualizacion, int codigo_respuesta, char *msj_codigo);


/**
 * Realiza un corte de la cadena a la ruta del archivo obteniendo el tipo de dato del archivo y se compara con los tipos de datos que acepta el servidor para obtener el tipo MIME.
 * 
 * @Parametro *nombre: recibe por parámetro la ruta a la dirección del archivo.
 * 
 * @Salida: Función que devuelve una cadena de caracteres con el tipo MIME (tipos de datos de archivos).
*/
char *get_mime_type(char *nombre);