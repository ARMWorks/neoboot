#include <stdio.h>
#include "uart.h"

void main(void)
{
    uart_init();
    puts("Hello world!");

    while (1);
}