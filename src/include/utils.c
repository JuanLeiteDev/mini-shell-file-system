#include "utils.h"

void escreva(char msg[]){
    int len = 0;
    while(msg[len] != '\0') len++;
    write(STDIN_FILENO, msg, len);
}

void escrevaErro(char msg[]){
    int len = 0;
    while(msg[len] != '\0') len++;
    write(STDERR_FILENO, msg, len);
}

void concatenarString(char s1[], char s2[]){
    int i = 0;
    int j = 0;

    while(s1[i] != '\0') i++;

    while(s2[j] != '\0'){
        s1[i+j] = s2[j];
        j++;
    }

    s1[i+j] = '\0';
}

int naoExiste(int fd, char nomeFicheiro[]){
    if(fd == -1){
        if(errno == ENOENT){
            char aviso[TAMANHO_BUFFER] = "O ficheiro não existe ";
            concatenarString(nomeFicheiro, "\n");
            concatenarString(aviso, nomeFicheiro);
            escrevaErro(aviso);
        } else {
            char aviso[TAMANHO_BUFFER] = "Erro ao abrir ficheiro ";
            concatenarString(nomeFicheiro, "\n");
            concatenarString(aviso, nomeFicheiro);
            escrevaErro(aviso);
        }
        return 1;
    }
    return 0;
}

void converteInt(unsigned int num, char *buffer, int *total_bytes){
    if(num == 0){
        buffer[0] = '0';
        buffer[1] = '\0';
        *total_bytes = 2;
        return;
    }
    
    unsigned int aux;
    int index = -1;
    for(int i = 0; num != 0; i++){
        aux = num % 10;
        num = num / 10;
        buffer[i] = aux + '0';
        index++;
    }
    *total_bytes = index+2;
    buffer[index+1] = '\0';
    for(int j = 0; index > 0; j++){
        char temp = buffer[j];
        buffer[j] = buffer[j+index];
        buffer[index+j] = temp;
        index = index - 2;
    }
}

void converteLong(unsigned long num, char *buffer, int *total_bytes){
    if(num == 0){
        buffer[0] = '0';
        buffer[1] = '\0';
        *total_bytes = 1;
        return;
    }
    
    unsigned long aux;
    int index = -1;
    for(int i = 0; num != 0; i++){
        aux = num % 10;
        num = num / 10;
        buffer[i] = aux + '0';
        index++;
    }
    *total_bytes = index+2;
    buffer[index+1] = '\0';

    for(int j = 0; index > 0; j++){
        char temp = buffer[j];
        buffer[j] = buffer[j+index];
        buffer[index+j] = temp;
        index = index - 2;
    }
}

int strIgual(char s1[], char s2[]){
    int i = 0;
    for(; s1[i] != '\0' && s2[i] != '\0'; i++){
        if(s1[i] == s2[i]) continue;
        else return 0;
    }
    if(s1[i] != s2[i]) return 0;
    else return 1;
}

int procurarNomeUser(int fd, char userID[], char userNome[]){
    char buffer[TAMANHO_BUFFER];
    char id[100];
    ssize_t bytes_lidos = 0;
    int contador_lidos = 0;
    int contador_pontos = 0;
    int contador_id = 0;
    int contador_nome = 0;
    while((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0){
        while(contador_lidos < bytes_lidos){
            while(contador_lidos < bytes_lidos && buffer[contador_lidos] != '\n'){
                if(contador_pontos == 2 && buffer[contador_lidos] != ':'){
                    id[contador_id] = buffer[contador_lidos];
                    contador_id++;
                } else if (contador_pontos == 2 && buffer[contador_lidos] == ':'){
                    id[contador_id] = '\0';
                }
                if(buffer[contador_lidos] == ':') contador_pontos++;
                if(contador_pontos == 0){
                    userNome[contador_nome] = buffer[contador_lidos];
                    contador_nome++;
                } else if (contador_pontos == 1 && userNome[contador_nome] != '\0') userNome[contador_nome] = '\0';
                contador_lidos++;
            }
            if(contador_lidos == bytes_lidos){
                break;
            } else {
                contador_pontos = 0;
                contador_id = 0;
                contador_nome = 0;
            }
            if(strIgual(userID, id)){
                return 1;
            }
            contador_lidos++;
        }   
        if(strIgual(userID, id)){
            return 1;
        }   
        contador_lidos = 0;  
    }
    if(bytes_lidos == -1) return 0;
}

void limpaTela(){
    escreva("\e[1;1H\e[2J");
}
