// src/lib/common.h
#ifndef COMMON_H
#define COMMON_H

#include <stddef.h> // Para size_t

// Definiciones de tipos comunes
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Constantes
#define MAX_PATH_LEN 128
#define MAX_FILENAME_LEN 32

// Estructura simplificada para un nodo del sistema de archivos (para el current_dir_node)
// En un sistema real, esto sería mucho más complejo
typedef struct FSNode {
    char name[MAX_FILENAME_LEN];
    // Otros campos como tipo (directorio/archivo), permisos, etc.
} FSNode;

// Funciones de utilidad básicas (si las necesitas globalmente)
void outb(uint116_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

#endif // COMMON_H