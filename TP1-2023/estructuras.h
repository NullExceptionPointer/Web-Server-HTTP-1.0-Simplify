#include "includes-defines.h"


/**
 * Estructura utilizada para almacenar un socket junto a su dirección destino
 */
typedef struct{
    int socket;
    struct sockaddr_in destino;
}Host;

/**
 * Estructura utilizada para organizar la petición de un cliente, haciendo la división del path "directorio" + protocolo + versión
 */
typedef struct{
    char line[MAXBUF];          //Esto es el mensaje entero del cliente.
    char metodo[10];		    //Método GET o POST.
    char recurso[1000];         //Recurso cliente . Ex: index.html ; random/image.gif...
    char protocolo[20];         //Versión del Protocolo http 1.0.
}Request;

/**
 * Estructura utilizada para guardar los returns de la función checkRequest.
 * Esta estructura tiene información sobre directorio del archivo local, código de respuesta del servidor, el código y mensaje motivo y una subestructura que comprueba el estado de los archivos. 
 */
typedef struct{
    char dir[MAXBUF];		        // Directorio local del archivo.
    struct stat statBuffer;	        // Estructura para verificar el archivo.
    int n;              			// Abrir el archivo local como interno.
    int codigo;	                    // Respuesta del servidor HTTP. Ex: 200, 404 ... 
    char msj_codigo[20];            // Mensaje respecto al código.
}CR_returns;

