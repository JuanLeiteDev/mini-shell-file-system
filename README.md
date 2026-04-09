# Mini Shell File System

Um projeto pessoal em C para estudar chamadas de sistema no Linux, manipulação de ficheiros, criação de processos e construção de um interpretador de comandos simples.

Embora a ideia tenha nascido a partir de um trabalho académico de Sistemas Operativos, este repositório está a ser organizado como um projeto pessoal de aprendizagem, implementação prática e documentação técnica.

## Sobre o projeto

O objetivo deste projeto é construir um pequeno ecossistema de ferramentas de linha de comando usando C puro e primitivas do sistema operativo, evitando atalhos comuns da biblioteca padrão e sem depender de comandos externos para executar a lógica principal.

A proposta central é desenvolver:

- um conjunto de comandos próprios para manipulação de ficheiros;
- um interpretador de comandos que consiga executá-los;
- suporte a redirecionamento de entrada e saída;
- suporte a um pipe simples entre dois comandos;
- uma forma de empacotar tudo com Docker.

Em termos práticos, a ideia é usar este projeto para aprofundar conceitos clássicos de Sistemas Operativos de forma aplicada, com código pequeno, direto e próximo do sistema.

## Origem da ideia

Este projeto foi inspirado no enunciado presente em `docs/TP-SO-esi-2526.pdf`, que propõe a construção de um mini-sistema de gestão de ficheiros com interpretador próprio.

Em vez de tratar isso apenas como uma entrega de disciplina, a proposta aqui é transformar o enunciado numa base de projeto pessoal, com foco em:

- organização de repositório;
- documentação clara;
- evolução incremental;
- código compreensível;
- estudo prático de APIs POSIX/Linux.

## O que pretendo construir

### 1. Comandos de ficheiros

Com base no PDF, os comandos previstos são:

- `mostra`: exibe todo o conteúdo de um ficheiro;
- `copia`: cria uma cópia de um ficheiro;
- `acrescenta`: concatena o conteúdo de um ficheiro ao final de outro;
- `conta`: conta linhas, palavras e caracteres;
- `apaga`: remove um ficheiro;
- `informa`: mostra metadados e permissões;
- `lista`: lista ficheiros e diretórios;
- `procura`: procura linhas que contenham um padrão.

Todos estes comandos devem ser implementados em C, com chamadas ao sistema e tratamento explícito de erros via `stderr`.

### 2. Interpretador de comandos

Além dos comandos individuais, o projeto também inclui um pequeno shell personalizado, com:

- leitura de comandos a partir da consola;
- criação de processos com `fork`;
- execução com `exec`;
- sincronização com `wait`;
- encerramento com o comando `termina`;
- suporte a `>`, `<` e `|`.

O objetivo não é competir com um shell real, mas entender na prática como esse tipo de ferramenta funciona internamente.

### 3. Execução em container

O projeto também prevê um `Dockerfile` para compilar e executar o interpretador dentro de um container, facilitando testes e portabilidade do ambiente.

### 4. Análise de sistema de ficheiros

O enunciado também inclui uma parte de análise a uma imagem de sistema de ficheiros (`fs.img`). Essa etapa complementa o projeto com uma componente mais investigativa, voltada para estrutura de filesystem e inspeção de conteúdo.

## Objetivos técnicos

Este projeto serve principalmente para praticar:

- `open`, `read`, `write`, `close`;
- `dup` e `dup2`;
- `pipe`;
- `stat`;
- leitura de diretórios;
- `fork`, `exec` e `wait`;
- tratamento de erros em programas Unix;
- organização modular de código em C.

Também é uma boa oportunidade para exercitar disciplina de projeto em algo relativamente pequeno, mas com complexidade suficiente para exigir estrutura e clareza.

## Estado atual

Neste momento, o repositório está em fase inicial de estruturação.

Já existe uma base para separar:

- comandos;
- interpretador;
- código comum;
- documentação;
- testes e ficheiros de apoio.

Alguns ficheiros ainda estão em modo esqueleto, servindo apenas como ponto de partida para a implementação real.

## Estrutura do repositório

```text
.
├── assets/                 # imagens e capturas de apoio
├── docs/                   # documentação e material de referência
├── include/                # headers
├── src/
│   ├── commands/           # comandos individuais
│   ├── common/             # utilitários partilhados
│   └── interpreter/        # shell personalizado
├── tests/                  # ficheiros de teste e fixtures
├── Dockerfile
├── Makefile
└── README.md
```

## Compilação

Para compilar o projeto:

```bash
make
```

Para limpar os artefactos gerados:

```bash
make clean
```

## Execução

Quando os binários estiverem implementados, a ideia é permitir execuções como estas:

```bash
./bin/mostra ficheiro.txt
./bin/copia origem.txt destino.txt
./bin/lista /tmp
./bin/interpretador
```

Neste momento, parte dos executáveis ainda funciona apenas como placeholder.

## Docker

Build da imagem:

```bash
docker build -t mini-shell-file-system .
```

Execução do container:

```bash
docker run --rm -it mini-shell-file-system
```

## Direção do projeto

Quero que este repositório fique com cara de projeto pessoal técnico, e não apenas de submissão académica. Isso significa:

- README claro e honesto sobre o estado atual;
- código organizado por responsabilidade;
- commits pequenos e rastreáveis;
- documentação do raciocínio técnico;
- evolução visível das funcionalidades.

## Próximos passos

- implementar os comandos de ficheiros um a um;
- construir o interpretador com parsing básico;
- validar redirecionamentos e pipe simples;
- preparar casos de teste;
- documentar decisões e limitações;
- refinar a experiência de execução local e via Docker.

## Referência

Documento base utilizado como referência funcional:

- `docs/tmp/TP-SO-esi-2526.pdf`
