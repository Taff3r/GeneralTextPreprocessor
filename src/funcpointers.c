#include "funcpointers.h"
#include "process_file.h"
#include <string.h>
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


void delete_val(void* macro)
{
    macro_t* m = macro;
    switch (m->type) {
        case DEF:
            {
                def_m* dm = m->macro;
                free(dm->expansion);
                free(dm);
                break;
            }
        case FUN:
            {
                func_m* fm = m->macro;
                for (size_t i = 0; i < fm->argc; ++i)
                    free(fm->argv[i]); 
                free(fm->argv);
                free(fm->expansion);
                free(fm);
                break;
            }
        case FLE:
            {
                file_m* fm= m->macro;
                fclose(fm->file);
                free(fm);
                break;
            }
        default:
            return;
            
    }
}

void delete_key(void* str)
{
    return;
}

int string_cmp(const void* a, const void* b)
{
    const char** ia = (const char**) a;
    const char** ib = (const char**) b;
    if (strlen(*ia) == strlen(*ib))
        return 0;
    else if (strlen(*ia) < strlen(*ib))
        return 1;
    else
        return -1;
}
