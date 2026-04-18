#include "../include/utils.h"

int ehBissexto(int ano) {
    if ((ano % 400) == 0) return 1;
    if ((ano % 100) == 0) return 0;
    if ((ano % 4) == 0) return 1;
    return 0;
}

char *timestampParaString(unsigned long timestamp, char buffer[], size_t tamanhoBuffer) {
    unsigned long dias;
    int segundos;
    int minutos;
    int horas;
    int dia;
    int mes;
    int ano;
    int diasNoAno;
    int i;

    int diasMeses[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if (buffer == NULL) {
        return NULL;
    }

    if (tamanhoBuffer < 20) {
        if (tamanhoBuffer > 0) {
            buffer[0] = '\0';
        }
        return NULL;
    }

    segundos = timestamp % 60;
    timestamp /= 60;

    minutos = timestamp % 60;
    timestamp /= 60;

    horas = timestamp % 24;
    timestamp /= 24;

    dias = timestamp;
    ano = 1970;

    while (1) {
        diasNoAno = ehBissexto(ano) ? 366 : 365;

        if (dias >= (unsigned long)diasNoAno) {
            dias -= diasNoAno;
            ano++;
        } else {
            break;
        }
    }

    if (ehBissexto(ano)) {
        diasMeses[1] = 29;
    }

    mes = 0;
    while (mes < 12 && dias >= (unsigned long)diasMeses[mes]) {
        dias -= diasMeses[mes];
        mes++;
    }

    if (mes >= 12) {
        buffer[0] = '\0';
        return NULL;
    }

    dia = (int)dias + 1;
    mes = mes + 1;

    for (i = 0; i < (int)tamanhoBuffer; i++) {
        buffer[i] = '\0';
    }

    buffer[0]  = (dia / 10) + '0';
    buffer[1]  = (dia % 10) + '0';
    buffer[2]  = '/';
    buffer[3]  = (mes / 10) + '0';
    buffer[4]  = (mes % 10) + '0';
    buffer[5]  = '/';
    buffer[6]  = ((ano / 1000) % 10) + '0';
    buffer[7]  = ((ano / 100) % 10) + '0';
    buffer[8]  = ((ano / 10) % 10) + '0';
    buffer[9]  = (ano % 10) + '0';
    buffer[10] = ' ';
    buffer[11] = (horas / 10) + '0';
    buffer[12] = (horas % 10) + '0';
    buffer[13] = ':';
    buffer[14] = (minutos / 10) + '0';
    buffer[15] = (minutos % 10) + '0';
    buffer[16] = ':';
    buffer[17] = (segundos / 10) + '0';
    buffer[18] = (segundos % 10) + '0';
    buffer[19] = '\0';

    return buffer;
}

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
    char data[20];
    if(timestampParaString((unsigned long)dados.st_ctime, data, sizeof(data)) != NULL){
        concatenarString(final, data);
        concatenarString(final, " ");
    }

    // Nome do ficheiro
    concatenarString(final, argv[1]);
    escreva(final);
    escreva("\n");

    return 0;
}
