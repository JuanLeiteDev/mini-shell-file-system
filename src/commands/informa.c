#include "../include/utils.h"
#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        escrevaErro("Uso: informa <ficheiro>\n");
        return 1;
    }

    struct stat dados;
    char final[TAMANHO_BUFFER];
    char aux[TAMANHO_BUFFER];
    int tamanho = 0;

    if(stat(argv[1], &dados) == -1){
        escrevaErro("Erro ao obter informações.\n");
        return 1;
    }

    unsigned long i_node = dados.st_ino;
    converteLong(i_node, aux, &tamanho);
    char permissoes[11];

    if (S_ISDIR(dados.st_mode)) permissoes[0] = 'd';
    else if (S_ISREG(dados.st_mode)) permissoes[0] = '-';
    else if (S_ISLNK(dados.st_mode)) permissoes[0] = 'l';
    else if (S_ISCHR(dados.st_mode)) permissoes[0] = 'c';
    else if (S_ISBLK(dados.st_mode)) permissoes[0] = 'b';
    else if (S_ISFIFO(dados.st_mode)) permissoes[0] = 'p';
    else if (S_ISSOCK(dados.st_mode)) permissoes[0] = 's';
    
    permissoes[1] = (dados.st_mode & S_IRUSR) ? 'r' : '-';
    permissoes[2] = (dados.st_mode & S_IWUSR) ? 'w' : '-';
    permissoes[3] = (dados.st_mode & S_IXUSR) ? 'x' : '-';
    permissoes[4] = (dados.st_mode & S_IRGRP) ? 'r' : '-';
    permissoes[5] = (dados.st_mode & S_IWGRP) ? 'w' : '-';
    permissoes[6] = (dados.st_mode & S_IXGRP) ? 'x' : '-';
    permissoes[7] = (dados.st_mode & S_IROTH) ? 'r' : '-';
    permissoes[8] = (dados.st_mode & S_IWOTH) ? 'w' : '-';
    permissoes[9] = (dados.st_mode & S_IXOTH) ? 'x' : '-';
    permissoes[10] = '\0';

    concatenarString(final, aux);
    concatenarString(final, permissoes);

    unsigned long nLinks = dados.st_nlink;
    converteLong(nLinks, aux, &tamanho);
    concatenarString(final, aux);

    unsigned int user_id = dados.st_uid;
    converteInt(user_id, aux, &tamanho);

    int fd = open("/etc/passwd", O_RDONLY);
    if(fd != -1){
        char nome[100];
        nome[0] = '\0';
        if(procurarNomeUser(fd, aux, nome)){
            concatenarString(final, nome);
        } else {
            char msg[50] = "UID:";
            concatenarString(msg, aux);
            concatenarString(final, msg);
        }
    } else {
        char msg[50] = "UID:";
        concatenarString(msg, aux);
        concatenarString(final, msg);  
    }
    printf("%s", final);
    return 0;
}