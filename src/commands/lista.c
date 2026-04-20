#include "../include/utils.h"

int main(int argc, char *argv[]) {
    if (argc > 2) {
        escrevaErro("Uso: lista [directoria]\n");
        return 1;
    }

    char *caminho = (argc == 2) ? argv[1] : ".";

    struct stat dados;
    if (stat(caminho, &dados) == -1) {
        escrevaErro("Erro: diretoria não encontrada.\n");
        return 1;
    }

    if (!S_ISDIR(dados.st_mode)) {
        escrevaErro("Erro: o caminho indicado não é uma diretoria.\n");
        return 1;
    }

    DIR *dir = opendir(caminho);
    if (dir == NULL) {
        escrevaErro("Erro ao abrir a diretoria.\n");
        return 1;
    }

    struct dirent *entrada;
    while ((entrada = readdir(dir)) != NULL) {
        if (entrada->d_name[0] == '.' && (entrada->d_name[1] == '\0' ||
            (entrada->d_name[1] == '.' && entrada->d_name[2] == '\0'))) {
            continue;
        }

        char caminho_entrada[TAMANHO_BUFFER];
        caminho_entrada[0] = '\0';
        concatenarString(caminho_entrada, caminho);
        concatenarString(caminho_entrada, "/");
        concatenarString(caminho_entrada, entrada->d_name);

        struct stat info;
        if (stat(caminho_entrada, &info) == -1) {
            escrevaErro("Erro ao obter informações da entrada.\n");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            escreva("[dir] ");
        } else {
            escreva("[fic] ");
        }
        escreva(entrada->d_name);
        escreva("\n");
    }

    closedir(dir);
    return 0;
}
