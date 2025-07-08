// src/filesystem/fs.c
#include "fs.h"
#include "mem.h"    // Para malloc/free si necesitas crear contenido dinámicamente
#include "string.h" // Para strcpy, strcmp, strtok, memset
#include "stdio.h"  // Para printf (debugging)
#include "common.h" // Para MAX_PATH_LEN

static FSNode* s_fs_root = NULL; // La raíz de nuestro sistema de archivos (C:\)

void fs_init() {
    // Crear la unidad raíz C:\
    s_fs_root = fs_create_node("C:", FS_TYPE_DIRECTORY);
    if (!s_fs_root) {
        printf("ERROR: Failed to create root directory C:!\n");
        return;
    }
    printf("Created C:\\\n");

    // --- C:\YaxelsitoOS\ ---
    FSNode* yaxelsito_os_dir = fs_create_node("YaxelsitoOS", FS_TYPE_DIRECTORY);
    if (!yaxelsito_os_dir) return; // Error al crear, manejar
    fs_add_child(s_fs_root, yaxelsito_os_dir);
    printf("Created C:\\YaxelsitoOS\\\n");

    FSNode* kernel_bin = fs_create_node("KERNEL.BIN", FS_TYPE_FILE);
    if (!kernel_bin) return;
    kernel_bin->content = (void*)strdup("Binary content of Yaxelsito OS Kernel.");
    kernel_bin->content_len = strlen(kernel_bin->content);
    fs_add_child(yaxelsito_os_dir, kernel_bin);
    printf("  Added KERNEL.BIN\n");

    // C:\YaxelsitoOS\DRIVERS\
    FSNode* drivers_dir = fs_create_node("DRIVERS", FS_TYPE_DIRECTORY);
    if (!drivers_dir) return;
    fs_add_child(yaxelsito_os_dir, drivers_dir);
    printf("  Created DRIVERS\\\n");
    FSNode* vga_sys = fs_create_node("VGA.SYS", FS_TYPE_FILE);
    if (!vga_sys) return;
    vga_sys->content = (void*)strdup("YaxelsitoOS VGA Driver."); vga_sys->content_len = strlen(vga_sys->content);
    fs_add_child(drivers_dir, vga_sys);
    printf("    Added VGA.SYS\n");
    FSNode* keyboard_drv = fs_create_node("KEYBOARD.DRV", FS_TYPE_FILE);
    if (!keyboard_drv) return;
    keyboard_drv->content = (void*)strdup("YaxelsitoOS Keyboard Driver."); keyboard_drv->content_len = strlen(keyboard_drv->content);
    fs_add_child(drivers_dir, keyboard_drv);
    printf("    Added KEYBOARD.DRV\n");
    FSNode* mouse_drv = fs_create_node("MOUSE.DRV", FS_TYPE_FILE);
    if (!mouse_drv) return;
    mouse_drv->content = (void*)strdup("YaxelsitoOS Mouse Driver."); mouse_drv->content_len = strlen(mouse_drv->content);
    fs_add_child(drivers_dir, mouse_drv);
    printf("    Added MOUSE.DRV\n");

    // C:\YaxelsitoOS\PROGRAMS\
    FSNode* programs_dir = fs_create_node("PROGRAMS", FS_TYPE_DIRECTORY);
    if (!programs_dir) return;
    fs_add_child(yaxelsito_os_dir, programs_dir);
    printf("  Created PROGRAMS\\\n");
    FSNode* shell_exe = fs_create_node("SHELL.EXE", FS_TYPE_FILE);
    if (!shell_exe) return;
    shell_exe->content = (void*)strdup("YaxelsitoOS Command Shell Executable."); shell_exe->content_len = strlen(shell_exe->content);
    fs_add_child(programs_dir, shell_exe);
    printf("    Added SHELL.EXE\n");
    FSNode* paint_exe = fs_create_node("PAINT.EXE", FS_TYPE_FILE);
    if (!paint_exe) return;
    paint_exe->content = (void*)strdup("Simple graphical paint application."); paint_exe->content_len = strlen(paint_exe->content);
    fs_add_child(programs_dir, paint_exe);
    printf("    Added PAINT.EXE\n");
    FSNode* calculator_exe = fs_create_node("CALCULATOR.EXE", FS_TYPE_FILE);
    if (!calculator_exe) return;
    calculator_exe->content = (void*)strdup("Basic calculator utility."); calculator_exe->content_len = strlen(calculator_exe->content);
    fs_add_child(programs_dir, calculator_exe);
    printf("    Added CALCULATOR.EXE\n");
    FSNode* editor_exe = fs_create_node("EDITOR.EXE", FS_TYPE_FILE);
    if (!editor_exe) return;
    editor_exe->content = (void*)strdup("Simple text editor."); editor_exe->content_len = strlen(editor_exe->content);
    fs_add_child(programs_dir, editor_exe);
    printf("    Added EDITOR.EXE\n");

    // C:\YaxelsitoOS\FONTS\
    FSNode* fonts_dir = fs_create_node("FONTS", FS_TYPE_DIRECTORY);
    if (!fonts_dir) return;
    fs_add_child(yaxelsito_os_dir, fonts_dir);
    printf("  Created FONTS\\\n");
    FSNode* default_fnt = fs_create_node("DEFAULT.FNT", FS_TYPE_FILE);
    if (!default_fnt) return;
    default_fnt->content = (void*)strdup("Default bitmap font for YaxelsitoOS."); default_fnt->content_len = strlen(default_fnt->content);
    fs_add_child(fonts_dir, default_fnt);
    printf("    Added DEFAULT.FNT\n");

    // C:\YaxelsitoOS\WALLPAPERS\
    FSNode* wallpapers_dir = fs_create_node("WALLPAPERS", FS_TYPE_DIRECTORY);
    if (!wallpapers_dir) return;
    fs_add_child(yaxelsito_os_dir, wallpapers_dir);
    printf("  Created WALLPAPERS\\\n");
    FSNode* bg_bmp = fs_create_node("BG.BMP", FS_TYPE_FILE);
    if (!bg_bmp) return;
    bg_bmp->content = (void*)strdup("Example background image data."); bg_bmp->content_len = strlen(bg_bmp->content);
    fs_add_child(wallpapers_dir, bg_bmp);
    printf("    Added BG.BMP\n");

    // C:\YaxelsitoOS\CONFIG.SYS
    FSNode* config_sys = fs_create_node("CONFIG.SYS", FS_TYPE_FILE);
    if (!config_sys) return;
    config_sys->content = (void*)strdup("YaxelsitoOS System Configuration File."); config_sys->content_len = strlen(config_sys->content);
    fs_add_child(yaxelsito_os_dir, config_sys);
    printf("  Added CONFIG.SYS\n");

    // --- C:\YaxelsitoOS\System32\ --- ¡Aquí está System32 dentro de YaxelsitoOS!
    FSNode* system32_dir = fs_create_node("System32", FS_TYPE_DIRECTORY);
    if (!system32_dir) return;
    fs_add_child(yaxelsito_os_dir, system32_dir); // System32 es hijo de YaxelsitoOS
    printf("  Created System32\\\n");

    // C:\YaxelsitoOS\System32\DLLS\
    FSNode* dlls_dir = fs_create_node("DLLS", FS_TYPE_DIRECTORY);
    if (!dlls_dir) return;
    fs_add_child(system32_dir, dlls_dir);
    printf("    Created DLLS\\\n");

    FSNode* kernel32_dll = fs_create_node("KERNEL32.DLL", FS_TYPE_FILE);
    if (!kernel32_dll) return;
    kernel32_dll->content = (void*)strdup("Contains core system functions for YaxelsitoOS."); kernel32_dll->content_len = strlen(kernel32_dll->content);
    fs_add_child(dlls_dir, kernel32_dll);
    printf("      Added KERNEL32.DLL\n");

    FSNode* user32_dll = fs_create_node("USER32.DLL", FS_TYPE_FILE);
    if (!user32_dll) return;
    user32_dll->content = (void*)strdup("Provides user interface and windowing functions."); user32_dll->content_len = strlen(user32_dll->content);
    fs_add_child(dlls_dir, user32_dll);
    printf("      Added USER32.DLL\n");

    FSNode* gdi32_dll = fs_create_node("GDI32.DLL", FS_TYPE_FILE);
    if (!gdi32_dll) return;
    gdi32_dll->content = (void*)strdup("Manages graphical device interface for drawing."); gdi32_dll->content_len = strlen(gdi32_dll->content);
    fs_add_child(dlls_dir, gdi32_dll);
    printf("      Added GDI32.DLL\n");

    // Otros archivos importantes directamente en System32
    FSNode* cmd_exe = fs_create_node("CMD.EXE", FS_TYPE_FILE);
    if (!cmd_exe) return;
    cmd_exe->content = (void*)strdup("YaxelsitoOS Command Prompt executable."); cmd_exe->content_len = strlen(cmd_exe->content);
    fs_add_child(system32_dir, cmd_exe);
    printf("    Added CMD.EXE\n");

    FSNode* ntoskrnl_exe = fs_create_node("NTOSKRNL.EXE", FS_TYPE_FILE);
    if (!ntoskrnl_exe) return;
    ntoskrnl_exe->content = (void*)strdup("The core operating system kernel component."); ntoskrnl_exe->content_len = strlen(ntoskrnl_exe->content);
    fs_add_child(system32_dir, ntoskrnl_exe);
    printf("    Added NTOSKRNL.EXE\n");

    FSNode* bootconf_dat = fs_create_node("BOOTCONF.DAT", FS_TYPE_FILE);
    if (!bootconf_dat) return;
    bootconf_dat->content = (void*)strdup("System boot configuration data."); bootconf_dat->content_len = strlen(bootconf_dat->content);
    fs_add_child(system32_dir, bootconf_dat);
    printf("    Added BOOTCONF.DAT\n");


    // --- C:\AppData\ ---
    FSNode* appdata_dir = fs_create_node("AppData", FS_TYPE_DIRECTORY);
    if (!appdata_dir) return;
    fs_add_child(s_fs_root, appdata_dir);
    printf("Created AppData\\\n");

    FSNode* yaxelpaint_data_dir = fs_create_node("YaxelPaint", FS_TYPE_DIRECTORY);
    if (!yaxelpaint_data_dir) return;
    fs_add_child(appdata_dir, yaxelpaint_data_dir);
    printf("  Created YaxelPaint\\\n");

    FSNode* paint_config_ini = fs_create_node("CONFIG.INI", FS_TYPE_FILE);
    if (!paint_config_ini) return;
    paint_config_ini->content = (void*)strdup("[General]\nLastColor=FF00FF\nDefaultBrushSize=5\n");
    paint_config_ini->content_len = strlen(paint_config_ini->content);
    fs_add_child(yaxelpaint_data_dir, paint_config_ini);
    printf("    Added CONFIG.INI\n");

    FSNode* palettes_dir = fs_create_node("PALETTES", FS_TYPE_DIRECTORY);
    if (!palettes_dir) return;
    fs_add_child(yaxelpaint_data_dir, palettes_dir);
    printf("    Created PALETTES\\\n");
    FSNode* default_pal = fs_create_node("DEFAULT.PAL", FS_TYPE_FILE);
    if (!default_pal) return;
    default_pal->content = (void*)strdup("Standard color palette data.");
    default_pal->content_len = strlen(default_pal->content);
    fs_add_child(palettes_dir, default_pal);
    printf("      Added DEFAULT.PAL\n");


    // --- C:\AppData_x86\ ---
    FSNode* appdata_x86_dir = fs_create_node("AppData_x86", FS_TYPE_DIRECTORY);
    if (!appdata_x86_dir) return;
    fs_add_child(s_fs_root, appdata_x86_dir);
    printf("Created AppData_x86\\\n");

    FSNode* retrogame_data_dir = fs_create_node("RetroGame", FS_TYPE_DIRECTORY);
    if (!retrogame_data_dir) return;
    fs_add_child(appdata_x86_dir, retrogame_data_dir);
    printf("  Created RetroGame\\\n");

    FSNode* highscore_dat = fs_create_node("HIGHSCORE.DAT", FS_TYPE_FILE);
    if (!highscore_dat) return;
    highscore_dat->content = (void*)strdup("Top Scores:\n1. Player1 - 10000\n2. Player2 - 8000\n");
    highscore_dat->content_len = strlen(highscore_dat->content);
    fs_add_child(retrogame_data_dir, highscore_dat);
    printf("    Added HIGHSCORE.DAT\n");


    // --- C:\Users\ ---
    FSNode* users_dir = fs_create_node("Users", FS_TYPE_DIRECTORY);
    if (!users_dir) return;
    fs_add_child(s_fs_root, users_dir);
    printf("Created Users\\\n");

    // C:\Users\GUEST\
    FSNode* guest_dir = fs_create_node("GUEST", FS_TYPE_DIRECTORY);
    if (!guest_dir) return;
    fs_add_child(users_dir, guest_dir);
    printf("  Created GUEST\\\n");

    // C:\Users\GUEST\DOCUMENTS\
    FSNode* documents_dir_guest = fs_create_node("DOCUMENTS", FS_TYPE_DIRECTORY);
    if (!documents_dir_guest) return;
    fs_add_child(guest_dir, documents_dir_guest);
    printf("    Created DOCUMENTS\\\n");
    FSNode* readme_txt_guest = fs_create_node("README.TXT", FS_TYPE_FILE);
    if (!readme_txt_guest) return;
    readme_txt_guest->content = (void*)strdup("Welcome to your Guest Documents folder!\nUse 'type README.TXT' to read me.");
    readme_txt_guest->content_len = strlen(readme_txt_guest->content);
    fs_add_child(documents_dir_guest, readme_txt_guest);
    printf("      Added README.TXT\n");

    // C:\Users\GUEST\DOWNLOADS\
    FSNode* downloads_dir_guest = fs_create_node("DOWNLOADS", FS_TYPE_DIRECTORY);
    if (!downloads_dir_guest) return;
    fs_add_child(guest_dir, downloads_dir_guest);
    printf("    Created DOWNLOADS\\\n");

    // C:\Users\GUEST\DESKTOP\
    FSNode* desktop_dir_guest = fs_create_node("DESKTOP", FS_TYPE_DIRECTORY);
    if (!desktop_dir_guest) return;
    fs_add_child(guest_dir, desktop_dir_guest);
    printf("    Created DESKTOP\\\n");


    // --- C:\TEMP\ ---
    FSNode* temp_dir = fs_create_node("TEMP", FS_TYPE_DIRECTORY);
    if (!temp_dir) return;
    fs_add_child(s_fs_root, temp_dir);
    printf("Created TEMP\\\n");


    // --- Archivos en la raíz C:\ ---
    FSNode* autoexec_bat = fs_create_node("AUTOEXEC.BAT", FS_TYPE_FILE);
    if (!autoexec_bat) return;
    autoexec_bat->content = (void*)strdup("echo Welcome to Yaxelsito OS PC!\necho Type 'help' for commands.\n");
    autoexec_bat->content_len = strlen(autoexec_bat->content);
    fs_add_child(s_fs_root, autoexec_bat);
    printf("Added AUTOEXEC.BAT\n");

    FSNode* readme_txt_root = fs_create_node("README.TXT", FS_TYPE_FILE);
    if (!readme_txt_root) return;
    readme_txt_root->content = (void*)strdup("This is the root directory (C:) of Yaxelsito OS PC. Explore!\n");
    readme_txt_root->content_len = strlen(readme_txt_root->content);
    fs_add_child(s_fs_root, readme_txt_root);
    printf("Added README.TXT\n");

    printf("File System initialization complete.\n");
}

