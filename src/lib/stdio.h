// src/lib/stdio.h
#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h> // Para va_list, va_start, etc.
#include "common.h" // Para size_t (si no está ya incluido por otros)

// Declaraciones de funciones de entrada/salida
int printf(const char* format, ...);
void putc(char c);
char getc();
char* gets(char* buffer, int size); // Modificado para incluir size

#endif // STDIO_H