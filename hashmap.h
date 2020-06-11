#ifndef HASH_MAP_H
#define HASH_MAP_H
#include <stdlib.h>
typedef struct hash_table hash_table;
typedef struct hash_entry hash_entry;
// Keep the size of the hash table a power of 2 of efficieny reasons.
#define TABLE_INIT_SIZE 64
#define MAX_SATIATION (0.65f)
struct hash_table {
    hash_entry** entries;
    int (*compare)(const void* a, const void* b);
    void(*key_deleter)(void*);
    void(*value_deleter)(void*);
    size_t (*hash)(const void* k);
    size_t sz;
    size_t inserted; 
};

struct hash_entry {
    void* key;
    void* val;
    hash_entry* next;
};
/*
 * Table operations
 */
hash_table* new_hash_table(int (*compare)(const void* a, const void*b), size_t (*hash)(const void* k), void (*key_del)(void*), void(*val_del)(void*));
void delete_hash_table(hash_table*);
int insert(hash_table*, void*, void*);
void* lookup(hash_table*, const void* key);
int remove_from_table(hash_table*, const void* key);
void grow(hash_table*);
/*
 * Entry operations
 */
hash_entry* new_hash_entry();
void delete_entry(hash_entry*, void(*key_deleter)(void*), void(*val_deleter)(void*));
void copy(hash_entry*, hash_entry**, size_t, size_t(*)(const void*));
#endif
