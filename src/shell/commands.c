// src/shell/commands.c
#include "commands.h"
#include "stdio.h"      // Para printf, gets
#include "string.h"     // Para strcmp, strtok, strlen, strcpy, strlwr
#include "vga.h"        // Para vga_clear
#include "system_state.h" // Para acceder y modificar g_sys_state

// --- Implementación de Funciones de Comandos ---

// Comando HELP
void cmd_help(const char* args) {
    printf("Comandos disponibles:\n");
    for (int i = 0; command_list[i].name != NULL; ++i) {
        printf("  %s: %s\n", command_list[i].name, command_list[i].description);
    }
}

// Comando ECHO
void cmd_echo(const char* args) {
    if (args && strlen(args) > 0) {
        printf("%s\n", args);
    } else {
        printf("\n");
    }
}

// Comando CLEAR (o CLS)
void cmd_clear(const char* args) {
    vga_clear();
}
void cmd_cls(const char* args) {
    vga_clear(); // 'cls' es un alias de 'clear'
}

// Comando REBOOT (simulado)
void cmd_reboot(const char* args) {
    printf("Reiniciando sistema...\n");
    printf("Este es un entorno simulado. Para reiniciar realmente, tendrias que reiniciar tu emulador.\n");
    // En un OS real, aquí iría un reinicio de hardware (ej. 0xFE al puerto 0x64 para un reset de CPU)
    while(1); // Bucle infinito para simular el "apagado"
}

// Comando SHUTDOWN (simulado)
void cmd_shutdown(const char* args) {
    printf("Apagando sistema...\n");
    printf("Este es un entorno simulado. Puedes cerrar tu emulador.\n");
    // En un OS real, aquí iría un apagado ACPI o Power Off
    while(1); // Bucle infinito para simular el "apagado"
}

// Comando VERSION
void cmd_version(const char* args) {
    printf("Yaxelsito OS PC v0.1.0-alpha\n");
    printf("Fecha de compilacion: %s %s\n", __DATE__, __TIME__);
}

// Comando ABOUT
void cmd_about(const char* args) {
    printf("Yaxelsito OS PC es un sistema operativo didactico y de aprendizaje.\n");
    printf("Desarrollado con fines educativos y de exploracion del kernel.\n");
    printf("Creado por Yaxel.\n");
}

// Comando LOGOFF (simulado)
void cmd_logoff(const char* args) {
    printf("Cerrando sesion de '%s'...\n", g_sys_state.username);
    strcpy(g_sys_state.username, "guest"); // Volver al usuario por defecto
    // En un OS real, esto podría implicar limpiar más estado o regresar a una pantalla de login.
    printf("Sesion cerrada. Usuario actual: '%s'.\n", g_sys_state.username);
}

// --- Comandos de manejo de sistema de archivos (simulados) ---

// Comando LS (simulado)
void cmd_ls(const char* args) {
    printf("Listado de archivos/directorios en %s (simulado):\n", g_sys_state.display_path);
    printf("  <DIR> . \n");
    printf("  <DIR> ..\n");
    printf("  <DIR> users\n");
    printf("  <DIR> system32\n");
    printf("  <DIR> programs\n");
    printf("        README.txt         1024 bytes\n");
    printf("        config.ini         256 bytes\n");
}

// Comando MKDIR (simulado)
void cmd_mkdir(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: mkdir <nombre_directorio>\n");
        return;
    }
    printf("Creando directorio '%s' en %s (simulado).\n", args, g_sys_state.display_path);
    // Lógica real: crear un nuevo nodo de directorio en el sistema de archivos.
}

