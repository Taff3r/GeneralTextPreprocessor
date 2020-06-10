#include "process_file.h"
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include "hashmap.h"
#include "definitions.h"
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
    const char* m_a = (const char*) a;
    const char* m_b = (const char*) b;
    return strcmp(m_a, m_b) == 0;
}
/*
 * Read all macro defintions and put them in the map.
 */
void process(FILE* input, FILE* output)
{
    hash_table* map = new_hash_table(cmp, hash);
    char line[MAX_LINE_LENGTH];
    while(1) {
        /* Assume no word longer than 65536 chars */
        char* r = fgets(line,MAX_LINE_LENGTH - 1, input);
        if (r == NULL)
            break;
        // normal word write to output
        if (line[0] != RESERVED_MACRO_CHAR)
            write_line(map, line, output);
        else /* Add since it is a macro line. */
            add_macro(line, map);
    }
    delete_hash_table(map);
}

void write_line(hash_table* macros, const char* line, FILE* output)
{
    /* TODO: Check if line contains any of the macros in the table,
     * if so expand them if possible 
     */
    printf("Writing line to file\n");
    fputs(line, output);
    printf("%s", line);
}

int add_macro(char* line, hash_table* t)
{
        char* macro_type; // TODO: Can avoid alloc?
        char* key;
        macro_t* m;

        m = xmalloc(sizeof(macro_t));
        m->expansion = xcalloc(MAX_LINE_LENGTH, sizeof(char));
        key = xcalloc(MAX_WORD_LENGTH + 1, sizeof(char));

        macro_type = strtok(line, " "); /* TODO: Make case of which identifier */
        strcpy(key, strtok(NULL, " "));
        strcpy(m->expansion, strtok(NULL, "\n")); // Read until end of line.
        printf("Macrotype: %s\n", macro_type);
        printf("Macro key: %s\n", key);
        printf("Expansion: %s\n", m->expansion);

        insert(t, key, m);
        printf("%s\n", ((macro_t*)lookup(t, key))->expansion);
        free(m->expansion);
        return 0;
}
