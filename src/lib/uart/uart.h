#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
uint32_t uart_write(uint8_t *data, uint32_t len);
void uart_str_send(const char *format, ...);
void uart_print_bytes(uint8_t *data_ptr, uint32_t data_len);

#endif // UART_H
