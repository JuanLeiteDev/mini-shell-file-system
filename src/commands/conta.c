#include "../include/utils.h"

int main(int argc, char *argv[]){
    if(argc > 2){
        escrevaErro("Uso: conta [ficheiro]\n");
        return 1;
    }

    int fd;
    if(argc == 2){
        fd = open(argv[1], O_RDONLY);
        if(naoExiste(fd, argv[1])) return 1;
    } else {
        fd = STDIN_FILENO;
    }

    int emPalavra = 0;
    unsigned int linhas = 0, palavras = 0, caracteres = 0;
    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos = 0;

    while((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0){
        for(int i = 0; i < bytes_lidos; i++){
            if(buffer[i] == '\n') linhas++;

            if(emPalavra == 1 && (buffer[i] == ' ' || buffer[i] == '\0' || buffer[i] == '\n'
                    || buffer[i] == '\t' || buffer[i] == '\r')){
                emPalavra = 0;
                palavras++;
            }

            if(emPalavra == 0 && buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0'
                    && buffer[i] != '\t' && buffer[i] != '\r'){
                emPalavra = 1;
            }

            caracteres++;
        }
    }
    if(emPalavra == 1) palavras++;

    if(bytes_lidos == -1){
        escrevaErro("Erro ao ler ficheiro.\n");
        if(fd != STDIN_FILENO) close(fd);
        return 1;
    }

    unsigned int contagens[3] = {linhas, palavras, caracteres};
    for(int i = 0; i < 3; i++){
        int tamanho = 0;
        converteInt(contagens[i], buffer, &tamanho);
        buffer[tamanho] = ' ';
        buffer[tamanho+1] = '\0';
        write(STDOUT_FILENO, buffer, tamanho+1);
    }

    if(argc == 2){
        int tamanho_nome = 0;
        while(argv[1][tamanho_nome] != '\0') tamanho_nome++;
        write(STDOUT_FILENO, argv[1], tamanho_nome);
    } else {
        write(STDOUT_FILENO, "stdin", 5);
    }
    write(STDOUT_FILENO, "\n", 1);

    if(fd != STDIN_FILENO) close(fd);
    return 0;
}
