// src/drivers/timer.h
#ifndef TIMER_H
#define TIMER_H

#include "common.h"

void init_timer(uint32_t frequency); // Inicializa el temporizador a una frecuencia dada
void timer_handler(); // Manejador de interrupcion del temporizador

#endif // TIMER_H