#include "./include/utils.h"
#include <sys/syscall.h>
#include <sys/wait.h>

void escrevaCaminho() {
    char caminhoAtual[TAMANHO_BUFFER];
    syscall(SYS_getcwd, caminhoAtual, TAMANHO_BUFFER);
    char aux[TAMANHO_BUFFER] = "\x1b[1;34m";
    concatenarString(aux, caminhoAtual);
    concatenarString(aux, "\x1b[0m");
    concatenarString(aux, " % ");
    escreva(aux);
}

void obterDiretorioRaiz(char caminho[], ssize_t tamanho){
    while(tamanho >= 0){
        if(caminho[tamanho] == '/'){ caminho[tamanho+1] = '\0'; break; }
        tamanho--;
    }
}

ssize_t descobrirCaminho(char buffer[]){
    ssize_t bytes = syscall(SYS_readlink, "/proc/self/exe", buffer, TAMANHO_BUFFER);
    if(bytes < 0) return -1;
    buffer[bytes] = '\0';
    return bytes;
}

int receberEntrada(char buffer[], char *argumentos[]){
    int emPalavra = 0;
    int nArgumentos = 0;
    int i = 0;
    int temAspas = 0;

    for (; buffer[i] != '\0'; i++) {
        if (!emPalavra) {
            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
                continue;

            emPalavra = 1;

            if (buffer[i] == '"') {
                temAspas = 1;
                argumentos[nArgumentos] = &buffer[i + 1];
                continue;
            }

            argumentos[nArgumentos] = &buffer[i];
            continue;
        }

        if (temAspas) {
            if (buffer[i] == '"') {
                buffer[i] = '\0';
                temAspas = 0;
                emPalavra = 0;
                nArgumentos++;
            }
        } else {
            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t') {
                buffer[i] = '\0';
                emPalavra = 0;
                nArgumentos++;
            }
        }
    }

    if (temAspas)
        return -1;

    if (emPalavra)
        nArgumentos++;

    argumentos[nArgumentos] = NULL;
    return nArgumentos;
}

int verificarExistencia(char caminho[]){
    long resultado = syscall(SYS_access, caminho, X_OK);
    if(resultado < 0) return -1;
    return 0;
}

int executar_cd(char *argv[]){
    if(argv[1] == NULL) return -1;
    long resultado;
    resultado = syscall(SYS_chdir, argv[1]);
    if(resultado < 0) return -1;
    return 0;
}

int main(){
    limpaTela();
    do{
        char buffer[TAMANHO_BUFFER];
        escrevaCaminho();

        ssize_t bytes_lidos = read(STDIN_FILENO, buffer, TAMANHO_BUFFER);
        buffer[bytes_lidos] = '\0';

        char *argumentos[TAMANHO_BUFFER];
        int nArgumentos = receberEntrada(buffer, argumentos);

        if(nArgumentos == 0){ escrevaErro("Nenhum comando digitado.\n"); continue; }
        if(nArgumentos < 0){ escrevaErro("Erro na digitação.\n"); continue; }
        if(strIgual("termina", argumentos[0])) return 0;
        if(strIgual("cd", argumentos[0])){
            if(executar_cd(argumentos) < 0) escrevaErro("Erro ao executar comando.\n");
            continue; 
        }

        char caminho_comandos[TAMANHO_BUFFER];
        ssize_t caminhoLen = descobrirCaminho(caminho_comandos);
        if(caminhoLen < 0){ escrevaErro("Erro ao obter caminho do comando.\n"); continue; }
        obterDiretorioRaiz(caminho_comandos, caminhoLen);
        concatenarString(caminho_comandos, argumentos[0]);
        if(verificarExistencia(caminho_comandos) < 0) {escrevaErro("Comando não encontrado.\n"); continue; }

        pid_t pid = fork();
        if(pid == 0){
            execv(caminho_comandos, argumentos);
            escrevaErro("Erro ao executar comando.\n");
        } else {
            wait(NULL);
        }

    } while(1);
    return 0;
}