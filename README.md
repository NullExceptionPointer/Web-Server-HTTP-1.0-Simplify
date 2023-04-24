# Web-Server-HTTP-1.0-Simplify
a server develop in c. This server work concurrent with fork() and excev(). It accept GET and HEAD. Made for the universiti UDC

#Espaniol: 

Servidor web http 1.0 desarrollado en c, maneja la concurrencia utilizando fork() y para ejecutar el servicio utilizamos execv(). Este es un servidor simplificado que reconoce los metodo GET y HEAD como validos, en caso de otro metodo devolvera error 501, Puede reconocer peticiones mal hechas hasta cierto punto, el servidor devolvera error 400.

Este projecto fue realizado con fines de estudio de la tecnologia socket y el protocolo http para la universidad del chubut, argentina.

Fue realizado por:
    Macarena Belén Garcia Arcija
    Mauro Leiva
    Lucas Isaac Soto

Sistema operativo utilizado: linux mint 21.1.

Instrucciones para probar el trabjo.
* ingresar a la carpeta TP1-2023.
* abrir una consola
* make clean 
* make

con esto el projecto compilara de forma limpia el programa.

para correr el server:
* ./server

con esto el servidor ya esta corriendo, utilizar el navegador con: http://localhost:8000/redes/index.html para probar.


#English: 

HTTP 1.0 web server developed in C, handles concurrency using fork() and executes the service using execv(). This is a simplified server that recognizes GET and HEAD methods as valid. In case of any other method, it returns a 501 error. It can recognize bad requests to some extent, and the server will return a 400 error.

This project was created for studying socket technology and the HTTP protocol for the University of Chubut, Argentina.

It was developed by:
Macarena Belén Garcia Arcija
Mauro Leiva
Lucas Isaac Soto

Operating system used: Linux Mint 21.1.

Instructions to test the project:

Go to the TP1-2023 folder.
Open a console.
make clean
make
This will compile the program cleanly.

To run the server:

./server
The server is now running. Use the browser with http://localhost:8000/redes/index.html to test it.