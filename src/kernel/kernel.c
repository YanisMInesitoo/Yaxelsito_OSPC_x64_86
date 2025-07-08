// src/kernel/kernel.c (extracto, solo las partes modificadas/añadidas)
#include "vga.h"
#include "keyboard.h"
#include "stdio.h"
#include "shell.h"
#include "string.h"
#include "system_state.h"
#include "timer.h" // ¡Incluir el nuevo driver del temporizador!

// ... (estructuras GDT, IDT, pic_remap, idt_set_gate, load_idt ya presentes) ...

// Mensajes de excepciones comunes (solo los primeros 32)
const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Coprocessor Segment Overrun (reserved)",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved (Intel)",
    "x87 FPU Floating-Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// Estructura de un frame de interrupción (lo que pushad guarda + int_no)
typedef struct {
   uint32_t edi;
   uint32_t esi;
   uint32_t ebp;
   uint32_t esp_before_pushad; // ESP antes del pushad
   uint32_t ebx;
   uint32_t edx;
   uint32_t ecx;
   uint32_t eax;
   uint32_t int_no; // Numero de interrupcion
   uint33_t err_code; // Codigo de error (si hay)
   uint32_t eip;
   uint32_t cs;
   uint32_t eflags;
   uint32_t useresp; // ESP del usuario (si el switch de privilegio ocurre)
   uint32_t ss;      // SS del usuario (si el switch de privilegio ocurre)
} __attribute__((packed)) registers_t;


// Manejador genérico de interrupciones C (para excepciones y IRQs sin handler específico)
void isr_handler(registers_t regs) {
    // Si es una excepcion de CPU (0-31)
    if (regs.int_no < 32) {
        printf("\n");
        printf("!!! EXCEPCION DE CPU !!!\n");
        printf("  Tipo: %s (%d)\n", exception_messages[regs.int_no], regs.int_no);
        if (regs.err_code != 0) { // Algunas excepciones tienen codigo de error
            printf("  Codigo de Error: 0x%x\n", regs.err_code);
        }
        printf("  EIP: 0x%x\n", regs.eip);
        printf("  CS:  0x%x\n", regs.cs);
        printf("  EFLAGS: 0x%x\n", regs.eflags);
        printf("  EAX: 0x%x EBX: 0x%x ECX: 0x%x EDX: 0x%x\n", regs.eax, regs.ebx, regs.ecx, regs.edx);
        printf("  ESI: 0x%x EDI: 0x%x EBP: 0x%x ESP: 0x%x\n", regs.esi, regs.edi, regs.ebp, regs.esp_before_pushad);
        printf("  El sistema se ha detenido debido a un error critico.\n");
        __asm__ __volatile__ ("cli; hlt"); // Deshabilitar interrupciones y detener la CPU
    } else {
        // Manejador para IRQs (si no tienen un handler específico)
        // Esto solo es para IRQs que no son el teclado o el timer
        // En un sistema real, no deberías llegar aquí para IRQs activas.
        printf("IRQ no manejada: %d\n", regs.int_no - 0x20); // Restamos el offset del PIC

        // EOI para PIC Maestro
        outb(0x20, 0x20);
        // Si la IRQ es del PIC Esclavo (IRQs 8-15, que corresponden a IDT 0x28-0x2F)
        if (regs.int_no >= 0x28) {
            outb(0xA0, 0x20); // EOI para PIC Esclavo
        }
    }
}


// Definiciones de los stubs de ensamblador para excepciones (con y sin código de error)
// Estos son los que saltarán a isr_handler
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();  // Con codigo de error
extern void isr9();
extern void isr10(); // Con codigo de error
extern void isr11(); // Con codigo de error
extern void isr12(); // Con codigo de error
extern void isr13(); // Con codigo de error
extern void isr14(); // Con codigo de error
extern void isr15();
extern void isr16();
extern void isr17(); // Con codigo de error
extern void isr18();
extern void isr19();
// ... hasta isr31 (para todas las excepciones)

// Stubs para IRQs (desde 0x20 en la IDT)
extern void irq0(); // Timer
extern void irq1(); // Keyboard
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


void load_idt() {
    idt_ptr_reg.limit = (sizeof(struct idt_entry) * 256) - 1;
    idt_ptr_reg.base = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    // Remapear el PIC para evitar conflictos con excepciones de CPU
    pic_remap(0x20, 0x28); // Mapea IRQ0-7 a 0x20-0x27, IRQ8-15 a 0x28-0x2F

    // Establecer entradas de la IDT para EXCEPCIONES (0-31)
    // Las excepciones 8, 10, 11, 12, 13, 14, 17 tienen código de error pushado por la CPU
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);  // Con codigo de error
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E); // Con codigo de error
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E); // Con codigo de error
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E); // Con codigo de error
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E); // Con codigo de error
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E); // Con codigo de error
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E); // Con codigo de error
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    // ... y el resto hasta 31


    // Establecer entradas de la IDT para IRQs (32-47, o 0x20-0x2F)
    idt_set_gate(0x20, (uint32_t)irq0, 0x08, 0x8E); // IRQ0 (Timer)
    idt_set_gate(0x21, (uint32_t)irq1, 0x08, 0x8E); // IRQ1 (Keyboard)

    // Configurar el resto de IRQs con el manejador generico si no hay uno especifico
    for (int i = 0x22; i <= 0x2F; i++) {
        // En un OS real, se configurarían handlers específicos para cada IRQ importante.
        // Aquí usamos el isr_handler común que filtra por numero de IRQ.
        idt_set_gate(i, (uint32_t)irq_handler_stub, 0x08, 0x8E);
    }
    
    // Cargar el registro IDTR
    __asm__ __volatile__ ("lidt (%0)" : : "r" (&idt_ptr_reg));

    // Desmascarar IRQ0 (timer) e IRQ1 (teclado) en el PIC
    uint8_t mask_master = inb(0x21);
    outb(0x21, mask_master & ~(1 << 0) & ~(1 << 1)); // Desmascarar IRQ0 y IRQ1
}


// Función principal del kernel
void kernel_main() {
    vga_init();         // Inicializa el driver VGA
    keyboard_init();    // Inicializa el driver de teclado (solo preparación)
    init_system_state(); // Inicializa el estado del sistema

    printf("Iniciando Yaxelsito OS PC...\n");

    // Configuración del PIC e IDT (incluyendo IRQ0 y excepciones)
    load_idt();
    init_timer(100);    // Inicializa el timer a 100 Hz (100 ticks por segundo)
    __asm__ __volatile__ ("sti"); // Habilitar interrupciones

    printf("Sistema iniciado. Escribe 'help' para ver los comandos.\n");

    run_shell(); // Inicia la shell

    while (1) {
        __asm__ __volatile__ ("hlt"); // Detener la CPU hasta la próxima interrupción
    }
}