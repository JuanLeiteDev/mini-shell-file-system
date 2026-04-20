#include "../include/utils.h"

int main(int argc, char *argv[]){

    if(argc > 1){ escrevaErro("Uso: clear"); return 1; }
    limpaTela();
    return 0;
}