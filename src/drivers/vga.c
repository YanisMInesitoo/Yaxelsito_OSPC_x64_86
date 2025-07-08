// src/drivers/vga.c
#include "vga.h"
#include "common.h" // Para outb, inb
#include "string.h" // Para memset

// Constantes VGA
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Puntero al buffer de video
static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;

// Posición actual del cursor
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

// Color actual de primer plano y fondo
static uint8_t current_color = (VGA_COLOR_LIGHT_GREY << 4) | VGA_COLOR_BLACK; // Gris claro sobre negro

// Función auxiliar para construir un carácter VGA
static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Actualiza la posición del cursor de hardware
static void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Inicializa la pantalla VGA
void vga_init() {
    vga_clear();
    update_cursor();
}

// Establece el color de primer plano y fondo
void vga_set_color(uint8_t fg, uint8_t bg) {
    current_color = (bg << 4) | (fg & 0x0F);
}

// Desplaza la pantalla hacia arriba una fila
static void scroll_up() {
    for (uint8_t y = 1; y < VGA_HEIGHT; y++) {
        for (uint8_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }
    // Borrar la última fila
    for (uint8_t x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', current_color);
    }
}

// Escribe un carácter en la pantalla
void vga_putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') { // Backspace
        if (cursor_x > 0) {
            cursor_x--;
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', current_color);
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', current_color);
        }
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, current_color);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        scroll_up();
        cursor_y = VGA_HEIGHT - 1;
    }
    update_cursor();
}

// Escribe una cadena en la pantalla
void vga_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putc(str[i]);
    }
}

// Limpia la pantalla
void vga_clear() {
    for (uint8_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint8_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', current_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

// Establece la posición del cursor directamente
void vga_set_cursor_pos(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
    update_cursor();
}