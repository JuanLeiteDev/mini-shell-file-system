#include "utils.h"
#include <stdio.h>

void escrevaErro(char msg[]){
    int len = 0;
    while(msg[len] != '\0') len++;
    write(STDERR_FILENO, msg, len);
}

char *concatenarString(char s1[], char s2[]){
    int i = 0;
    int j = 0;

    while(s1[i] != '\0') i++;

    s1[i] = ' ';
    i++;

    while(s2[j] != '\0'){
        s1[i+j] = s2[j];
        j++;
    }

    s1[i+j] = '\n';
    s1[i+j+1] = '\0';

    return s1;
}

int naoExiste(int fd, char nomeFicheiro[]){
    if(fd == -1){
        if(errno == ENOENT){
            char aviso[TAMANHO_BUFFER] = "O ficheiro não existe";
            char *msg = concatenarString(aviso, nomeFicheiro);
            escrevaErro(msg);
        } else {
            char aviso[TAMANHO_BUFFER] = "Erro ao abrir ficheiro";
            char *msg = concatenarString(aviso, nomeFicheiro);
            escrevaErro(msg);
        }
        return 1;
    }
    return 0;
}

void converteInt(int num, char *buffer, int *total_bytes){
    if(num == 0){
        buffer[0] = '0';
        buffer[1] = ' ';
        buffer[2] = '\0';
        *total_bytes = 2;
        return;
    }
    
    int aux;
    int tamanho = -1;
    for(int i = 0; num != 0; i++){
        aux = num % 10;
        num = num / 10;
        buffer[i] = aux + '0';
        tamanho++;
    }
    *total_bytes = tamanho+2;
    buffer[tamanho+1] = ' ';
    buffer[tamanho+2] = '\0';
    for(int j = 0; tamanho > 0; j++){
        char temp = buffer[j];
        buffer[j] = buffer[j+tamanho];
        buffer[tamanho] = temp;
        tamanho = tamanho - 2;
    }
}
