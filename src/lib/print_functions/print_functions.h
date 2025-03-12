#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H

// cSpell:words printc

#if UART_PRINT
#include "uart/uart.h" // IWYU pragma: export

#define PRINT(X) uart_str_send(X"\r\n")
#define PRINTC(X) uart_str_send(X)
#define PRINTF(X, ...) uart_str_send(X"\r\n", __VA_ARGS__)
#define PRINT_HEX(ptr,len) uart_print_bytes(ptr,len)
#define LINE_MARK uart_str_send("%s:%s:%u\r\n", __FILE__, __FUNCTION__, __LINE__)
#else
#define PRINT(X)
#define PRINTC(X)
#define PRINTF(X, ...)
#define PRINT_HEX(ptr,len)
#define LINE_MARK
#endif

#endif // PRINT_FUNCTIONS_H
