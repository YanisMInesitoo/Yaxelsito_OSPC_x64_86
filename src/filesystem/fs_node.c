// src/filesystem/fs_node.c
#include "fs_node.h"
#include "mem.h"    // Para malloc, free, realloc
#include "string.h" // Para strcpy, strcmp

// Número inicial de hijos para un directorio, se expandirá con realloc
#define INITIAL_CHILDREN_CAPACITY 4

FSNode* fs_create_node(const char* name, FSNodeType type) {
    FSNode* node = (FSNode*)malloc(sizeof(FSNode));
    if (!node) {
        // Manejar error de asignación de memoria
        return NULL;
    }
    memset(node, 0, sizeof(FSNode)); // Limpiar la memoria

    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0'; // Asegurar terminación nula
    node->type = type;
    node->content = NULL;
    node->content_len = 0;
    node->parent = NULL;
    node->children = NULL;
    node->num_children = 0;
    node->max_children = 0;

    if (type == FS_TYPE_DIRECTORY) {
        node->children = (FSNode**)malloc(INITIAL_CHILDREN_CAPACITY * sizeof(FSNode*));
        if (!node->children) {
            free(node); // Falló la asignación de hijos, liberar el nodo padre
            return NULL;
        }
        node->max_children = INITIAL_CHILDREN_CAPACITY;
    }

    return node;
}

void fs_add_child(FSNode* parent, FSNode* child) {
    if (!parent || !child || parent->type != FS_TYPE_DIRECTORY) {
        return; // No es un directorio padre o nodos inválidos
    }

    // Si la capacidad está llena, reasignar más memoria
    if (parent->num_children >= parent->max_children) {
        size_t new_capacity = parent->max_children * 2;
        if (new_capacity == 0) new_capacity = INITIAL_CHILDREN_CAPACITY; // Caso inicial
        FSNode** new_children = (FSNode**)realloc(parent->children, new_capacity * sizeof(FSNode*));
        if (!new_children) {
            // Manejar error de reasignación
            return;
        }
        parent->children = new_children;
        parent->max_children = new_capacity;
    }

    parent->children[parent->num_children++] = child;
    child->parent = parent; // Establecer el padre del hijo
}

FSNode* fs_find_child(FSNode* parent, const char* name) {
    if (!parent || parent->type != FS_TYPE_DIRECTORY || !name) {
        return NULL;
    }

    for (size_t i = 0; i < parent->num_children; ++i) {
        if (strcmp(parent->children[i]->name, name) == 0) {
            return parent->children[i];
        }
    }
    return NULL; // No encontrado
}

void fs_delete_node(FSNode* node) {
    if (!node) return;

    // Si es un directorio, eliminar recursivamente a los hijos
    if (node->type == FS_TYPE_DIRECTORY) {
        for (size_t i = 0; i < node->num_children; ++i) {
            fs_delete_node(node->children[i]); // Eliminar recursivamente
        }
        free(node->children); // Liberar el array de punteros
    }

    // Si es un archivo, liberar su contenido
    if (node->type == FS_TYPE_FILE && node->content) {
        free(node->content);
    }

    // Remover el nodo de su padre (si tiene uno)
    if (node->parent) {
        FSNode* parent = node->parent;
        for (size_t i = 0; i < parent->num_children; ++i) {
            if (parent->children[i] == node) {
                // Mover el último hijo a la posición actual
                parent->children[i] = parent->children[parent->num_children - 1];
                parent->children[parent->num_children - 1] = NULL; // Limpiar el último
                parent->num_children--;
                break;
            }
        }
    }

    free(node); // Liberar el nodo actual
}