// Comando CD (simulado)
void cmd_cd(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Directorio actual: %s\n", g_sys_state.display_path);
        return;
    }

    if (strcmp(args, "..") == 0) {
        // Simplemente ir al raíz para simplificar. Lógica real sería parsear la ruta.
        if (strcmp(g_sys_state.display_path, "/") != 0) {
            printf("Cambiando a directorio padre (simulado).\n");
            strcpy(g_sys_state.display_path, "/"); // Regresa a raíz
        } else {
            printf("Ya estas en el directorio raiz.\n");
        }
    } else if (strcmp(args, "/") == 0) {
        printf("Cambiando a directorio raiz (simulado).\n");
        strcpy(g_sys_state.display_path, "/");
    } else {
        // Aquí se simula el cambio, en un sistema real se verificaría si el directorio existe.
        printf("Cambiando a directorio '%s' (simulado).\n", args);
        // Concatenar a la ruta actual si no es la raíz, o reemplazar si es una ruta absoluta.
        if (strcmp(g_sys_state.display_path, "/") != 0) {
            strcat(g_sys_state.display_path, "/");
        }
        strcat(g_sys_state.display_path, args);
        // Asegurarse de que no exceda el MAX_PATH_LEN
        g_sys_state.display_path[MAX_PATH_LEN - 1] = '\0';
    }
}

// Comando TOUCH (simulado)
void cmd_touch(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: touch <nombre_archivo>\n");
        return;
    }
    printf("Creando archivo '%s' en %s (simulado).\n", args, g_sys_state.display_path);
    // Lógica real: crear un archivo vacío o actualizar su timestamp.
}

// Comando RM (simulado)
void cmd_rm(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: rm <nombre_archivo_o_directorio>\n");
        return;
    }
    printf("Eliminando '%s' de %s (simulado).\n", args, g_sys_state.display_path);
    // Lógica real: eliminar el archivo o directorio (con manejo de no-vacío para directorios).
}

// Comando CAT (simulado)
void cmd_cat(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: cat <nombre_archivo>\n");
        return;
    }
    printf("Contenido de '%s' (simulado):\n", args);
    if (strcmp(args, "README.txt") == 0) {
        printf("Bienvenido a Yaxelsito OS PC!\n");
        printf("Este es un sistema operativo en desarrollo.\n");
        printf("Explora los comandos y diviertete!\n");
    } else {
        printf("Este es un archivo de texto de ejemplo.\n");
        printf("Linea 2 de contenido simulado.\n");
    }
}

// Comando WRITE (simulado)
void cmd_write(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: write <nombre_archivo>\n");
        return;
    }
    printf("Abriendo '%s' para escritura (simulado).\n", args);
    printf("Escribe tu texto y presiona Enter. No se guardara realmente.\n");
    char line_buffer[MAX_COMMAND_LEN];
    while(1) {
        printf(">> ");
        gets(line_buffer, MAX_COMMAND_LEN);
        if (strcmp(line_buffer, "exit") == 0) { // Un comando simple para salir de la escritura
            break;
        }
        printf("  (Simulando escritura: '%s')\n", line_buffer);
    }
    printf("Cerrando archivo '%s' (simulado).\n", args);
}

// --- Comandos de información del sistema (simulados) ---

// Comando MEMINFO (simulado)
void cmd_meminfo(const char* args) {
    printf("Informacion de memoria (simulada):\n");
    printf("  Memoria Fisica Total: 4096 MB\n");
    printf("  Memoria Fisica Usada:  512 MB (Kernel + Drivers + Shell)\n");
    printf("  Memoria Fisica Libre: 3584 MB\n");
    printf("  Memoria Swap Total:   0 MB\n");
    printf("  Memoria Swap Usada:   0 MB\n");
}

// Comando CPUINFO (simulado)
void cmd_cpuinfo(const char* args) {
    printf("Informacion de CPU (simulada):\n");
    printf("  Modelo: QEMU Virtual CPU version 2.5+\n");
    printf("  Arquitectura: x86_64 (compatibilidad i386)\n");
    printf("  Nucleos: 1 (Configurable en QEMU)\n");
    printf("  Frecuencia Base: 2.0 GHz\n");
    printf("  Cache L1: 32 KB\n");
    printf("  Cache L2: 256 KB\n");
    printf("  Caracteristicas: FPU, VME, PSE, TSC, MSR, PAE, APIC, MTRR, CMOV, PGE, FXSR, SSE, SSE2\n");
}

// Comando DATE (simulado)
void cmd_date(const char* args) {
    printf("Fecha actual del sistema (simulada): Martes, 09 de Julio de 2025\n");
    printf("Puedes cambiarla con 'date MM-DD-YYYY' (no implementado).\n");
}

