#include "../include/utils.h"

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

    // Inode
    final[0] = '\0';
    converteLong(dados.st_ino, aux, &tamanho);
    concatenarString(final, aux);
    concatenarString(final, " ");

    // Tipo de ficheiro (caractere) + permissões simbólicas
    char permissoes[11];
    if      (S_ISDIR(dados.st_mode))  permissoes[0] = 'd';
    else if (S_ISREG(dados.st_mode))  permissoes[0] = '-';
    else if (S_ISLNK(dados.st_mode))  permissoes[0] = 'l';
    else if (S_ISCHR(dados.st_mode))  permissoes[0] = 'c';
    else if (S_ISBLK(dados.st_mode))  permissoes[0] = 'b';
    else if (S_ISFIFO(dados.st_mode)) permissoes[0] = 'p';
    else if (S_ISSOCK(dados.st_mode)) permissoes[0] = 's';
    else                              permissoes[0] = '?';

    permissoes[1]  = (dados.st_mode & S_IRUSR) ? 'r' : '-';
    permissoes[2]  = (dados.st_mode & S_IWUSR) ? 'w' : '-';
    permissoes[3]  = (dados.st_mode & S_IXUSR) ? 'x' : '-';
    permissoes[4]  = (dados.st_mode & S_IRGRP) ? 'r' : '-';
    permissoes[5]  = (dados.st_mode & S_IWGRP) ? 'w' : '-';
    permissoes[6]  = (dados.st_mode & S_IXGRP) ? 'x' : '-';
    permissoes[7]  = (dados.st_mode & S_IROTH) ? 'r' : '-';
    permissoes[8]  = (dados.st_mode & S_IWOTH) ? 'w' : '-';
    permissoes[9]  = (dados.st_mode & S_IXOTH) ? 'x' : '-';
    permissoes[10] = '\0';

    concatenarString(final, permissoes);
    concatenarString(final, " ");

    // Número de links
    converteLong(dados.st_nlink, aux, &tamanho);
    concatenarString(final, aux);
    concatenarString(final, " ");

    // Utilizador dono
    unsigned int user_id = dados.st_uid;
    converteInt(user_id, aux, &tamanho);

    int fd = open("/etc/passwd", O_RDONLY);
    if(fd != -1){
        char nomeUtilizador[100];
        nomeUtilizador[0] = '\0';
        if(procurarNomeUser(fd, aux, nomeUtilizador)){
            concatenarString(final, nomeUtilizador);
        } else {
            char msg[50] = "UID:";
            concatenarString(msg, aux);
            concatenarString(final, msg);
        }
        close(fd);
    } else {
        char msg[50] = "UID:";
        concatenarString(msg, aux);
        concatenarString(final, msg);
    }
    concatenarString(final, " ");

    // Tamanho em bytes
    converteLong((unsigned long)dados.st_size, aux, &tamanho);
    concatenarString(final, aux);
    concatenarString(final, " ");

    // Data de modificação
    char *data = ctime(&(dados.st_atime));
    concatenarString(final, data);
    tamanho = 0;
    while(final[tamanho] != '\0') tamanho++;
    final[tamanho-1] = ' ';

    // Nome do ficheiro
    concatenarString(final, argv[1]);
    escreva(final);
    escreva("\n");

    return 0;
}
