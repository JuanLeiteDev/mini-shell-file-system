#include "../include/utils.h"

int comprimento(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

int contemPadrao(char *linha, int tamanhoLinha, char *padrao, int tamanhoPadrao) {
    for(int i = 0; i <= tamanhoLinha - tamanhoPadrao; i++){
        int j;
        for(j = 0; j < tamanhoPadrao; j++){
            if(linha[i+j] != padrao[j]) break;
        }
        if(j == tamanhoPadrao) return 1;
    }
    return 0;
}

void imprimeLinha(int numLinha, char *linha) {
    char aux[TAMANHO_BUFFER];
    int tamanho;
    converteInt(numLinha, aux, &tamanho);
    concatenarString(aux, " ");
    concatenarString(aux, linha);
    escreva(aux);
    escreva("\n");
}

int main(int argc, char *argv[]){
    if(argc < 2){
        escrevaErro("Uso: procura [ficheiro] <padrao>\n");
        return 1;
    }

    char padrao[TAMANHO_BUFFER];
    padrao[0] = '\0';

    int fd;
    if(argc >= 3){
        for(int i = 2; i <= argc-1; i++){
            concatenarString(padrao, argv[i]);
            if(i < argc-1) concatenarString(padrao, " ");
        }
        fd = open(argv[1], O_RDONLY);
        if(naoExiste(fd, argv[1])) return 1;
    } else {
        concatenarString(padrao, argv[1]);
        fd = STDIN_FILENO;
    }

    char linha[TAMANHO_BUFFER];
    char buffer[1];
    int numLinha = 1;
    int posLinha = 0;
    int bytes_lidos;
    int tamanhoPadrao = comprimento(padrao);
    int encontrou = 0;

    while((bytes_lidos = read(fd, buffer, 1)) > 0){
        if(buffer[0] == '\n'){
            linha[posLinha] = '\0';
            if(contemPadrao(linha, posLinha, padrao, tamanhoPadrao)){
                encontrou = 1;
                imprimeLinha(numLinha, linha);
            }
            posLinha = 0;
            numLinha++;
        } else {
            linha[posLinha++] = buffer[0];
        }
    }

    // Última linha do ficheiro caso não termine com '\n'
    if(posLinha > 0){
        linha[posLinha] = '\0';
        if(contemPadrao(linha, posLinha, padrao, tamanhoPadrao)){
            encontrou = 1;
            imprimeLinha(numLinha, linha);
        }
    }

    if(bytes_lidos == -1){
        escrevaErro("Erro ao ler ficheiro.\n");
        if(fd != STDIN_FILENO) close(fd);
        return 1;
    }

    if(!encontrou){
        escreva("Nenhuma linha encontrada.\n");
    }

    if(fd != STDIN_FILENO) close(fd);
    return 0;
}