// Comando TIME (simulado)
void cmd_time(const char* args) {
    printf("Hora actual del sistema (simulada): 13:25:00 CEST\n");
    printf("Puedes cambiarla con 'time HH:MM:SS' (no implementado).\n");
}

// Comando HOSTNAME (muestra el nombre del equipo actual)
void cmd_hostname(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Nombre del equipo: %s\n", g_sys_state.hostname);
        if (strlen(g_sys_state.domain) > 0) {
            printf("Dominio: %s\n", g_sys_state.domain);
        }
    } else {
        printf("Uso: hostname (para mostrar), use 'sethostname' para cambiarlo.\n");
    }
}

// Comando IRQINFO
void cmd_irqinfo(const char* args) {
    printf("Informacion de IRQs (simulada):\n");
    printf("  IRQ 0 (Timer):       Activo - Sistema Tick\n");
    printf("  IRQ 1 (Keyboard):    Activo - Entrada de teclado\n");
    printf("  IRQ 2 (PIC Cascade): Ocupado - Enlace PIC maestro/esclavo\n");
    printf("  IRQ 3 (COM2/4):      Inactivo\n");
    printf("  IRQ 4 (COM1/3):      Inactivo\n");
    printf("  IRQ 5 (LPT2):        Inactivo\n");
    printf("  IRQ 6 (Floppy):      Inactivo\n");
    printf("  IRQ 7 (LPT1):        Inactivo\n");
    printf("  IRQ 8 (RTC):         Activo - Reloj de tiempo real\n");
    printf("  IRQ 9 (ACPI/PCI):    Activo - Redireccion de IRQ2\n");
    printf("  IRQ 10 (Abierto):    Inactivo\n");
    printf("  IRQ 11 (Abierto):    Inactivo\n");
    printf("  IRQ 12 (PS/2 Mouse): Inactivo\n"); // Activo si hay mouse
    printf("  IRQ 13 (FPU):        Activo - Coprocesador matematico\n");
    printf("  IRQ 14 (Primary ATA):Activo - Disco duro/CD-ROM Primario\n");
    printf("  IRQ 15 (Secondary ATA):Inactivo - Disco duro/CD-ROM Secundario\n");
}

// Comando PCIINFO
void cmd_pciinfo(const char* args) {
    printf("Dispositivos PCI detectados (simulado):\n");
    printf("  Slot: 00:00.0, ID: 8086:1237, Clase: 0600 (Host bridge) - Intel 440FX\n");
    printf("  Slot: 00:01.0, ID: 8086:7000, Clase: 0601 (ISA bridge) - Intel PIIX3\n");
    printf("  Slot: 00:01.1, ID: 8086:7010, Clase: 0101 (IDE interface) - Intel PIIX3 IDE\n");
    printf("  Slot: 00:02.0, ID: 1013:00B8, Clase: 0300 (VGA controller) - QEMU VGA\n");
    printf("  Slot: 00:03.0, ID: 1AF4:1000, Clase: 0200 (Ethernet controller) - Virtio network device\n");
    printf("  Slot: 00:04.0, ID: 1AF4:1001, Clase: 0100 (SCSI controller) - Virtio SCSI\n");
}

// Comando GETTEMP
void cmd_gettemp(const char* args) {
    printf("Temperaturas del sistema (simulado):\n");
    printf("  CPU: 45 C (Normal)\n");
    printf("  GPU: 50 C (N/A, simulado)\n");
    printf("  Placa Base: 38 C (Normal)\n");
    printf("  HDD: 30 C (N/A, simulado)\n");
}

// Comando REGISTER (simulado, para ver info de registros de CPU)
void cmd_register(const char* args) {
    printf("Valores de registros de CPU (simulado):\n");
    printf("  EAX: 0x00000123  EBX: 0x00004567\n");
    printf("  ECX: 0x000089AB  EDX: 0x0000CDEF\n");
    printf("  ESI: 0x00001122  EDI: 0x00003344\n");
    printf("  EBP: 0x0000A000  ESP: 0x0000B000\n");
    printf("  EIP: 0x00100000  EFLAGS: 0x00000246 (IF, AF, PF)\n");
    printf("  CS: 0x08  DS: 0x10  SS: 0x10  ES: 0x10  FS: 0x00  GS: 0x00\n");
}

