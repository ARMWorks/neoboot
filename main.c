#include "uart.h"

void main(void)
{
    uart_init();
    uart_putc('!');

    while (1);
}