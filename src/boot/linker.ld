/* src/boot/linker.ld */
ENTRY(kernel_main_asm)

SECTIONS {
    . = 0x100000; /* Dirección de carga del kernel */

    .text : {
        *(.multiboot_header) /* Asegura que la cabecera multiboot esté al principio */
        *(.text)             /* Código del kernel */
    }

    .data : {
        *(.data)             /* Datos inicializados */
    }

    .rodata : {
        *(.rodata)           /* Datos de solo lectura */
    }

    .bss : {
        *(.bss)              /* Datos no inicializados (p.ej., la pila, IDT) */
    }

    /* Ajusta el tamaño total del kernel */
    /DISCARD/ : {
        *(.note.gnu.build-id)
        *(.eh_frame)
        *(.comment)
    }
}