// --- Comandos de red (simulados) ---

// Comando ROUTE PRINT
void cmd_route(const char* args) {
    if (args && strcmp(args, "print") == 0) {
        printf("Tabla de enrutamiento IPv4 (simulada):\n");
        printf("  Destino de red       Mascara de red      Gateway          Interfaz       Metrica\n");
        printf("  0.0.0.0            0.0.0.0           192.168.1.1      192.168.1.100  20\n");
        printf("  127.0.0.0          255.0.0.0         Enlace           127.0.0.1      1\n");
        printf("  192.168.1.0        255.255.255.0     Enlace           192.168.1.100  20\n");
        printf("\n");
    } else {
        printf("Uso: route print (simulado)\n");
    }
}

// Comando NET VIEW
void cmd_netview(const char* args) {
    printf("Recursos compartidos en la red (simulado):\n");
    printf("  \\\\YP-SERVER\\SHAREDOCS        Archivos compartidos del servidor\n");
    printf("  \\\\YP-PRINTER\\EPSON_XP        Impresora en red\n");
    printf("  \\\\YP-DESKTOP\\PUBLIC          Carpeta publica de otro PC\n");
    printf("\n");
}

// Comando ARP -A
void cmd_arp(const char* args) {
    if (args && strcmp(args, "-a") == 0) {
        printf("Tabla ARP (simulada):\n");
        printf("  Interfaz: 192.168.1.100 --- 0x0\n");
        printf("    Direccion de Internet    Direccion Fisica      Tipo\n");
        printf("    192.168.1.1              00-50-56-C0-00-01     dinamico\n");
        printf("    192.168.1.254            00-1B-44-11-22-33     dinamico\n");
        printf("    192.168.1.5              00-0C-29-AB-CD-EF     estatico\n");
        printf("\n");
    } else {
        printf("Uso: arp -a (simulado)\n");
    }
}

// --- Comandos de utilidades y configuración (simulados) ---

// Comando SETUSER (modifica el nombre de usuario del prompt)
void cmd_setuser(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: setuser <nombre_de_usuario>\n");
        return;
    }
    if (strlen(args) >= sizeof(g_sys_state.username)) {
        printf("Error: Nombre de usuario demasiado largo (max %d caracteres).\n", (int)sizeof(g_sys_state.username) - 1);
        return;
    }
    strcpy(g_sys_state.username, args);
    printf("Nombre de usuario del prompt cambiado a '%s'.\n", g_sys_state.username);
}

// Comando SETHOSTNAME (modifica el nombre del equipo del prompt)
void cmd_sethostname(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: sethostname <nombre_de_equipo>\n");
        return;
    }
    if (strlen(args) >= sizeof(g_sys_state.hostname)) {
        printf("Error: Nombre de equipo demasiado largo (max %d caracteres).\n", (int)sizeof(g_sys_state.hostname) - 1);
        return;
    }
    strcpy(g_sys_state.hostname, args);
    printf("Nombre del equipo del prompt cambiado a '%s'.\n", g_sys_state.hostname);
}

// Comando SETDOMAIN (modifica el dominio del prompt)
void cmd_setdomain(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: setdomain [nombre_de_dominio|none]\n");
        return;
    }

    if (strcmp(args, "none") == 0) {
        strcpy(g_sys_state.domain, ""); // Eliminar el dominio
        printf("Dominio eliminado del prompt.\n");
    } else {
        if (strlen(args) >= sizeof(g_sys_state.domain)) {
            printf("Error: Nombre de dominio demasiado largo (max %d caracteres).\n", (int)sizeof(g_sys_state.domain) - 1);
            return;
        }
        strcpy(g_sys_state.domain, args);
        printf("Dominio del prompt establecido a '%s'.\n", g_sys_state.domain);
    }
}

