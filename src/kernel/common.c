// src/lib/common.c
#include "common.h"

// Implementaciones básicas para puertos I/O (para la simulación o hardware real)
void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void io_wait() {
    // Arbitrary port, just to ensure I/O delay
    __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
}