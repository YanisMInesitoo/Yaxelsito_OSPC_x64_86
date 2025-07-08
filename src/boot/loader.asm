; src/boot/loader.asm
; Multiboot2 compliant loader for YaxelsitoOS PC (x86-64)
; Loaded by GRUB

%include "src/kernel/common.h" ; For defining KERNEL_VIRTUAL_BASE

section .bootstrap_stack, nobits
    stack_top: equ $ ; Define el tope de la pila inicial

section .text
bits 32 ; Empieza en 32-bit protected mode (GRUB ya nos dejó aquí)

; Multiboot 2 header
; magic number must be 0x36d76289
; architecture must be 0 for i386 (or 0x00000004 for AMD64, but GRUB handles this usually)
; header_length must be size of header + all tags
; checksum must make all four fields sum to 0
MB2_HEADER_MAGIC    equ 0x36d76289
MB2_HEADER_ARCH     equ 0x00000000 ; Any architecture (or 0 for i386 for compatibility)
MB2_HEADER_LENGTH   equ MB2_HEADER_END - MB2_HEADER_START
MB2_HEADER_CHECKSUM equ -(MB2_HEADER_MAGIC + MB2_HEADER_ARCH + MB2_HEADER_LENGTH)

section .multiboot_header
MB2_HEADER_START:
    dd MB2_HEADER_MAGIC
    dd MB2_HEADER_ARCH
    dd MB2_HEADER_LENGTH
    dd MB2_HEADER_CHECKSUM

    ; End tag (required)
    dw 0x0000 ; Tag type: End
    dw 0x0000 ; Tag flags: None
    dd 0x0008 ; Tag size: 8 bytes
MB2_HEADER_END:

; Global GDT (minimal for 64-bit long mode transition)
gdt_start:
    ; Null segment
    dq 0x0

    ; Code segment (ring 0)
    dw 0xFFFF    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10011010b ; Access byte: Present, DPL0, Code, Executable, Read/Write, Accessed
    db 11001111b ; Flags: G=1 (limit*4k), D/B=1 (32-bit), L=1 (64-bit code), AVL=0, Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

    ; Data segment (ring 0)
    dw 0xFFFF    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; Access byte: Present, DPL0, Data, Read/Write, Accessed
    db 01001111b ; Flags: G=1, D/B=1 (32-bit), L=0 (not 64-bit code, but required for data segment in long mode context), AVL=0, Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Limit
    dq gdt_start               ; Base address

; Externamos la función C del kernel
extern kernel_main

section .text
bits 32

_start:
    ; GRUB ya ha cargado nuestro kernel y nos dejó en modo protegido de 32 bits.
    ; La pila de GRUB podría no ser fiable, así que configuramos la nuestra.
    mov esp, stack_top

    ; Cargar GDT
    lgdt [gdt_descriptor]

    ; Habilitar A20 gate (GRUB ya lo hace, pero por si acaso)
    ; Incompleto: Lógica para A20 si no usas GRUB

    ; *** Configuración de la Paginación (CRÍTICO para 64-bit Long Mode) ***
    ; Necesitas definir estructuras de tablas de paginación (PML4, PDPT, PD, PT)
    ; en memoria, mapeando al menos el kernel.
    ; Esto es un ESQUELETO MUY BÁSICO, DEBERÁS LLENAR LA LÓGICA COMPLETA DE PAGINACIÓN.
    ; Ejemplo mínimo para identidad-mapear el kernel (0-4MB o más) y el framebuffer
    ; con páginas de 2MB o 4KB.
    ; Para este ejemplo, supondremos que `setup_paging` es una función assembly
    ; que inicializa las tablas de paginación y devuelve el CR3.

    ; Definiciones para las tablas de paginación (necesitarás más en .bss o .data)
    %define PML4_BASE 0x1000    ; Dirección física para PML4
    %define PDPT_BASE 0x2000    ; Dirección física para PDPT
    %define PD_BASE   0x3000    ; Dirección física para PD
    ; Puedes necesitar más tablas de directorios de página si mapeas más memoria
    ; o si usas páginas de 4KB en lugar de 2MB.

    ; Ejemplo de creación de tablas de paginación (esto iría en una función assembly real)
    ; Poner ceros en las tablas
    mov edi, PML4_BASE
    xor eax, eax
    mov ecx, 0x1000 / 4 ; 1KB de dwords
    rep stosd           ; Rellenar con ceros (para PML4, PDPT, PD, PT)

    ; Configurar PML4: la primera entrada apunta a PDPT
    mov dword [PML4_BASE], PDPT_BASE | 0x3 ; Present, Read/Write

    ; Configurar PDPT: la primera entrada apunta a PD (para mapear 1GB)
    mov dword [PDPT_BASE], PD_BASE | 0x3   ; Present, Read/Write

    ; Configurar PD: Mapear los primeros 2MB del kernel y el framebuffer
    ; Esto es un ejemplo, necesitarás mapear el rango completo del kernel y otros recursos
    ; como el framebuffer VESA, si está en bajo o alto.
    mov eax, 0x00000000 | 0x83 ; Present, Read/Write, Page Size (2MB)
    mov dword [PD_BASE + 0*8], eax ; Mapea la dirección física 0x0 a virtual 0x0
    ; Si el kernel es grande o si el framebuffer está en un rango alto, necesitarás más entradas.

    ; Habilitar PAE (Physical Address Extension)
    mov eax, cr4
    or eax, (1 << 5) ; Set CR4.PAE
    mov cr4, eax

    ; Cargar la dirección de la PML4 en CR3
    mov eax, PML4_BASE
    mov cr3, eax

    ; Habilitar Long Mode
    mov ecx, 0xC0000080 ; EFER MSR
    rdmsr
    or eax, (1 << 8)    ; Set EFER.LME (Long Mode Enable)
    wrmsr

    ; Habilitar Paging
    mov eax, cr0
    or eax, (1 << 31)   ; Set CR0.PG (Paging Enable)
    mov cr0, eax

    ; Saltar al código de 64 bits
    ; Aquí usaremos un "far jump" a un selector de código de 64 bits para refrescar el CS
    ; 0x08 es el selector del segmento de código de 64 bits en nuestra GDT
    jmp 0x08:long_mode_start

bits 64 ; Ahora estamos en 64-bit Long Mode

long_mode_start:
    ; Configurar segmentos de datos de 64 bits (DS, ES, FS, GS, SS)
    ; Los registros de segmento son más simples en 64 bits.
    ; 0x10 es el selector del segmento de datos en nuestra GDT
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Limpiar los registros no utilizados antes de llamar a C
    xor rax, rax
    xor rbx, rdx
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    xor rdi, rdi
    xor r8, r8
    xor r9, r9
    xor r10, r10
    xor r11, r11
    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    ; La pila ya fue configurada, pero podemos reconfirmarla
    mov rsp, stack_top

    ; Llamar a la función C del kernel
    call kernel_main

    ; Si kernel_main() regresa (no debería en un SO), entra en un bucle infinito
    cli
.halt_loop:
    hlt
    jmp .halt_loop