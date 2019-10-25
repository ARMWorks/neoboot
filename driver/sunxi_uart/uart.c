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

char uart_getc(void)
{
    while (*uart0_lsr & 0x01 == 0);
    return *uart0_rbr;
}

void uart_fifo_flush(void)
{
    while (*uart0_lsr & 0x40 == 0);
}