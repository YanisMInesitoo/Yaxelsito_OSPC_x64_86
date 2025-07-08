// src/shell/shell.c
#include "shell.h"
#include "stdio.h"
#include "string.h"
#include "commands.h"
#include "keyboard.h" // Para getc, gets
#include "system_state.h" // Para g_sys_state

#define COMMAND_BUFFER_SIZE 256

// Función para dibujar el prompt
void draw_prompt() {
    // Si hay un dominio configurado, lo mostramos; si no, solo el nombre del equipo.
    if (strlen(g_sys_state.domain) > 0) {
        printf("%s@%s \u2260 %s> ", g_sys_state.username, g_sys_state.hostname, g_sys_state.domain);
    } else {
        printf("%s@%s> ", g_sys_state.username, g_sys_state.hostname);
    }
}

// Bucle principal de la shell
void run_shell() {
    char command_buffer[COMMAND_BUFFER_SIZE];
    vga_clear(); // Limpiar la pantalla al iniciar la shell
    printf("Bienvenido a Yaxelsito OS PC!\n");
    printf("Escribe 'help' para ver los comandos disponibles.\n");

    while (1) {
        draw_prompt(); // Dibuja el prompt
        gets(command_buffer, COMMAND_BUFFER_SIZE); // Lee el comando del usuario
        printf("\n"); // Nueva línea después de la entrada
        process_command(command_buffer); // Procesa el comando
    }
}