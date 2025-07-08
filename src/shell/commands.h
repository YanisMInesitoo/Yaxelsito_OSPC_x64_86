// src/shell/commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

#include "common.h" // Para FSNode

// Estructura para definir un comando
typedef struct {
    const char* name;
    void (*function)(const char* args);
    const char* description;
} Command;

// Función para procesar un comando dado
void process_command(const char* input);

// Declaraciones de funciones de comandos
// Comandos básicos
void cmd_help(const char* args);
void cmd_echo(const char* args);
void cmd_clear(const char* args); // Para 'clear'
void cmd_cls(const char* args);    // Para 'cls' (alias de clear)
void cmd_reboot(const char* args);
void cmd_shutdown(const char* args);
void cmd_version(const char* args);
void cmd_about(const char* args);
void cmd_logoff(const char* args); // Para simular el cierre de sesión

// Comandos de manejo de sistema de archivos (simulados)
void cmd_ls(const char* args);
void cmd_mkdir(const char* args);
void cmd_cd(const char* args);
void cmd_touch(const char* args);
void cmd_rm(const char* args);
void cmd_cat(const char* args);
void cmd_write(const char* args); // Para simular escritura en archivos

// Comandos de información del sistema (simulados)
void cmd_meminfo(const char* args);
void cmd_cpuinfo(const char* args);
void cmd_date(const char* args);
void cmd_time(const char* args);
void cmd_hostname(const char* args); // Muestra el hostname
void cmd_irqinfo(const char* args);
void cmd_pciinfo(const char* args);
void cmd_gettemp(const char* args);
void cmd_register(const char* args); // Para mostrar info de registros CPU

// Comandos de red (simulados)
void cmd_route(const char* args);
void cmd_netview(const char* args);
void cmd_arp(const char* args);

// Comandos de utilidades y configuración (simulados)
void cmd_setuser(const char* args);     // Para cambiar el nombre de usuario del prompt
void cmd_sethostname(const char* args); // Para cambiar el nombre del equipo del prompt
void cmd_setdomain(const char* args);   // Para cambiar el dominio del prompt
void cmd_verify(const char* args);      // Simula verificación de escritura
void cmd_vol(const char* args);         // Muestra info del volumen
void cmd_benchmark(const char* args);   // Simula pruebas de rendimiento
void cmd_hexdump(const char* args);     // Simula volcado hexadecimal
void cmd_debug(const char* args);       // Activa/desactiva modo debug

#endif // COMMANDS_H