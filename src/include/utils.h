#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TAMANHO_BUFFER 1024

void escrevaErro(const char msg[]);

int naoExiste(const int fd);
