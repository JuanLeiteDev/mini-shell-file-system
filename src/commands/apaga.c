#include "../include/utils.h"

int main(int argc, char *argv[]){
    if(argc != 2){
        escrevaErro("Uso: apaga <ficheiro>\n");
        return 1;
    }

    if(unlink(argv[1]) != 0){
        escrevaErro("Erro ao apagar ficheiro.\n");
        return 1;
    }

    return 0;
}