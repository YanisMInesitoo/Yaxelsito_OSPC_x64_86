# 💻 Yaxelsito OS PC 🌟

Bienvenido al repositorio de **Yaxelsito OS PC**, un sistema operativo didáctico y en constante evolución para la arquitectura x86_64 (con compatibilidad i386) desarrollado desde cero. ¡Prepárate para explorar el fascinante mundo del desarrollo de sistemas operativos! 🚀

## 🚧 Estado del Proyecto

Este proyecto se encuentra en una fase **Alpha/Desarrollo Activo**. Estamos construyendo los cimientos, ¡así que espera bugs, funcionalidad limitada y muchos commits emocionantes! 💪

## ✨ Características (¡Simuladas y Reales!)

* **Bootloader ⚙️:** Arranque inicial y cambio a modo protegido.
* **Gestión de Memoria 🧠:** (¡Futuro! Por ahora, un modelo plano simple).
* **Interrupciones (PIC) 🚦:** Manejo básico de interrupciones de hardware.
* **Driver VGA 📺:** Salida de texto en pantalla para una experiencia de usuario básica.
* **Driver de Teclado ⌨️:** ¡Entrada de texto para interactuar con la shell! (Soporte básico, ¡más por venir!)
* **Shell Interactiva 🐚:** Un intérprete de comandos con soporte para:
    * Comandos básicos (`help`, `echo`, `clear`, `version`, `about`, `reboot`, `shutdown`, `logoff`).
    * Comandos de sistema de archivos **(¡Simulados por ahora! 😉)**: `ls`, `mkdir`, `cd`, `touch`, `rm`, `cat`, `write`.
    * Comandos de información del sistema **(¡Simulados por ahora! 📊)**: `meminfo`, `cpuinfo`, `date`, `time`, `hostname`, `irqinfo`, `pciinfo`, `gettemp`, `register`.
    * Comandos de red **(¡Simulados por ahora! 🌐)**: `route`, `netview`, `arp`.
    * Comandos de utilidad **(¡Simulados por ahora! 🛠️)**: `setuser`, `sethostname`, `setdomain`, `verify`, `vol`, `benchmark`, `hexdump`, `debug`.
* **Temporizador del Sistema (IRQ0) ⏰:** Un reloj interno para el kernel.
* **Manejo Básico de Excepciones de CPU 💥:** Para detectar y reportar errores críticos.
* **Librerías C Básicas 📚:** `stdio.h`, `string.h` con funciones esenciales como `printf` y `gets`.
* **Estado del Sistema Global 🌍:** Un sistema de variables globales para la configuración del shell (usuario, hostname, path).

## 🛠️ Tecnologías Utilizadas

* **NASM ✍️:** Ensamblador para el bootloader y stubs de interrupción.
* **GCC (Cross-Compiler) ➕:** Compilador C para el kernel y drivers.
* **LD (GNU Linker) 🔗:** Enlazador para crear el binario del kernel.
* **QEMU 🖥️:** Emulador para probar el sistema operativo sin hardware real.
* **GRUB (grub-mkrescue) 💿:** Herramienta para crear una imagen ISO booteable.
* **`Makefile` automatización 🏗️:** Automatización del proceso de compilación y emulación.

## 🚀 Cómo Compilar y Ejecutar

Asegúrate de tener un entorno de desarrollo Linux (o WSL en Windows) con `build-essential`, `qemu-system-x86`, `nasm`, y `grub-pc-bin` (o `grub-emu`).

1.  **Clona el repositorio:**
    ```bash
    git clone [https://github.com/tu-usuario/Yaxelsito_OS_PC.git](https://github.com/tu-usuario/Yaxelsito_OS_PC.git)
    cd Yaxelsito_OS_PC
    ```
2.  **Compila el proyecto:**
    ```bash
    make all
    ```
    Esto creará una imagen `.iso` booteable en el directorio `bin/`.

3.  **Ejecuta el sistema operativo en QEMU:**
    ```bash
    make run
    ```
    ¡Se abrirá una ventana de QEMU y verás Yaxelsito OS PC arrancando! 🎉

4.  **Limpiar archivos de compilación:**
    ```bash
    make clean
    ```

## 📜 Estructura del Proyecto
├── src/
│   ├── boot/             # Código del bootloader y configuraciones iniciales
│   │   ├── boot.asm      # Código de arranque en 16 y 32 bits
│   │   └── linker.ld     # Script de enlazador para el kernel
│   ├── drivers/          # Controladores de hardware
│   │   ├── keyboard.c    # Driver de teclado
│   │   ├── keyboard.h
│   │   ├── timer.c       # Driver de temporizador (¡Nuevo!)
│   │   ├── timer.h
│   │   ├── vga.c         # Driver de texto VGA
│   │   └── vga.h
│   ├── filesystem/       # Implementación (futura) del sistema de archivos
│   │   ├── fs.c
│   │   ├── fs.h
│   │   ├── fs_node.c
│   │   └── fs_node.h
│   ├── kernel/           # Núcleo del sistema operativo
│   │   ├── common.c      # Funciones y utilidades comunes
│   │   ├── common.h
│   │   ├── kernel.c      # Punto de entrada del kernel, IDT, GDT, manejadores de ISR
│   │   ├── main.c        # (Si tienes un main separado del kernel.c)
│   │   ├── system_state.c # Estado global del sistema (usuario, ruta, etc.)
│   │   └── system_state.h
│   ├── lib/              # Librerías estándar
│   │   ├── stdio.c       # Entrada/Salida estándar (printf, gets)
│   │   ├── stdio.h
│   │   ├── string.c      # Funciones de cadena (strcmp, strlen, etc.)
│   │   └── string.h
│   ├── shell/            # La interfaz de línea de comandos
│   │   ├── commands.c    # Implementación de los comandos de la shell
│   │   ├── commands.h    # Declaración de los comandos
│   │   ├── shell.c       # Lógica principal de la shell
│   │   └── shell.h
│   └── user_management/  # (Opcional) Gestión de usuarios (no en foco ahora)
│       ├── user_management.c
│       └── user_management.h
├── Makefile              # Archivo de automatización para compilación
└── README.md             # ¡Este archivo!
## 🤝 Contribuciones

¡Todas las contribuciones, sugerencias y reportes de bugs son bienvenidos! Si quieres ayudar a mejorar Yaxelsito OS PC, por favor:

1.  Haz un "fork" de este repositorio. 🍴
2.  Crea una rama para tu nueva característica (`git checkout -b feature/AmazingFeature`). 🌳
3.  Comité tus cambios (`git commit -m 'Add some AmazingFeature'`). 📝
4.  Empuja la rama (`git push origin feature/AmazingFeature`). ⬆️
5.  Abre un Pull Request. ➡️

## 📄 Licencia

Este proyecto está distribuido bajo la **Licencia Pública General de GNU v3.0 (GPLv3)**. Consulta el archivo [`LICENSE`](./LICENSE) para más detalles. Puedes encontrar una copia completa de la licencia en [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html). ⚖️ GNU GPLv3 💖

---

¡Disfruta construyendo tu propio sistema operativo con Yaxelsito OS PC! Si tienes alguna pregunta, no dudes en abrir un "issue" o contactarme. 😊

Made with ❤️ by Yaxelsito Soft.
