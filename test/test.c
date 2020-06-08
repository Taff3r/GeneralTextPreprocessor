#include <stdio.h>
#include <string.h>
#include "../hashmap.h"
#include <assert.h>
#define INSERTS (999)
size_t hash(const void* ptr)
{
    size_t hash = 5381;
    int c;
    const char* str = (char*) ptr;
    while ((c = *str++) != '\0')
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
int cmp(const void* a, const void* b)
{
    const char* str_a = (const char*) a;
    const char* str_b = (const char*) b;
    return strcmp(str_a, str_b) == 0;
}

size_t ihash(const void* ptr)
{
    size_t hash = *(int*)ptr;
    
    hash = (hash^0xdeadbeef) + (hash<<4);
    hash = hash ^ (hash>>10);
    hash = hash + (hash<<7);
    hash = hash ^ (hash>>13);
    //hash = hash - (hash >> 1);
    return hash;
}

int icmp(const void* a, const void* b)
{
    const int* int_a = (int*) a;
    const int* int_b = (int*) b;
    return *int_a == *int_b;
}
int main(void)
{
    hash_table* t = new_hash_table(cmp, hash);
    int i;
    // Test inserts and lookups
    for (i = 0; i < INSERTS; ++i) {
        char* s = malloc(sizeof(char) * 5);
        int* k = malloc(sizeof(int));
        *k = i;
        snprintf(s, 5, "%d", i);
        insert(t, s, k);
        assert(*k == *(int*)lookup(t, s));
    }

    printf("Test(1) passed, going to clean up!\n");
    delete_hash_table(t);



    hash_table* is = new_hash_table(icmp, ihash);
    for (i = 0; i < INSERTS; ++i) {
        char* s = malloc(sizeof(char) * 5);
        if (s == NULL) {
            printf("malloc returned NULL\n");
            exit(1);
        }
        int* k  = malloc(sizeof(int));
        *k = i;
        snprintf(s, 5, "%d", i);
        insert(is, k, s);
        assert(strcmp(s, (char*)lookup(is, k)) == 0);
    }


    // Test lookup of value that doesn't exist
    assert(lookup(is, "some random value") == NULL);
    // Test insert duplicate value
    char* k = calloc(5, sizeof(char));
    int* p = malloc(sizeof(int));
    *p = i;
    snprintf(k, 5, "%d", 1);
    assert(insert(is, k, p) == 0);

    // Test delete value that exists
    assert(remove_from_table(is, k) == 1);
    // Test delete value that doesn't exist
    assert(remove_from_table(is, k) == 0);
    free(k);
    free(p);
    printf("Test(2) passed, going to clean up!\n");
    delete_hash_table(is);
}
