# Mini Shell File System

Mini-sistema de gestão de ficheiros com interpretador de comandos personalizado, implementado em C puro com chamadas ao sistema POSIX (sem LibC de alto nível, sem `system()`).

Trabalho prático de Sistemas Operativos — Lic. Eng. Sistemas Informáticos, IPCA 2025/2026.

## Comandos implementados

| Comando | Uso | Descrição |
| --- | --- | --- |
| `mostra` | `mostra ficheiro` | Exibe o conteúdo de um ficheiro |
| `copia` | `copia origem destino` | Copia um ficheiro |
| `acrescenta` | `acrescenta origem destino` | Concatena origem no final de destino |
| `conta` | `conta ficheiro` | Conta linhas, palavras e caracteres |
| `apaga` | `apaga ficheiro` | Remove um ficheiro |
| `informa` | `informa ficheiro` | Mostra metadados, permissões e datas |
| `lista` | `lista [directoria]` | Lista ficheiros e pastas |
| `procura` | `procura ficheiro padrão` | Procura linhas que contenham um padrão |
| `comandos` | `comandos` | Lista todos os comandos disponíveis para uso |

Todos os erros são reportados via `stderr`. Implementados com `open`, `read`, `write`, `close`, `stat`, `opendir`/`readdir`.

## Interpretador

O `interpretador` apresenta o símbolo `%` como prompt e suporta:

- execução de comandos via `fork` + `execve` + `wait`
- redirecionamento de saída: `lista /tmp > saida.txt`
- redirecionamento de entrada: `conta < dados.txt`
- pipe simples entre dois comandos: `lista /tmp | procura .log`
- comando `termina` para encerrar

```sh
$ ./build/bin/interpretador
% lista /home
...
Terminou comando lista com código 0
% mostra ficheiro.txt > copia.txt
Terminou comando mostra com código 0
% termina
$
```

## Compilação e execução

```bash
make               # compila tudo para build/bin/
make clean         # remove artefactos
```

Executar diretamente:

```bash
./build/bin/interpretador
./build/bin/mostra ficheiro.txt
./build/bin/lista /tmp
```

## Docker

```bash
docker build -t mini-shell-file-system .
docker run --rm -it mini-shell-file-system
```

O container arranca diretamente no interpretador.

## Estrutura

```text
.
├── src/
│   ├── commands/       # acrescenta, apaga, conta, copia, informa, lista, mostra, procura
│   ├── include/        # utils.h / utils.c (partilhados)
│   └── interpretador.c
├── build/bin/          # binários compilados
├── docs/               # enunciado PDF
├── tmp/                # ficheiros de teste
├── Makefile
└── dockerfile
```
