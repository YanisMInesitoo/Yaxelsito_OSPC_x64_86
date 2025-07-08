// src/kernel/user_management.c (Nuevo archivo)
#include "user_management.h"
#include "stdio.h"
#include "string.h"
// #include "filesystem.h" // Necesitarás un módulo de sistema de archivos real aquí

UserAccount g_user_accounts[MAX_USERS];
int g_num_users = 0;

// Una función de hash muy simple para demostrar el concepto (¡NO USAR EN PRODUCCIÓN!)
char* simple_hash(const char* password) {
    static char hash[MAX_PASSWORD_HASH_LEN + 1];
    unsigned long h = 5381; // djb2 hash
    int c;

    while ((c = *password++)) {
        h = ((h << 5) + h) + c; // h * 33 + c
    }
    itoa(h, hash, 16); // Convertir el hash a hexadecimal
    return hash;
}

void init_user_management() {
    memset(g_user_accounts, 0, sizeof(g_user_accounts));
    g_num_users = 0;
    // Intentar cargar usuarios al inicio del sistema
    // Esto es donde necesitarías el sistema de archivos real
    printf("Initializing user management...\n");
    if (!load_users_from_file("C:/YaxelsitoOS/System32/infos.dat")) {
        printf("No existing users found or failed to load. Creating default 'guest' user.\n");
        // Crear un usuario por defecto si no se pudo cargar o no hay usuarios
        if (g_num_users == 0) {
            add_user("guest", "password"); // Contraseña simple para el ejemplo
            printf("Default user 'guest' created.\n");
        }
    }
}

// SIMULADO: Carga de usuarios desde un archivo
// En una implementación real, esto leería del disco
int load_users_from_file(const char* filepath) {
    printf("Simulating loading users from %s...\n", filepath);
    // Para la demo, cargamos algunos usuarios predefinidos si no se encuentran
    // En el futuro, usarías fopen/fread o tu API de FS
    if (g_num_users == 0) { // Solo si no hay usuarios cargados
        // Ejemplo de usuarios que podrían venir de un archivo real:
        add_user("admin", "securepass");
        add_user("dev", "devpass");
        printf("Simulated users loaded (admin, dev).\n");
        return 1; // Éxito simulado
    }
    return 0; // Fallo simulado o ya hay usuarios
}

// SIMULADO: Guardado de usuarios a un archivo
// En una implementación real, esto escribiría al disco
int save_users_to_file(const char* filepath) {
    printf("Simulating saving users to %s...\n", filepath);
    // En el futuro, usarías fopen/fwrite o tu API de FS para guardar cada UserAccount
    for (int i = 0; i < g_num_users; i++) {
        printf("  Saving user: %s (Hash: %s)\n", g_user_accounts[i].username, g_user_accounts[i].password_hash);
    }
    return 1; // Éxito simulado
}

UserAccount* find_user(const char* username) {
    for (int i = 0; i < g_num_users; i++) {
        if (strcmp(g_user_accounts[i].username, username) == 0) {
            return &g_user_accounts[i];
        }
    }
    return NULL;
}

int add_user(const char* username, const char* password) {
    if (g_num_users >= MAX_USERS) {
        printf("Error: Maximo numero de usuarios alcanzado.\n");
        return 0;
    }
    if (find_user(username) != NULL) {
        printf("Error: El usuario '%s' ya existe.\n", username);
        return 0;
    }
    if (strlen(username) > MAX_USERNAME_LEN || strlen(password) == 0) {
        printf("Error: Nombre de usuario demasiado largo o contraseña vacia.\n");
        return 0;
    }

    UserAccount* new_user = &g_user_accounts[g_num_users];
    strcpy(new_user->username, username);
    strcpy(new_user->password_hash, simple_hash(password));
    g_num_users++;
    printf("Usuario '%s' añadido.\n", username);
    save_users_to_file("C:/YaxelsitoOS/System32/infos.dat"); // Guardar al añadir
    return 1;
}

int delete_user(const char* username) {
    if (strcmp(g_sys_state.username, username) == 0) {
        printf("Error: No puedes eliminar el usuario actualmente logueado.\n");
        return 0;
    }

    for (int i = 0; i < g_num_users; i++) {
        if (strcmp(g_user_accounts[i].username, username) == 0) {
            // Mover los últimos usuarios hacia adelante para llenar el hueco
            for (int j = i; j < g_num_users - 1; j++) {
                g_user_accounts[j] = g_user_accounts[j+1];
            }
            g_num_users--;
            printf("Usuario '%s' eliminado.\n", username);
            save_users_to_file("C:/YaxelsitoOS/System32/infos.dat"); // Guardar al eliminar
            return 1;
        }
    }
    printf("Error: Usuario '%s' no encontrado.\n", username);
    return 0;
}

int change_password(const char* username, const char* new_password) {
    UserAccount* user = find_user(username);
    if (!user) {
        printf("Error: Usuario '%s' no encontrado.\n", username);
        return 0;
    }
    if (strlen(new_password) == 0) {
        printf("Error: La nueva contraseña no puede estar vacia.\n");
        return 0;
    }
    strcpy(user->password_hash, simple_hash(new_password));
    printf("Contraseña para '%s' cambiada.\n", username);
    save_users_to_file("C:/YaxelsitoOS/System32/infos.dat"); // Guardar al cambiar contraseña
    return 1;
}

int verify_password(const char* username, const char* password) {
    UserAccount* user = find_user(username);
    if (!user) {
        return 0; // Usuario no encontrado
    }
    return strcmp(user->password_hash, simple_hash(password)) == 0;
}