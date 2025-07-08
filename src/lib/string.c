// src/lib/string.c
#include "string.h"
#include "stdio.h" // Para debug, si es necesario

// Implementación de strlen
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Implementación de strcmp
int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

// Implementación de strcpy
char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

// Implementación de strncpy
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// Implementación de strcat
char* strcat(char* dest, const char* src) {
    char* original_dest = dest;
    while (*dest != '\0') {
        dest++;
    }
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

// Implementación de strchr
char* strchr(const char* str, int character) {
    while (*str != '\0') {
        if (*str == (char)character) {
            return (char*)str;
        }
        str++;
    }
    if ((char)character == '\0') {
        return (char*)str; // Si se busca el nulo, devuelve el puntero al nulo final
    }
    return NULL;
}

// Implementación de strtok (simple, no reentrante)
static char* strtok_next = NULL;

char* strtok(char* str, const char* delim) {
    if (str != NULL) {
        strtok_next = str;
    }
    if (strtok_next == NULL || *strtok_next == '\0') {
        return NULL;
    }

    // Saltar delimitadores iniciales
    while (*strtok_next != '\0' && strchr(delim, *strtok_next) != NULL) {
        strtok_next++;
    }
    if (*strtok_next == '\0') { // Solo delimitadores al final
        return NULL;
    }

    char* token_start = strtok_next;

    // Buscar el siguiente delimitador
    while (*strtok_next != '\0' && strchr(delim, *strtok_next) == NULL) {
        strtok_next++;
    }

    if (*strtok_next != '\0') {
        *strtok_next = '\0'; // Nulificar el delimitador
        strtok_next++;
    }
    return token_start;
}


// Implementación de memset
void* memset(void* s, int c, size_t n) {
    unsigned char* p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

// Invertir una cadena (helper para itoa)
static void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

// Implementación de itoa
char* itoa(int value, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    while (value != 0) {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

// Implementación de tolower
int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Implementación de strlwr
void strlwr(char* str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}