#include "server-function.h"

/**
 * Se encarga de pedir un socket TCP creado y configurado y luego atender los clientes del mismo socket.
 * Cuando atienda un cliente este crea un hijo y ejecuta con excev() el servicio que atendera al mismo, tambien 
 * se encarga del evitar procesos zombies.
 * 
 * @Parametros no tiene
 * 
 * @Salida no tiene ya que se encuentra en un bucle infinito.
*/
int main(){
    Host servidor,cliente;          
    int id_fork = 0;    
    char *args[3], *str = NULL;
    
    str = malloc(sizeof(char) * 16);
    args[0] = "servicio";
    args[2] = NULL;

    printf("\n INICIANDO SERVER %s..",SERVERNAME);

    if( setServer(&servidor,PUERTO) > 0)    
        salidaError(" No pudo crearse el socket del servidor.");
    
    printf("\n ******************************");
    printf("\n Inicio Exitoso\n");
    printf("\n Servidor escuchando en puerto [%d]",PUERTO);
    printf("\n ******************************\n\n");

    signal(SIGCHLD, sig_chld);

    while(1){

        if( initAccept(&servidor,&cliente) == false)
            salidaError("No fue posible aceptar la peticion del cliente");
        else {
            if( (id_fork=fork())  == -1 )
                salidaError("Ocurrio un fallo en el proceso hijo");
            
            
            if(id_fork == 0){   //PROCESO HIJO
                close(servidor.socket);

                sprintf(str, "%d", cliente.socket);
                args[1] = str;
                execv("servicio", args);
                
                close(cliente.socket);
                exit(0);
            }

        }
        //PROCESO PADRE
        printf("\n Hijo[%d] atendiendo al cliente [%s] en puerto[%d]", id_fork,inet_ntoa(cliente.destino.sin_addr),ntohs(cliente.destino.sin_port) );
        close(cliente.socket);
    }

}

