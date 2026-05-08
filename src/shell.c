#include "shell.h"
#include "pre.h"
#include "std.h" // IWYU pragma: keep
#include "uart.h"

static u8 cstr_eq(char *a, char *b)
{
    for (; *a; ++a)
        if (*a == *b)
            ++b;
        else
            return 0;
    return 1;
}

static void read_echo_cmd(char *cmd, usize len)
{
    ++len;
    for (char ch = uart_getc(&uart0), *c = cmd; --len;
         ch = uart_getc(&uart0)) {
        *c = ch;

        if (ch == '\r' || ch == '\n') {
            *c = '\0'; // end string

            break;
        } else {
            uart_putc(uart0, ch);
        }

        ++c;
    } // per cmd loop
    uart_puts(uart0, "\r\n");
}

noreturn void shell(void)
{
    uart_puts(uart0, "\r\n\r\nWelcome to Hell (H[ans' Sh]ell)!\r\n");
    char cmd[125];
    do { // repl loop
        uart_puts(uart0, "> ");
        read_echo_cmd(cmd, 4); // per cmd loop

        // uart_puts(uart0, "echo: ");
        // uart_puts(uart0, cmd);
        // uart_puts(uart0, "\r\n");

        if (cstr_eq(cmd, "exec"))
            uart_puts(uart0, "lmao!\r\n");

    } while (1); // repl loop
}