FSNode* fs_get_root() {
    return s_fs_root;
}

// Función auxiliar para obtener un nodo a partir de una ruta (ej. C:\Users\GUEST)
// Soporta rutas absolutas y relativas.
FSNode* fs_get_node_from_path(const char* path) {
    if (!path || *path == '\0') {
        return NULL;
    }

    FSNode* current_node = NULL;
    char path_copy[MAX_PATH_LEN];
    strncpy(path_copy, path, MAX_PATH_LEN - 1);
    path_copy[MAX_PATH_LEN - 1] = '\0';

    char* token;
    char* rest = path_copy;

    // Determinar si la ruta es absoluta o relativa
    if (path_copy[0] == 'C' && path_copy[1] == ':' && (path_copy[2] == '\\' || path_copy[2] == '/')) {
        current_node = s_fs_root; // Ruta absoluta, empezar desde la raíz
        rest += 3; // Saltar "C:\"
    } else {
        // Ruta relativa, empezar desde el directorio actual del sistema
        current_node = g_sys_state.current_dir_node; // Asume que g_sys_state ya está inicializado
    }

    if (!current_node) {
        return NULL; // No se pudo determinar el punto de partida
    }

    // Dividir la ruta por separadores de directorio '\' o '/'
    token = strtok(rest, "\\/");

    while (token != NULL) {
        if (strcmp(token, ".") == 0) {
            // Directorio actual, no hacer nada
        } else if (strcmp(token, "..") == 0) {
            // Directorio padre
            if (current_node->parent) {
                current_node = current_node->parent;
            }
        } else {
            // Buscar el hijo
            current_node = fs_find_child(current_node, token);
            if (!current_node) {
                return NULL; // Componente de la ruta no encontrado
            }
        }
        token = strtok(NULL, "\\/");
    }

    return current_node;
}

