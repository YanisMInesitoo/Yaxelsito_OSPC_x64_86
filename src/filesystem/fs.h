// src/filesystem/fs.h
#ifndef FS_H
#define FS_H

#include "fs_node.h" // Necesita la definición de FSNode

void fs_init();
FSNode* fs_get_root();
FSNode* fs_get_node_from_path(const char* path);
void fs_get_full_path(FSNode* node, char* buffer, size_t buffer_len);

#endif // FS_H