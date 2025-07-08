// src/lib/stdio.c
#include "stdio.h"
#include "vga.h"    // Para vga_putc
#include "string.h" // Para strlen, itoa
#include "keyboard.h" // Para keyboard_read_char

// Implementación de putc
void putc(char c) {
    vga_putc(c);
}

// Implementación de getc (lee un carácter del buffer del teclado)
char getc() {
    return keyboard_read_char();
}

// Implementación de gets (lee una linea hasta el \n o buffer lleno)
char* gets(char* buffer, int size) {
    int i = 0;
    char c;

    if (size <= 0) return NULL;

    while (i < size - 1) { // Dejar espacio para el nulo terminador
        c = getc(); // Leer un carácter del teclado (bloqueante)
        
        if (c == '\b') { // Manejar backspace
            if (i > 0) {
                putc('\b'); // Mueve el cursor hacia atrás
                putc(' ');  // Borra el carácter
                putc('\b'); // Mueve el cursor hacia atrás de nuevo
                i--;
            }
            continue;
        }

        putc(c); // Imprimir el carácter que el usuario tipeó

        if (c == '\n' || c == '\r') { // Si es Enter
            buffer[i] = '\0'; // Terminar la cadena
            return buffer;
        }

        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0'; // Asegurarse de que el buffer esté terminado
    return buffer;
}


// Implementación de printf (mejorada)
int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int chars_printed = 0;
    char num_buf[32]; // Suficientemente grande para cualquier numero

    while (*format != '\0') {
        if (*format == '%') {
            format++; // Saltar el '%'
            switch (*format) {
                case 'c':
                    putc((char)va_arg(args, int)); // char se promociona a int en va_arg
                    chars_printed++;
                    break;
                case 's': {
                    char* str = va_arg(args, char*);
                    if (str == NULL) str = "(null)";
                    while (*str != '\0') {
                        putc(*str);
                        str++;
                        chars_printed++;
                    }
                    break;
                }
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    itoa(val, num_buf, 10);
                    char* str = num_buf;
                    while (*str != '\0') {
                        putc(*str);
                        str++;
                        chars_printed++;
                    }
                    break;
                }
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, num_buf, 16); // Base 16 para hexadecimal
                    char* str = num_buf;
                    // Opcional: prefijo "0x"
                    putc('0'); chars_printed++;
                    putc('x'); chars_printed++;
                    while (*str != '\0') {
                        putc(*str);
                        str++;
                        chars_printed++;
                    }
                    break;
                }
                case '%':
                    putc('%');
                    chars_printed++;
                    break;
                default:
                    // Si el especificador no es reconocido, imprimirlo tal cual
                    putc('%');
                    putc(*format);
                    chars_printed += 2;
                    break;
            }
        } else {
            putc(*format);
            chars_printed++;
        }
        format++;
    }

    va_end(args);
    return chars_printed;
}