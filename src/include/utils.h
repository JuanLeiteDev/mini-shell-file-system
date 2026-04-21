#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#define TAMANHO_BUFFER 1024
#define TAMANHO_CONTADOR 10

typedef struct Contador {
    int arr[TAMANHO_CONTADOR];
    int qtd;
} Contador;

void escreva(char msg[]);

void escrevaErro(char msg[]);

int naoExiste(int fd, char nomeFicheiro[]);

void concatenarString(char s1[], char s2[]);

void converteInt(unsigned int num, char *buffer, int *total_bytes);

void converteLong(unsigned long num, char *buffer, int *total_bytes);

int strIgual(char s1[], char s2[]);

int procurarNomeUser(int fd, char userID[], char userNome[]);

void limpaTela();
