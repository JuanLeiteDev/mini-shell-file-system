#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TAMANHO_BUFFER 1024

int main(int argc, char *argv[]){
    if(argc != 2){
        write(STDERR_FILENO, "Uso: mostra <ficheiro>\n", 24);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        if(errno == ENOENT){
            write(STDERR_FILENO, "O ficheiro não existe.\n", 24);
        } else {
            write(STDERR_FILENO, "Erro ao tentar abrir ficheiro.\n", 32);
        }
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;
    while((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0){
        write(STDOUT_FILENO, buffer, bytes_lidos);
    }

    if(bytes_lidos == -1){
        write(STDERR_FILENO, "Erro ao tentar ler ficheiro.\n", 29);
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
