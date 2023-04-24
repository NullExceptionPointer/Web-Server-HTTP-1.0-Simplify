all: server servicio

server: server.c server-function.c
		gcc $^ -o $@

servicio: servicio.c servicio-function.c
		gcc $^ -o $@

clean:
	rm -f server servicio