// Comando VERIFY
void cmd_verify(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Verificacion de escritura: ON (simulado).\n");
        printf("Uso: verify [on|off]\n");
    } else if (strcmp(args, "on") == 0) {
        printf("Verificacion de escritura activada (simulado).\n");
    } else if (strcmp(args, "off") == 0) {
        printf("Verificacion de escritura desactivada (simulado).\n");
    } else {
        printf("Parametro invalido. Uso: verify [on|off]\n");
    }
}

// Comando VOL
void cmd_vol(const char* args) {
    printf("Volumen en la unidad C: es YAXELSITO_OS\n");
    printf("Numero de serie del volumen: 1234-ABCD\n");
}

// Comando BENCHMARK
void cmd_benchmark(const char* args) {
    printf("Iniciando pruebas de rendimiento del sistema (simulado)...\n");
    // Simulación de cálculos intensivos
    for (volatile int i = 0; i < 50000000; i++); // Pequeña espera para simular trabajo

    printf("  CPU (Integer Ops): 1.5 MIPS (Millones de Instrucciones por Segundo)\n");
    printf("  Memoria (Lectura): 120 MB/s\n");
    printf("  Memoria (Escritura): 90 MB/s\n");
    printf("  E/S Disco (Secuencial): 50 MB/s\n");
    printf("  E/S Disco (Aleatorio): 5 MB/s\n");
    printf("Pruebas de rendimiento completadas.\n");
}

// Comando HEXDUMP (simulado)
void cmd_hexdump(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Uso: hexdump <nombre_archivo_o_direccion_memoria>\n");
        return;
    }
    printf("Volcado hexadecimal de '%s' (simulado):\n", args);
    printf("00000000: 48 65 6C 6C 6F 2C 20 57  6F 72 6C 64 21 0A 4C 69  Hello, World!.Li\n");
    printf("00000010: 6E 65 61 20 64 65 20 65  6A 65 6D 70 6C 6F 2E 0A  nea de ejemplo..\n");
    printf("00000020: 59 61 78 65 6C 73 69 74  6F 20 4F 53 20 50 43 21  Yaxelsito OS PC!\n");
    // Puedes añadir más líneas o simular diferentes contenidos
}

// Comando DEBUG
void cmd_debug(const char* args) {
    if (!args || strlen(args) == 0) {
        printf("Estado de depuracion: ON (simulado)\n");
        printf("Uso: debug [on|off]\n");
    } else if (strcmp(args, "on") == 0) {
        printf("Modo depuracion activado (simulado). Se mostraran mensajes de bajo nivel.\n");
    } else if (strcmp(args, "off") == 0) {
        printf("Modo depuracion desactivado (simulado).\n");
    } else {
        printf("Parametro invalido. Uso: debug [on|off]\n");
    }
}


