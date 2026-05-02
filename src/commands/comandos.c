#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <stdbool.h>

#define PATH_MAX 4096

bool strcompara(char s1[], char s2[]){
    int i = 0;
    for(;s1[i] != '\0' && s2[i] != '\0'; i++){
        if(s1[i] != s2[i]) return false;
    }
    if(s1[i] != s2[i]) return false;
    return true;
}

char *obterDiretorio(char path[], ssize_t *len){
    while(path[*len] != '/') (*len)--;
    path[*len] = '\0';

    return path;
}

int meuLen(char str[]){
    int len = 0;
    while(str[len] != '\0') len++;
    return len;
}

int main(void){
    char exePATH[PATH_MAX];

    // Pegar caminho do executável
    ssize_t len = syscall(SYS_readlink, "/proc/self/exe", exePATH, PATH_MAX);

    // Obter diretorio do executável
    char *charPATH = obterDiretorio(exePATH, &len);

    DIR *diretorio = opendir(charPATH);

    if(!diretorio){write(STDERR_FILENO, "Erro ao abrir diretório dos comandos.\n", 40); exit(EXIT_FAILURE);}

    struct dirent *entrada;
    while((entrada = readdir(diretorio)) != NULL){
        if(strcompara(entrada->d_name, ".") || 
        strcompara(entrada->d_name, "..") || 
        strcompara(entrada->d_name, "interpretador")) continue;

        int tamanho = meuLen(entrada->d_name);
        write(STDOUT_FILENO, entrada->d_name, tamanho);
        write(STDOUT_FILENO, "\n", 2);
    }
    
    return 0;
}