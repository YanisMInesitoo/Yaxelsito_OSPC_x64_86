// src/drivers/timer.c
#include "timer.h"
#include "common.h" // Para outb, inb
#include "stdio.h"  // Para printf (para debug)

// Puertos del PIT (Programmable Interval Timer) 8253/8254
#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND_PORT  0x43

// El contador de ticks del temporizador
volatile uint32_t timer_ticks = 0;

// Inicializa el temporizador a una frecuencia dada (en Hz)
// El divisor se calcula como 1193180 / frecuencia
void init_timer(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency; // Frecuencia de entrada del PIT
    outb(PIT_COMMAND_PORT, 0x36);          // Canal 0, Lo/Hi byte, Modo 3 (Square Wave Generator)
    outb(PIT_CHANNEL0_DATA, (uint8_t)(divisor & 0xFF));        // Byte bajo del divisor
    outb(PIT_CHANNEL0_DATA, (uint8_t)((divisor >> 8) & 0xFF)); // Byte alto del divisor
    printf("Timer inicializado a %d Hz (divisor %d).\n", frequency, divisor);
}

// Manejador de interrupción del temporizador (IRQ0)
void timer_handler() {
    timer_ticks++;
    // Aquí puedes añadir lógica que se ejecute cada tick, por ejemplo:
    // - Actualizar un reloj de tiempo real
    // - Programar el scheduler (si tienes multitarea)

    // Ejemplo de mensaje cada segundo (si el tick es 100 Hz)
    // if (timer_ticks % 100 == 0) {
    //     printf("Tick! (Segundo %d)\n", timer_ticks / 100);
    // }

    // EOI (End of Interrupt) al PIC maestro para permitir futuras interrupciones
    outb(0x20, 0x20);
}