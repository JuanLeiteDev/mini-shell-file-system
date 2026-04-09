#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TAMANHO_BUFFER 1024

void escrevaErro(const int fd, const char msg[]){
    int len = 0;
    while(msg[len] != '\0') len++;
    write(fd, msg, len);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        escrevaErro(2, "Uso: mostra <ficheiro>\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        if(errno == ENOENT){
            escrevaErro(2, "O ficheiro não existe.\n");
        } else {
            escrevaErro(2, "Erro ao tentar abrir ficheiro.\n");
        }
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;
    while((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0){
        write(STDOUT_FILENO, buffer, bytes_lidos);
    }

    if(bytes_lidos == -1){
        escrevaErro(2, "Erro ao tentar ler ficheiro.\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
