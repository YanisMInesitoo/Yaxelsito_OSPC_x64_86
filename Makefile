# Makefile (extracto, solo las partes modificadas)

# Archivos fuente
BOOT_ASM_SRCS := $(SRC_DIR)/boot/boot.asm
KERNEL_C_SRCS := $(wildcard $(SRC_DIR)/kernel/*.c)
DRIVERS_C_SRCS := $(wildcard $(SRC_DIR)/drivers/*.c)
LIB_C_SRCS := $(wildcard $(SRC_DIR)/lib/*.c)
SHELL_C_SRCS := $(wildcard $(SRC_DIR)/shell/*.c)

# Añadir el nuevo archivo del temporizador
DRIVERS_C_SRCS += $(SRC_DIR)/drivers/timer.c

# Combinar todos los archivos C
C_SRCS := $(KERNEL_C_SRCS) $(DRIVERS_C_SRCS) $(LIB_C_SRCS) $(SHELL_C_SRCS)

# ... (El resto del Makefile debería ser similar, con las nuevas dependencias) ...

# Regla para compilar archivos C
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar archivos ASM
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@echo "Assembling $<"
	@$(AS) $(ASMFLAGS) $< -o $@

# Enlazar el kernel
$(BUILD_DIR)/kernel.bin: $(C_OBJS) $(ASM_OBJS)
	@echo "Linking kernel.bin"
	@$(LD) $(LDFLAGS) -o $@ $(ASM_OBJS) $(C_OBJS)

# ... (El resto del Makefile para floppy, iso, run, clean) ...