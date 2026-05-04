#include "./include/utils.h"

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
        char aux[TAMANHO_BUFFER];
        int tamanho_aux = 0;
        escrevaCaminho();

        ssize_t bytes_lidos = read(STDIN_FILENO, buffer, TAMANHO_BUFFER);
        if(bytes_lidos <= 0) return 0;
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

        Contador seta_direita;
        Contador seta_esquerda;
        Contador conta_pipe;
        seta_direita.qtd = 0, seta_esquerda.qtd = 0, conta_pipe.qtd = 0;
        for(int i = 0; i < nArgumentos; i++){
            if(strIgual(argumentos[i], ">")){
                seta_direita.arr[seta_direita.qtd] = i; 
                seta_direita.qtd++; 
            } else if(strIgual(argumentos[i], "<")){
                seta_esquerda.arr[seta_esquerda.qtd] = i;
                seta_esquerda.qtd++;
            } else if(strIgual(argumentos[i], "|")){
                conta_pipe.arr[conta_pipe.qtd] = i;
                conta_pipe.qtd++;
            }
        }
        
        if(seta_direita.qtd > 1){ escrevaErro("Erro na digitação. É aceitável apenas uma \">\"\n"); continue; }
        if(seta_esquerda.qtd > 1){ escrevaErro("Erro na digitação. É aceitável apenas uma \"<\"\n"); continue; }
        if(conta_pipe.qtd > 1){ escrevaErro("Erro na digitação. É aceitável apenas um \"|\"\n"); continue; }

        pid_t pid = fork();
        if(pid < 0){ escrevaErro("Erro ao criar processo.\n"); continue; }
        if(pid == 0){
            if(seta_direita.qtd == 1){
                if(argumentos[(seta_direita.arr[0])+1] != NULL){
                    int fd = open(argumentos[(seta_direita.arr[0])+1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if(fd == -1){
                        escrevaErro("Erro ao abrir ficheiro de redirecionamento.\n");
                        _exit(1);
                     } else {
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        argumentos[(seta_direita.arr[0])] = NULL;
                    }
                } else {
                    escrevaErro("Nenhum ficheiro especificado para redirecionamento.\n");
                    _exit(1);
                }
            }
            if(seta_esquerda.qtd == 1){
                if(argumentos[(seta_esquerda.arr[0])+1] != NULL){
                    int fd = open(argumentos[(seta_esquerda.arr[0])+1], O_RDONLY);
                    if(fd == -1){
                        escrevaErro("Erro ao abrir ficheiro de redirecionamento.\n");
                        _exit(1);
                     } else {
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                        for(int i = seta_esquerda.arr[0]; ; i++){
                            argumentos[i] = argumentos[i+2];
                            if(argumentos[i] == NULL) break;
                        }
                        // Bug 3: atualiza índice do pipe se o '<' veio antes dele
                        if(conta_pipe.qtd == 1 && seta_esquerda.arr[0] < conta_pipe.arr[0])
                            conta_pipe.arr[0] -= 2;
                    }
                } else {
                    escrevaErro("Nenhum ficheiro especificado para redirecionamento.\n");
                    _exit(1);
                }
            }

            if(conta_pipe.qtd > 0){
                int fd[2];
                if(pipe(fd) == -1){ escrevaErro("Erro ao executar o pipe.\n"); _exit(1); }

                pid_t pid2 = fork();
                if(pid2 < 0){ escrevaErro("Erro ao executar fork.\n"); _exit(1); }
                if(pid2 == 0){
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                    argumentos[conta_pipe.arr[0]] = NULL;
                    execv(caminho_comandos, argumentos);
                    escrevaErro("Erro ao executar primeiro comando.\n");
                    _exit(1);
                } else {
                    close(fd[1]);
                    for(int i = 0;; i++){
                        argumentos[i] = argumentos[i+conta_pipe.arr[0]+1];
                        if(argumentos[i] == NULL) break;
                    }
                    if(argumentos[0] == NULL){
                        escrevaErro("Nenhum comando especificado após '|'.\n");
                        _exit(1);
                    }
                    obterDiretorioRaiz(caminho_comandos, caminhoLen);
                    concatenarString(caminho_comandos, argumentos[0]);
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                    execv(caminho_comandos, argumentos);
                    escrevaErro("Erro ao executar segundo comando.\n");
                    _exit(1);
                }
            } else {
                execv(caminho_comandos, argumentos);
                escrevaErro("Erro ao executar comando.\n");
                _exit(1);
            }
        } else {
            int status;
            if(waitpid(pid, &status, 0) == -1){
                escrevaErro("Erro ao executar comando.\n");
            }
            if (WIFEXITED(status)) {
                int codigo = WEXITSTATUS(status);
                if(codigo != 0){
                    converteInt(codigo, aux, &tamanho_aux);
                    char msg[TAMANHO_BUFFER] = "O comando não foi concluído com sucesso. Código de saída: ";
                    concatenarString(msg, aux);
                    escrevaErro(msg);
                    escreva("\n");
                } else {
                    escreva("O comando terminou com sucesso lógico. Código de saída: 0\n");
                }
            } else {
                escrevaErro("O comando terminou de forma não esperada.\n");
            }
        }

    } while(1);
    return 0;
}