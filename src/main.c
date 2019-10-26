#include <stdio.h>
#include "uart.h"

void main(void)
{
    uart_init();
    puts("Hello world!");

    printf("sizeof(size_t) = %d\n", sizeof(size_t));

    while (1);
}