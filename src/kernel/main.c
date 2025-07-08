// src/kernel/main.c
#include "common.h"
#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "timer.h"
#include "keyboard.h"
#include "mem.h"
#include "fs.h"
#include "vesa.h"
#include "shell.h"
#include "system_state.h" // Para g_sys_state

// Punto de entrada principal del kernel (llamado desde loader.asm)
void kernel_main() {
    // 1. Inicialización de la consola VGA básica (modo texto)
    //    Si ya estás en modo gráfico VESA por defecto, esto no sería necesario
    //    Pero para depuración inicial, es útil.
    //    Por simplicidad, en este ejemplo asumimos una terminal de texto inicial.
    printf("Initializing Yaxelsito OS PC (x86-64)...\n");

    // 2. Inicialización de la GDT (para Protected Mode y Long Mode)
    //    Necesario para que el CPU sepa cómo interpretar los segmentos
    init_gdt();
    printf("GDT Initialized.\n");

    // 3. Inicialización de la IDT, ISRs e IRQs
    //    Esto configura cómo el kernel maneja interrupciones y excepciones (teclado, timer, etc.)
    init_idt();
    init_isr();
    init_irq();
    printf("Interrupts Initialized.\n");

    // 4. Habilitar Interrupciones (muy importante)
    asm volatile("sti");
    printf("Interrupts Enabled.\n");

    // 5. Inicialización del Temporizador
    init_timer(100); // Frecuencia de 100 Hz
    printf("Timer Initialized.\n");

    // 6. Inicialización del Teclado
    init_keyboard();
    printf("Keyboard Initialized.\n");

    // 7. Inicialización del Gestor de Memoria Heap
    //    Aquí necesitarás definir el tamaño del heap y dónde comienza.
    //    Se usan las variables del linker script (__kernel_heap_start, __kernel_end)
    extern uint64_t __kernel_heap_start;
    // Asumimos un heap de 4MB después del kernel. Ajusta según tus necesidades.
    uint64_t heap_size = 4 * 1024 * 1024;
    init_mem_manager((void*)&__kernel_heap_start, heap_size);
    printf("Heap Memory Manager Initialized.\n");

    // 8. Inicialización del Sistema de Archivos (RAM Disk)
    fs_init(); // Crea toda la estructura de directorios en memoria
    printf("File System (RAM Disk) Initialized.\n");

    // 9. Inicialización del Subsistema Gráfico VESA
    //    Por ahora, solo inicializamos la capacidad VESA sin cambiar el modo,
    //    asumiendo que la terminal inicial es texto. Los comandos gráficos
    //    cambiarán al modo VESA real cuando sea necesario.
    //    Si quieres que arranque en gráfico, puedes llamar a vesa_init_graphics_mode(WIDTH, HEIGHT, BPP) aquí.
    printf("Graphics Subsystem Initialized (VESA detection pending).\n");

    // 10. Inicialización del Estado Global del Sistema
    //     Establece el directorio actual a la raíz de C:
    init_system_state(); // Establece usuario, hostname.
    g_sys_state.current_dir_node = fs_get_root(); // C:\
    fs_get_full_path(g_sys_state.current_dir_node, g_sys_state.display_path, MAX_PATH_LEN);
    printf("System State Initialized. Current path: %s\n", g_sys_state.display_path);

    // Mensaje de bienvenida
    printf("\nWelcome to Yaxelsito OS PC (x86-64)!\n");
    printf("Type 'help' for a list of commands.\n\n");

    // 11. Iniciar la Shell
    run_shell();

    // El kernel nunca debería llegar aquí
    while (1) {
        asm volatile("hlt"); // Halts the CPU until the next interrupt
    }
}