// Función auxiliar para construir la ruta completa de un nodo (para el prompt)
void fs_get_full_path(FSNode* node, char* buffer, size_t buffer_len) {
    if (!node || !buffer || buffer_len == 0) {
        if (buffer) buffer[0] = '\0';
        return;
    }

    char temp_path[MAX_PATH_LEN];
    temp_path[0] = '\0';
    FSNode* current = node;
    int len = 0;

    // Recorrer hacia arriba hasta la raíz
    while (current != NULL) {
        char part[64]; // Para el nombre del nodo
        strncpy(part, current->name, 63);
        part[63] = '\0';

        // Construir la ruta de derecha a izquierda
        if (current->parent != NULL) { // Si no es la raíz
            char temp_buffer[MAX_PATH_LEN];
            snprintf(temp_buffer, MAX_PATH_LEN, "%s\\%s", part, temp_path);
            strcpy(temp_path, temp_buffer);
        } else { // Es la raíz (ej. "C:")
            char temp_buffer[MAX_PATH_LEN];
            snprintf(temp_buffer, MAX_PATH_LEN, "%s%s", part, temp_path); // La raíz no tiene \ antes
            strcpy(temp_path, temp_buffer);
        }
        current = current->parent;
    }

    // Copiar al buffer final, asegurando que no se desborde
    strncpy(buffer, temp_path, buffer_len - 1);
    buffer[buffer_len - 1] = '\0';
}