#define UART0_BASE (0x01c28000)

volatile unsigned int * const uart0_rbr = (unsigned int *) UART0_BASE + 0x00;
volatile unsigned int * const uart0_thr = (unsigned int *) UART0_BASE + 0x00;
volatile unsigned int * const uart0_lsr = (unsigned int *) UART0_BASE + 0x14;


void uart_init(void)
{
    /* TODO */
}

void uart_putc(char c)
{
    while (*uart0_lsr & 0x20 == 0);
    *uart0_thr = c;
}
void _putchar(char c) __attribute__((weak, alias("uart_putc")));

char uart_getc(void)
{
    while (*uart0_lsr & 0x01 == 0);
    return *uart0_rbr;
}
char  _getchar(void) __attribute__((weak, alias("uart_getc")));

void uart_fifo_flush(void)
{
    while (*uart0_lsr & 0x40 == 0);
}