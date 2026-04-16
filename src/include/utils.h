#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define TAMANHO_BUFFER 1024

void escrevaErro(char msg[]);

int naoExiste(int fd, char nomeFicheiro[]);

char *concatenarString(char s1[], char s2[]);

void converteInt(int num, char *buffer, int *total_bytes);