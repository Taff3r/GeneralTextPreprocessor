#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAX_LINE_LENGTH (65536) /* NEEDS to be power of 2 */
#define MAX_WORD_LENGTH (1024)  /* NEEDS to be power of 2 */
#define MACRO_DEF "$DEF"
#define FUNC_DEF  "$FUN"
#define INC_DEF   "$INC"
#define MAX_IDENTIFIER_LENGTH (4)
#define MAX_ARGC (10)
#define RESERVED_MACRO_CHAR '$'
#define STRING_DELIMTERS ", ("

/* String used to define how new lines are encoded
 * Needed because some use \r\n while some use
 * just \n.
 */
#define NEWLINE_CHAR "\n"   

#endif
