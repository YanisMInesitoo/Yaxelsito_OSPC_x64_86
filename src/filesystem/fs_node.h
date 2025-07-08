// src/filesystem/fs_node.h
#ifndef FS_NODE_H
#define FS_NODE_H

#include "common.h" // Para uint8_t, size_t, etc.

// Tipos de nodo del sistema de archivos
typedef enum {
    FS_TYPE_FILE,
    FS_TYPE_DIRECTORY
} FSNodeType;

// Estructura para un nodo del sistema de archivos (archivo o directorio)
typedef struct FSNode {
    char name[64];              // Nombre del archivo/directorio
    FSNodeType type;            // Tipo: archivo o directorio
    void* content;              // Puntero al contenido del archivo (si es FILE)
    size_t content_len;         // Longitud del contenido del archivo
    struct FSNode** children;   // Array de punteros a nodos hijos (si es DIRECTORY)
    size_t num_children;        // Número actual de hijos
    size_t max_children;        // Capacidad máxima de hijos (para realloc)
    struct FSNode* parent;      // Puntero al nodo padre
} FSNode;

// Funciones para manejar FSNode
FSNode* fs_create_node(const char* name, FSNodeType type);
void fs_add_child(FSNode* parent, FSNode* child);
FSNode* fs_find_child(FSNode* parent, const char* name);
void fs_delete_node(FSNode* node); // Borra el nodo y sus hijos (recursivo)

#endif // FS_NODE_H