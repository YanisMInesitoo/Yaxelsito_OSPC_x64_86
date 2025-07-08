// src/drivers/keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

void keyboard_init();
char keyboard_read_char();
void keyboard_handler(); // Para el manejador de interrupciones

#endif // KEYBOARD_H