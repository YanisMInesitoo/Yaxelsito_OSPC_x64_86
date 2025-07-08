// src/kernel/user_management.h (Nuevo archivo)
#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "common.h" // Para tipos básicos y MAX_PATH_LEN, etc.

#define MAX_USERNAME_LEN 31
#define MAX_PASSWORD_HASH_LEN 64 // Para un hash simple
#define MAX_USERS 10 // Limite de usuarios para tu OS

typedef struct {
    char username[MAX_USERNAME_LEN + 1];
    char password_hash[MAX_PASSWORD_HASH_LEN + 1];
    // Podrías añadir más campos aquí, como:
    // uint8_t is_admin;
    // char home_directory[MAX_PATH_LEN + 1];
} UserAccount;

// Variable global para almacenar las cuentas de usuario en memoria
extern UserAccount g_user_accounts[MAX_USERS];
extern int g_num_users;

// Funciones de gestión de usuarios
void init_user_management();
int load_users_from_file(const char* filepath); // Carga usuarios desde infos.dat
int save_users_to_file(const char* filepath);   // Guarda usuarios a infos.dat
UserAccount* find_user(const char* username);
int add_user(const char* username, const char* password);
int delete_user(const char* username);
int change_password(const char* username, const char* new_password);
int verify_password(const char* username, const char* password);
char* simple_hash(const char* password); // Una función de hash simple (¡no segura!)

#endif // USER_MANAGEMENT_H