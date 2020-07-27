# General Text Preprocessor
A general text preprocessor allowing users to define macros in their text documents. Written entirly in C without the use of any others code, e.g. no external libraries, with the exception for the C-standard library.

## Compilation and Installation
Clone the repo and run `make`.
To install run `sudo make install`. This will simply move the `gtpp` executable to `/usr/local/bin/` so that `gtpp` is in your users path.

## Usage
To use the program run `./gtpp <input-file> <output-file>`.
The program can read and write to stdin/stdout. Allowing for piping between programs. E.g. :
`cat some.file | ./gtpp > file.out`
will read the output from `cat` process it, and then the output will be redirected into the file `file.out`.

### Macro types
There are currently four different kinds of macros that can be defined.

| **Type** | **Explanation**                                    |**Usage**                                              |
| ----     | ----                                                 | ---                                                 |
| DEF      | Defines a simple text replacement as its expansion.  | `$DEF <KEY> <expansion> `                           |
| FUN      | Defines a function that takes a number of arguments. | `$FUN <KEY>($arg, $arg2, ...) <expansion>`          |
| INC      | Includes a file and reads all macros from it.        | `$INC <full/path/to/file>`                          |
| FILE     | Includes a file as the expansion of the macro.       | `$FILE <KEY> <full/path/to/file>`                   |
| UNDEF    | Undefines a macro, making it unavailable for use     | `$UNDEF <KEY> `                                     |
| IF/IFN   | Boolean statement, can be used for comments and conditional inclusion. | `$IF <KEY/1/0> ` or `$IFN <KEY/1/0>`|
| ENDIF    | Signifies the end of the boolean statement.          | `$ENDIF` |
## Writing macros
There are several rules for getting the expected usage of the program, this will (probably) be fixed in the future.
The rules are as follows:
* All macros must be **defined** on their **own line and at the beginning of it.**
* To define simple text replacement macros use `$DEF KEY <expansion>`.
* To define a function use `$FUN` at the begining of the line.
* Function macros must be defined according to the following rules, in addition to the rules above:
    + There should not be any white space between the macro-key and the argument list.
    + The arguments are comma-seperated and must be lead with the MACRO DEFINTION CHARACTER (default $).
    + Example: `$FUN GREET($greeting,$name) $greeting $name, how are you today?`
* To include a (as in one) file containing macros use `$INC <path/to/file>`
* White space when calling functions are parsed as a part of the the argument. E.g. `GREET( Hello , there )` will result in: ` Hello   there , how are you today?`



## Recursion
Macros can be implemented with calls to other functions or definitions.
Example:
```
$FUN TWO($a, $b)                         $a $b
$FUN THREE($a, $b, $c)                   $a TWO($b,$c)
$FUN FOUR($arg, $barg, $carg, $darg)     $arg THREE($barg,$carg,$darg)
$DEF one                                 1
$DEF two                                 2
$DEF three                               3 
$DEF four                                4

FOUR(one,two,three,four)
FOUR(Are you having a,good,day,today?)
```
Will result in:
```
1 2 3 4
Are you having a good day today?
```

Arguments to functions can also be functions themselves.
```
FOUR(TWO(Are,THREE(you,having,a)),THREE(good,day,today?),one,two)
```
Will evaluate to:
`Are you having a good day today? 1 2`

### Example of usage (**NEW** See usage of IF)
macros/personal.gtpp:
```
$DEF SCHOOL Lunds Tekniska Högskola (LTH)
$DEF NAME Simon Tenggren
```
./src/helloworld.c:
```c
#include <stdio.h>
int main(void)
{
    printf("Hello, world!\n");
}
```
test.md:
```
$INC ./macros/personal.gtpp
$FILE CMAIN ./src/helloworld.c
$FUN INSTALINK($link) https://instagram.com/$link

Hello everyone my name is NAME, and I am currently studying my fourth year at SCHOOL.
Here is a link to my Instagram INSTALINK(simontenggren), follow if you like.

CMAIN
$IF 0
Oops I didn't mean to include this...
$ENDIF

$IF CMAIN
This I want to include...
$ENDIF

```

When running test.md through `gtpp` the follwing output will be produced.
```
Hello everyone my name is Simon Tenggren, and I am currently studying my fourth year at Lunds Tekniska Högskola (LTH).
Here is a link to my Instagram https://instagram.com/simontenggren, follow if you like.

#include <stdio.h>
int main(void)
{
    printf("Hello, world!\n");
}

This I want to include...
```

### Customization
In the header file `src/definitions.h` I allow some customization of the program.
Follow the rules that the header file specifies in comments.
Editing this file will allow you to change the charcter for definitions of macros and arguments from `$` to something else, or change `$DEF`, `$FUN`, or `$INC` to something else.
### Trello.
Stay up to date with the issues and TODO list on Trello.
https://trello.com/b/L0uiNrn2/preprocessor
