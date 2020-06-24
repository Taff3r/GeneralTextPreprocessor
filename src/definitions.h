#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* COLORS FOR OUTPUT */
#define RST  "\x1b[0m"    /* RESET  */
#define KRED "\x1B[31m"   /* RED    */
#define KGRN "\x1B[32m"   /* GREEN  */
#define KYLW "\x1B[33m"   /* YELLOW */
#define KBLU "\x1B[34m"   /* BLUE   */
#define BOLD "\x1B[1m"
#define UNDL "\x1B[4m"

/* STRING DEFINITIONS */
#define MAX_LINE_LENGTH (65536) /* NEEDS to be power of 2 */
#define MAX_WORD_LENGTH (1024)  /* NEEDS to be power of 2 */
#define MACRO_DEF "$DEF"
#define FUNC_DEF  "$FUN"
#define INC_DEF   "$INC"
#define MAX_IDENTIFIER_LENGTH (4)
#define MAX_ARGC (10)
#define RESERVED_MACRO_CHAR '$'
#define STRING_DELIMTERS ", ("
#define NEWLINE_CHAR "\n"   

#endif
