#include "../include/utils.h"

int main(int argc, char *argv[]){
    if(argc != 2){
        escrevaErro("Uso: mostra <ficheiro>\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        if(errno == ENOENT){
            escrevaErro("O ficheiro não existe.\n");
        } else {
            escrevaErro("Erro ao abrir ficheiro.\n");
        }
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;
    while((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0){
        write(STDOUT_FILENO, buffer, bytes_lidos);
    }

    if(bytes_lidos == -1){
        escrevaErro("Erro ao ler ficheiro.\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
