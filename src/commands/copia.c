#include "../include/utils.h"

int main(int argc, char *argv[]) {
    if(argc != 3){
        escrevaErro("Uso: copia <origem> <destino>\n");
        return 1;
    }

    int fd_origem = open(argv[1], O_RDONLY);
    if(naoExiste(fd_origem, argv[1])) return 1;

    int fd_check = open(argv[2], O_RDONLY);
    if(fd_check != -1){
        close(fd_check);
        escrevaErro("Aviso: O ficheiro de destino já existe.\n");
        return 1;
    }

    int fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);
    if(fd_destino == -1){
        escrevaErro("Erro ao criar ficheiro de destino.\n");
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos = 0;
    while((bytes_lidos = read(fd_origem, buffer, TAMANHO_BUFFER)) > 0){
        write(fd_destino, buffer, bytes_lidos);
    }

    if(bytes_lidos == -1){
        escrevaErro("Erro ao escrever no ficheiro de destino.\n");
        close(fd_destino);
        close(fd_origem);
        return 1;
    }

    close(fd_destino);
    close(fd_origem);
    return 0;
}