// --- Tabla de Comandos Disponibles ---
// Esta tabla mapea el nombre del comando a su función y descripción.
static Command command_list[] = {
    // Comandos básicos
    {"help", cmd_help, "Muestra esta ayuda."},
    {"echo", cmd_echo, "Muestra un mensaje. Uso: echo <mensaje>"},
    {"clear", cmd_clear, "Limpia la pantalla."},
    {"cls", cmd_cls, "Limpia la pantalla (alias de 'clear')."},
    {"reboot", cmd_reboot, "Reinicia el sistema (simulado)."},
    {"shutdown", cmd_shutdown, "Apaga el sistema (simulado)."},
    {"version", cmd_version, "Muestra la version del sistema operativo."},
    {"about", cmd_about, "Informacion sobre Yaxelsito OS PC."},
    {"logoff", cmd_logoff, "Cierra la sesion del usuario actual (simulado)."},

    // Comandos de manejo de sistema de archivos (simulados)
    {"ls", cmd_ls, "Lista los archivos y directorios (simulado)."},
    {"mkdir", cmd_mkdir, "Crea un nuevo directorio (simulado). Uso: mkdir <nombre>"},
    {"cd", cmd_cd, "Cambia el directorio actual (simulado). Uso: cd <ruta>"},
    {"touch", cmd_touch, "Crea un archivo vacio o actualiza su timestamp (simulado). Uso: touch <archivo>"},
    {"rm", cmd_rm, "Elimina un archivo o directorio (simulado). Uso: rm <nombre>"},
    {"cat", cmd_cat, "Muestra el contenido de un archivo (simulado). Uso: cat <archivo>"},
    {"write", cmd_write, "Abre un archivo para escritura de texto (simulado). Uso: write <archivo>"},

    // Comandos de información del sistema (simulados)
    {"meminfo", cmd_meminfo, "Muestra informacion de la memoria del sistema (simulado)."},
    {"cpuinfo", cmd_cpuinfo, "Muestra informacion de la CPU (simulado)."},
    {"date", cmd_date, "Muestra la fecha actual del sistema (simulado)."},
    {"time", cmd_time, "Muestra la hora actual del sistema (simulado)."},
    {"hostname", cmd_hostname, "Muestra el nombre del equipo y dominio actual."},
    {"irqinfo", cmd_irqinfo, "Muestra informacion de las IRQs (simulado)."},
    {"pciinfo", cmd_pciinfo, "Muestra informacion de dispositivos PCI (simulado)."},
    {"gettemp", cmd_gettemp, "Muestra las temperaturas del sistema (simulado)."},
    {"register", cmd_register, "Muestra los valores de registros de la CPU (simulado)."},

    // Comandos de red (simulados)
    {"route", cmd_route, "Muestra la tabla de enrutamiento (simulado). Uso: route print"},
    {"netview", cmd_netview, "Muestra recursos compartidos en la red (simulado)."},
    {"arp", cmd_arp, "Muestra la tabla ARP (simulado). Uso: arp -a"},

    // Comandos de utilidades y configuración (simulados)
    {"setuser", cmd_setuser, "Establece el nombre de usuario del prompt. Uso: setuser <nombre>"},
    {"sethostname", cmd_sethostname, "Establece el nombre del equipo del prompt. Uso: sethostname <nombre>"},
    {"setdomain", cmd_setdomain, "Establece el dominio del prompt. Uso: setdomain <nombre> | none"},
    {"verify", cmd_verify, "Activa/desactiva la verificacion de escritura (simulado). Uso: verify [on|off]"},
    {"vol", cmd_vol, "Muestra la etiqueta y numero de serie del volumen (simulado)."},
    {"benchmark", cmd_benchmark, "Ejecuta pruebas de rendimiento del sistema (simulado)."},
    {"hexdump", cmd_hexdump, "Realiza un volcado hexadecimal de un recurso (simulado). Uso: hexdump <recurso>"},
    {"debug", cmd_debug, "Activa/desactiva el modo depuracion (simulado). Uso: debug [on|off]"},

    {NULL, NULL, NULL} // Marcador de fin de la lista de comandos
};


// Función principal para procesar un comando ingresado por el usuario.
void process_command(const char* input) {
    char command_copy[MAX_COMMAND_LEN];
    // Asegurarse de no desbordar el buffer y terminar con nulo.
    strncpy(command_copy, input, MAX_COMMAND_LEN - 1);
    command_copy[MAX_COMMAND_LEN - 1] = '\0';

    strlwr(command_copy); // Convertir todo a minúsculas para coincidencia sin distinción de mayúsculas/minúsculas

    char* cmd_name_token = strtok(command_copy, " "); // Extraer el nombre del comando
    char* args_token = strtok(NULL, "");             // El resto de la cadena son los argumentos

    if (cmd_name_token == NULL || strlen(cmd_name_token) == 0) {
        return; // No se ingresó ningún comando.
    }

    // Iterar sobre la lista de comandos para encontrar una coincidencia.
    for (int i = 0; command_list[i].name != NULL; ++i) {
        if (strcmp(command_list[i].name, cmd_name_token) == 0) {
            // Ejecutar la función asociada al comando.
            command_list[i].function(args_token);
            return;
        }
    }

    // Si no se encuentra el comando, imprimir un mensaje de error.
    printf("Error: Comando '%s' no reconocido. Escribe 'help' para ver los comandos disponibles.\n", cmd_name_token);
}