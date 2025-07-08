// src/kernel/system_state.c
#include "system_state.h"
#include "string.h" // Para strcpy, memset

SystemState g_sys_state; // Definición de la variable global

void init_system_state() {
    // Inicializamos un nodo FSNode dummy para current_dir_node
    // En un sistema real, esto apuntaría al nodo raíz '/'
    static FSNode root_node;
    strcpy(root_node.name, "/"); // Nombre del directorio raíz
    g_sys_state.current_dir_node = &root_node;

    memset(g_sys_state.display_path, 0, MAX_PATH_LEN);
    strcpy(g_sys_state.display_path, "/"); // Ruta inicial

    strcpy(g_sys_state.username, "guest"); // Nombre de usuario por defecto
    strcpy(g_sys_state.hostname, "yaxelsitopc"); // Nombre del equipo por defecto
    strcpy(g_sys_state.domain, ""); // Sin dominio por defecto
}