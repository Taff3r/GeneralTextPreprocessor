#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* COLORS FOR OUTPUT */
#define RST  "\x1b[0m"    /* RESET     */
#define KRED "\x1B[31m"   /* RED       */
#define KGRN "\x1B[32m"   /* GREEN     */
#define KYLW "\x1B[33m"   /* YELLOW    */
#define KBLU "\x1B[34m"   /* BLUE      */
#define BOLD "\x1B[1m"    /* BOLD      */
#define UNDL "\x1B[4m"    /* UNDERLINE */

/* STRING DEFINITIONS */
#define MAX_LINE_LENGTH (65536) /* NEEDS to be power of 2 */
#define MAX_WORD_LENGTH (1024)  /* NEEDS to be power of 2 */

#define RESERVED_MACRO_CHAR '$'
#define MACRO_DEF  "$DEF"
#define FUNC_DEF   "$FUN"
#define INC_DEF    "$INC"
#define FILE_DEF   "$FILE"
#define UNDEF_DEF  "$UNDEF"
#define IF_DEF     "$IF"
#define IFN_DEF    "$IFN"
#define ENDIF_DEF  "$ENDIF"
#define BOOL_TRUE  "1"
#define BOOL_FALSE "0"

#define MAX_IDENTIFIER_LENGTH (6)
#define MAX_ARGC (10)
#define STRING_DELIMTERS ", ("
#define NEWLINE_CHAR "\n"   
#define WHITESPACE   " \t"

#endif
