// src/lib/string.h
#ifndef STRING_H
#define STRING_H

#include <stddef.h> // Para size_t

// Funciones de manipulación de cadenas
size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
char* strchr(const char* str, int character);
char* strtok(char* str, const char* delim);
void* memset(void* s, int c, size_t n);

// Conversiones
char* itoa(int value, char* str, int base); // Integer to ASCII

// Nuevas funciones para manejo de minúsculas
int tolower(int c); // Convierte un caracter a minúscula
void strlwr(char* str); // Convierte una cadena a minúscula

#endif // STRING_H