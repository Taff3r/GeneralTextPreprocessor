#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void* xmalloc(size_t size)
{
    void* ptr = malloc(size);
    if(ptr == NULL)
        exit(1);
    return ptr;
}
void* xcalloc(size_t cnt, size_t size)
{
    void* ptr = calloc(cnt, size);
    if(ptr == NULL)
        exit(1);
    return ptr;
}
hash_table* new_hash_table(int (*comp)(const void* a, const void* b), size_t (*hash) (const void* k)) 
{
    hash_table* table;
    table = xmalloc(sizeof(hash_table));
    table->entries = xcalloc(TABLE_INIT_SIZE, sizeof(hash_entry*)); // TODO: Optimize since power of 2
    table->sz = TABLE_INIT_SIZE;
    table->inserted = 0; 
    table->compare = comp;
    table->hash = hash;

    return table;
}

hash_entry* new_hash_entry(void* key, void* val) 
{
    hash_entry* e;
    e = xmalloc(sizeof(hash_entry));
    e->key = key;
    e->val = val;
    e->next = NULL;
    return e;
}
void delete_hash_table(hash_table* table)
{
    int i;
    size_t sz;

    sz = table->sz;
    for (i = 0; i < sz; ++i) {
        delete_entry(table->entries[i]);
    }

    free(table->entries);
    free(table);
}

void delete_entry(hash_entry* e)
{
    if(e == NULL)
        return;
    hash_entry* t = e;
    hash_entry* n;
    do {
        free(t->val);
        free(t->key);
        n = t;
        t = t->next;
        free(n);
    } while(t != NULL);
}

int insert(hash_table* table, void* key, void* val)
{
    if(lookup(table,key) != NULL)
        return 0;
    size_t index = table->hash(key) & (table->sz - 1); 
    hash_entry* e = table->entries[index];
    if (e == NULL) {
        table->entries[index] = new_hash_entry(key, val);
    } else {
        if (table->entries[index]->next != NULL) { // Not NULL
            hash_entry* nn = table->entries[index]->next; // Can be NULL
            table->entries[index]->next = new_hash_entry(key, val);
            table->entries[index]->next->next = nn;
        } else {
            table->entries[index]->next = new_hash_entry(key, val);
            table->entries[index]->next->val = val;
            table->entries[index]->next->key = key;
        }
    }
    table->inserted++;
    if(table->inserted / table->sz > MAX_SATIATION)
        grow(table);
    return 1;
}

void grow(hash_table* table)
{
    size_t i;
    size_t old_sz;
    hash_entry** old_entries;

    old_entries = table->entries;
    old_sz = table->sz;
    table->sz = (table->sz << 1); // table size = 2 * old table size;
    table->entries = xcalloc(table->sz, sizeof(hash_entry*));

    for (i = 0; i < old_sz; ++i) {
        size_t index;
        if (old_entries[i] != NULL) {
            index = table->hash(old_entries[i]->key) & (table->sz - 1);
            table->entries[index] = old_entries[i];
        }
    }
    free(old_entries);
}

void* lookup(hash_table* table, const void* key)
{
    void* val = NULL;
    size_t index = table->hash(key) & (table->sz - 1);
    hash_entry* e = table->entries[index];

    if (e == NULL)
        return val;

    if(table->compare(e->key, key)) // 1 == Equal
        val = e->val;
    else {
        hash_entry* t = e->next;
        while(t != NULL) {
            if (table->compare(t->key, key)) {
                val = t->val;
                return val;
            }
            else
                t = t->next;
        }
    }
    return val;
}

int remove_from_table(hash_table* table, const void* key)
{
    size_t index;
    hash_entry* e;
    index = table->hash(key) & (table->sz - 1);

    e = table->entries[index];
    if (e == NULL)
        return 0;

    do {
        if (table->compare(e->key, key)) {
            if (e->next) {
                table->entries[index] = e->next;
            } else {
                table->entries[index] = NULL;
            }
            free(e->key);
            free(e->val);
            free(e);
            table->inserted--;
            return 1;
        }  else {
            e = e->next;
        }
    } while (e != NULL);
    return 0;
}
