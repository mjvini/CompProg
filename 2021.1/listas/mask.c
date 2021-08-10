#include <stdio.h>
#include <limits.h>

int main(){

    unsigned long int mask = LONG_MAX;  //LONG_MAX é o maior valor de um signed long int (apenas MSB desligado)

    mask = ~(mask); //inverte todos os bits (agora apenas o MSB está ligado)

    printf("máscara = 0x%lx \n", mask);

}