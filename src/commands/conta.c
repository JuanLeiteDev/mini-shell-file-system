#include "../include/utils.h"

int main(int argc, char *argv[]){
    if(argc != 2){
        escrevaErro("Uso: conta <ficheiro>\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(naoExiste(fd, argv[1])) return 1;

    int inicio = 0;
    unsigned int arr[3] = {0, 0, 0};
    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos = 0;

    while((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0){
        for(int i = 0; i < bytes_lidos; i++){
            if(buffer[i] == '\n') arr[0]++;
            if(inicio == 1 && buffer[i] == ' ' || buffer[i] == '\0' || buffer[i] == '\n' 
                || buffer[i] == '\t' || buffer[i] == '\r'){
                inicio = 0;
                arr[1]++;
            }
            if(inicio == 0 && buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0'){
                inicio = 1;
            }
            arr[2]++;
        }
    }
    if(inicio == 1) arr[1]++;

    if(bytes_lidos == -1){
        escrevaErro("Erro ao ler ficheiro.\n");
        close(fd);
        return 1;
    }

    for(int i = 0; i < 3;i++){
        int tamanho = 0;
        char aux[TAMANHO_BUFFER];
        converteInt(arr[i], buffer, &tamanho);
        buffer[tamanho] = ' ';
        buffer[tamanho+1] = '\0';
        write(STDOUT_FILENO, buffer, tamanho+1);
    }
    write(STDOUT_FILENO, argv[1], sizeof(argv[1]));

    close(fd);
    return 0;
}