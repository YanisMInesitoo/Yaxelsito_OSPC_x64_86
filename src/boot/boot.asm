; src/boot/boot.asm
[org 0x7c00]
[bits 16]

; --- Standard MBR Bootloader ---
; (Simplified for loading kernel, assumes kernel is at LBA 1)

jmp short start
nop

; BPB (BIOS Parameter Block) - Not strictly necessary for direct kernel load,
; but good practice for future filesystem integration.
oem_id          db "MSWIN4.1" ; OEM ID (8 bytes)
bytes_per_sector dw 512
sectors_per_cluster db 1
reserved_sectors dw 1
num_fats        db 2
num_root_entries dw 224
total_sectors   dw 2880 ; For 1.44MB floppy
media_descriptor db 0xF0
sectors_per_fat dw 9
sectors_per_track dw 18
num_heads       dw 2
hidden_sectors  dd 0
total_sectors_large dd 0

start:
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00 ; Stack grows downwards from bootloader start

    ; Load kernel (assuming it's directly after bootloader, or LBA 1)
    ; For simplicity, assume kernel is loaded at 0x100000 by QEMU directly (multiboot is better)

    ; Switch to protected mode
    cli ; Disable interrupts
    lgdt [gdt_ptr] ; Load GDT
    mov eax, cr0
    or eax, 0x1   ; Set PE bit (Protected Enable)
    mov cr0, eax
    jmp dword 0x08:kernel_start_protected_mode ; Far jump to flush pipeline and load CS

[bits 32]
kernel_start_protected_mode:
    ; Set up 32-bit segment registers
    mov ax, 0x10 ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000 ; Set up stack for kernel (above kernel code, below video memory)

    ; Call the C kernel entry point
    extern kernel_main
    call kernel_main

    ; If kernel_main returns (it shouldn't in a real OS)
    cli
    hlt ; Halt the CPU

; GDT (Global Descriptor Table)
gdt_start:
    gdt_null:
        dd 0x0 ; Null descriptor
        dd 0x0
    gdt_code: ; 0x08 (Code segment)
        dw 0xFFFF    ; Segment limit (0-15)
        dw 0x0       ; Base address (0-15)
        db 0x0       ; Base address (16-23)
        db 0x9A      ; Access byte: Present, DPL 0, Code, Conforming, Readable
        db 0xCF      ; Flags: G=1 (4KB granularity), D/B=1 (32-bit), L=0 (64-bit), AVL=0; Limit (16-19)
        db 0x0       ; Base address (24-31)
    gdt_data: ; 0x10 (Data segment)
        dw 0xFFFF
        dw 0x0
        db 0x0
        db 0x92      ; Access byte: Present, DPL 0, Data, Expand-up, Writable
        db 0xCF
        db 0x0
gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1 ; GDT Limit
    dd gdt_start               ; GDT Base

; --- Interrupt Descriptor Table (IDT) handlers ---
; These are assembly stubs that save/restore registers, send EOI,
; and call a C-level interrupt handler.

; External declaration for the C interrupt handler
extern isr_handler
extern keyboard_handler
extern timer_handler


; Macro para crear stubs de ISR sin código de error
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli                  ; Deshabilitar interrupciones (si no estan ya)
        push byte 0          ; Push un 0 como codigo de error dummy (para uniformidad con los que sí tienen)
        push byte %1         ; Push el numero de interrupcion
        jmp isr_common_stub
%endmacro

; Macro para crear stubs de ISR con código de error
%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli                  ; Deshabilitar interrupciones
        push byte %1         ; Push el numero de interrupcion
        ; La CPU ya ha pushado el codigo de error.
        jmp isr_common_stub
%endmacro

; Macro para crear stubs de IRQ (estas no tienen codigo de error de la CPU)
%macro IRQ 2
    global irq%1
    irq%1:
        cli
        push byte 0          ; Push un 0 como codigo de error dummy
        push byte %2         ; Push el numero de interrupcion (offset + IRQ_num)
        jmp irq_common_stub
%endmacro

; --- Excepciones con codigo de error (la CPU lo pone) ---
ISR_ERRCODE 8   ; Double Fault
ISR_ERRCODE 10  ; Invalid TSS
ISR_ERRCODE 11  ; Segment Not Present
ISR_ERRCODE 12  ; Stack-Segment Fault
ISR_ERRCODE 13  ; General Protection Fault
ISR_ERRCODE 14  ; Page Fault
ISR_ERRCODE 17  ; Alignment Check

; --- Excepciones sin codigo de error (nosotros ponemos un 0) ---
ISR_NOERRCODE 0  ; Divide-by-zero
ISR_NOERRCODE 1  ; Debug
ISR_NOERRCODE 2  ; Non-Maskable Interrupt
ISR_NOERRCODE 3  ; Breakpoint
ISR_NOERRCODE 4  ; Overflow
ISR_NOERRCODE 5  ; Bound Range Exceeded
ISR_NOERRCODE 6  ; Invalid Opcode
ISR_NOERRCODE 7  ; Device Not Available (No Math Coprocessor)
ISR_NOERRCODE 9  ; Coprocessor Segment Overrun (Reserved)
ISR_NOERRCODE 15 ; Reserved (Intel)
ISR_NOERRCODE 16 ; x87 FPU Floating-Point Error
ISR_NOERRCODE 18 ; Machine Check
ISR_NOERRCODE 19 ; SIMD Floating-Point Exception
ISR_NOERRCODE 20 ; Virtualization Exception
ISR_NOERRCODE 21 ; Control Protection Exception
; ... y los demas hasta el 31
%assign i 22
%rep 10
    ISR_NOERRCODE %i
%assign i i+1
%endrep

; --- IRQs (mapeadas a 0x20-0x2F en la IDT) ---
IRQ 0, 0x20 ; Timer (IRQ0)
IRQ 1, 0x21 ; Keyboard (IRQ1)
IRQ 2, 0x22
IRQ 3, 0x23
IRQ 4, 0x24
IRQ 5, 0x25
IRQ 6, 0x26
IRQ 7, 0x27
IRQ 8, 0x28
IRQ 9, 0x29
IRQ 10, 0x2A
IRQ 11, 0x2B
IRQ 12, 0x2C
IRQ 13, 0x2D
IRQ 14, 0x2E
IRQ 15, 0x2F

; --- Common ISR stub ---
; Called by all ISRs (exceptions and IRQs)
isr_common_stub:
    pushad              ; Push EAX, ECX, EDX, EBX, ESP (original), EBP, ESI, EDI
    mov eax, esp        ; Guardar ESP para pasar a la funcion C (puntero a 'registers_t')
    push eax            ; Push el puntero al stack frame (registers_t *)
    call isr_handler    ; Llamar al manejador C
    add esp, 4          ; Limpiar el argumento de la pila
    popad               ; Restaurar registros

    ; Dependiendo de si es una IRQ o Excepcion
    mov eax, [esp + 8]  ; Cargar el numero de interrupcion
    cmp al, 0x20        ; Es una IRQ? (IRQs empiezan en 0x20)
    jl .no_eoi          ; No, es una excepcion, no enviar EOI aqui
    
    ; Si es una IRQ, enviar EOI al PIC
    mov al, 0x20
    out 0xA0, al        ; EOI para PIC Esclavo (si la IRQ es de 8-15)
    out 0x20, al        ; EOI para PIC Maestro

.no_eoi:
    ; Quitar el numero de interrupcion y el codigo de error (si hay)
    add esp, 8          ; Pop int_no y err_code
    iret                ; Return from interrupt


; --- Common IRQ stub (para los que no tienen handler especifico como teclado/timer) ---
; Este es el que se usa en el for loop en kernel.c para idt_set_gate(i, (uint32_t)irq_handler_stub, ...)
; En lugar de 'irq_handler_stub_c', vamos a usar el mismo 'isr_handler' que es más genérico.
global irq_handler_stub
irq_handler_stub:
    pushad
    mov eax, esp
    push eax
    call isr_handler ; Usamos el mismo manejador genérico C
    add esp, 4
    popad
    
    ; Envío de EOI ya está en isr_handler (para IRQs)
    ; Si la IRQ fuera del PIC Esclavo, isr_handler se encargaría de enviar EOI a ambos
    
    ; Si decides no usar isr_handler para IRQs genericas, descomentar esto:
    ; mov al, 0x20
    ; out 0x20, al
    ; mov al, 0x20 ; Si viene del esclavo
    ; out 0xA0, al

    ; Quitar el numero de interrupcion y el codigo de error (dummy)
    add esp, 8
    iret

; Padding and Magic Number
times 510 - ($ - $$) db 0
dw 0xAA55