// src/kernel/system_state.h
#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include "common.h" // Necesitamos la definición de FSNode

// Estructura para el estado global del sistema
typedef struct {
    FSNode* current_dir_node; // Puntero al nodo del directorio actual (simulado)
    char display_path[MAX_PATH_LEN]; // Ruta actual para mostrar en el prompt
    char username[32]; // Nombre de usuario actual
    char hostname[32]; // Nombre del equipo
    char domain[32];   // Nombre del dominio (puede estar vacío)
} SystemState;

extern SystemState g_sys_state; // Variable global del estado del sistema

void init_system_state();

#endif // SYSTEM_STATE_H