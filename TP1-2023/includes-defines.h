/*LIBRERIAS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>   /* Biblioteca para utilizar la struct stat para verificacion de archivos*/
#include <fcntl.h>      /* Biblioteca para utilizar la funcion open*/
#include <sys/socket.h> /* Biblioteca que contienen las definiciones de sockets*/
#include <sys/wait.h>   /* Biblioteca para utillizar la funcion waitpid() */
#include <sys/types.h>  /* Biblioteca que contienen los tipos de socket */
#include <arpa/inet.h>  /* Biblioteca que contienen funciones referentes a inet (Red) */
#include <unistd.h>     /* Biblioteca que contiene varios comandos *NIX */
#include <stdbool.h>    /* Para usar booleanos*/
#include <signal.h>     /* Biblioteca para utilizar funciones de signal.*/

/*DEFINICINES*/
/*Parametros utilizados por el programa*/
#define SERVERNAME  "WebServer"	                    /* Nombre del server*/
#define PUERTO       (8000)				            /* Puerto utilizada por el server*/
#define PROTOCOLO   "HTTP/1.0"			            /* Protocolo HTTP (version)*/
#define PENDIENTES  (1024)				            /* Tamaño maximo de conexiones pendientes*/
#define MAXBUF      (4096)				            /* Tamaño maximo de buffer*/                                         
#define RFC1123     "%a, %d %b %Y %H:%M:%S GMT"		/* Formato de hora para cabecera*/ 

//PATH a archivos comunes
#define DIR_TP      "./recursos"
#define ERR400      "./recursos/redes/error400.html"
#define ERR404      "./recursos/redes/error404.html"
#define ERR501      "./recursos/redes/error501.html"
