// src/drivers/keyboard.c
#include "keyboard.h"
#include "common.h" // Para outb, inb
#include "stdio.h"  // Para putc (en un sistema real, no deberías usar stdio aquí directamente)

// Puertos del controlador de teclado 8042
#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64

// IRQ para el teclado
#define KBD_IRQ 1

// Mapa de códigos de escaneo (muy básico)
// Esto solo maneja un subconjunto muy limitado de teclas para simplicidad
static const char kbd_us_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   // 0-14
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     // 15-28
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',   // 29-43 (Ctrl, a-l, ;, ', `, LShift, \)
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ',           // 44-57 (z-m, ',', ., /, RShift, PrtSc, Alt, Espacio)
    // ... y más teclas, pero para un OS mínimo, esto es suficiente.
    // Solo manejamos teclas presionadas (scancodes < 0x80)
};

// Cola de caracteres del teclado (muy simple, sin concurrencia)
#define KEY_BUFFER_SIZE 256
static char key_buffer[KEY_BUFFER_SIZE];
static int key_buffer_head = 0;
static int key_buffer_tail = 0;

// Inicializa el teclado (solo para configuración básica del PIC si es necesario)
void keyboard_init() {
    // Aquí podrías deshabilitar/habilitar el teclado, configurar LEDs, etc.
    // Por ahora, solo lo preparamos para lectura directa si es necesario.
}

// Lee un carácter del búfer del teclado (bloqueante)
char keyboard_read_char() {
    while (key_buffer_head == key_buffer_tail) {
        // Esperar a que llegue un carácter
        // En un OS real, esto sería un bucle de eventos o se usarían interrupciones
        // Para este ejemplo, simulamos una espera pasiva
    }

    char c = key_buffer[key_buffer_tail];
    key_buffer_tail = (key_buffer_tail + 1) % KEY_BUFFER_SIZE;
    return c;
}

// Manejador de interrupciones del teclado (IRQ1)
// Esto necesita ser registrado con tu controlador de interrupciones
void keyboard_handler() {
    uint8_t scancode;

    // Leer el código de escaneo del puerto de datos del teclado
    scancode = inb(KBD_DATA_PORT);

    // Solo procesamos teclas presionadas (no liberadas)
    if (scancode < 0x80) {
        char c = kbd_us_map[scancode];
        if (c != 0) { // Si el carácter es válido
            // Añadir al búfer
            key_buffer[key_buffer_head] = c;
            key_buffer_head = (key_buffer_head + 1) % KEY_BUFFER_SIZE;
        }
    }

    // Acknowledge al PIC (controlador de interrupciones programable)
    // Esto es crucial para permitir futuras interrupciones
    outb(0x20, 0x20); // EOI para PIC Maestro
}