#include "servicio-function.h"


/**
 * La funcion se encarga de tratar la peticion del cliente. 
 * Recibe la peticion del cliente, la analiza. 
 * Luego arma la respuesta del servidor e intenta enviarla.
 * 
 * @Parametro argc: cantidad de argumentos pasados por parametros.
 * @Parametro argv: lista de parametros como cadenas de esta obtenemos el file descriptor del clienhte.
 * 
 * @Salida: la funcion retorna 0 al finalizar. 
 */
int main(int argc, char *argv[]){
    Host cliente;
    cliente.socket = atoi(argv[1]);

    FILE *f = fdopen(cliente.socket,"r+");
    Request cliente_peticion = readRequest(cliente,f);

    printf("\n***********************");
    printf("\n [Cliente_Peticion.linea]     %s",    cliente_peticion.line);
    printf("\n [Cliente_Peticion.metodo]    %s", cliente_peticion.metodo);
    printf("\n [Cliente_Peticion.recurso]   %s", cliente_peticion.recurso);
    printf("\n [Cliente_Peticion.protocolo] %s",    cliente_peticion.protocolo);   
    printf("\n***********************\n\n");


    //IDENTIFICA LA RESPUESTA SI ESTA ARMADA
    CR_returns returns = checkRequest(cliente_peticion);

    printf(" [Respuesta.directorio]   %s \n", returns.dir);
    printf(" ******CABECERA RESPUESTA*****************\n");

    
    //INTENTO ENVIAR RESPUESTA
    sendRequest(cliente, returns,cliente_peticion.metodo);

    fclose(f);
    return 0;
}