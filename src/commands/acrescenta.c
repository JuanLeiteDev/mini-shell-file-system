#include "../include/utils.h"

int main(int argc, char *argv[]){
    if(argc != 3){
        escrevaErro("Uso: acrescenta <origem> <destino>\n");
        return 1;
    }

    int fd_origem = open(argv[1], O_RDONLY);
    if(naoExiste(fd_origem, argv[1])){
        return 1;
    }

    int fd_destino = open(argv[2], O_WRONLY | O_APPEND);
    if(naoExiste(fd_destino, argv[2])) return 1;

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;
    while((bytes_lidos = read(fd_origem, buffer, TAMANHO_BUFFER)) > 0){
        write(fd_destino, buffer, bytes_lidos);
    }

    if(bytes_lidos == -1){
        escrevaErro("Erro ao ler o ficheiro de origem.\n");
        close(fd_origem);
        close(fd_destino);
        return 1;
    }

    close(fd_origem);
    close(fd_destino);

    return 0;
}