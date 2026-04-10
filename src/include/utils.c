#include "utils.h"

void escrevaErro(const char msg[]){
    int len = 0;
    while(msg[len] != '\0') len++;
    write(STDERR_FILENO, msg, len);
}

int naoExiste(const int fd){
    if(fd == -1){
        if(errno == ENOENT){
            escrevaErro("O ficheiro não existe.\n");
        } else {
            escrevaErro("Erro ao abrir ficheiro.\n");
        }
        return 1;
    }
    return 0;
}