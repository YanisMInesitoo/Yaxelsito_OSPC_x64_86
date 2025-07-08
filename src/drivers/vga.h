// src/drivers/vga.h
#ifndef VGA_H
#define VGA_H

#include "common.h" // Para tipos como uint8_t

// Constantes de color VGA
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

// Funciones de control de VGA
void vga_init();
void vga_set_color(uint8_t fg, uint8_t bg);
void vga_putc(char c);
void vga_puts(const char* str);
void vga_clear();
void vga_set_cursor_pos(uint8_t x, uint8_t y);

#endif // VGA_H