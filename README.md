# General Text Preprocessor
A general text preprocessor allowing users to define macros in their text documents. Written entirly in C without the use of any others code, e.g. no external libraries, with the exception for the C-standard library.

## Compilation
Clone the repo and run `make`.
## Usage
To use the program run `./gtpp <input-file> <output-file>`.
The program can read and write to stdin/stdout. Allowing for piping between programs. E.g. :
`cat some.file | ./gtpp > file.out`
will read the output from `cat` process it, and then the output will be redirected into the file `file.out`.


## Writing macros
There are several rules for getting the expected usage of the program, this will (probably) be fixed in the future.
The rules are as follows:
* All macros must be **defined** on their own line.
* After the macro there has to be a space, *NOT A TAB* (yet) between the key and the usage.
* To define simple text replacement macros use `$def KEY <expansion>`.
* To define a function use `$fun` at the begining of the line.
* Function macros must be defined according to the following rules, in addition to the rules above:
    + There should not be any white space between the macro-key and the argument list.
    + The arguments are comma-seperated and must be lead with the MACRO DEFINTION CHARACTER (default $).
    + The should be no whitespace between argument names and the commas.
    + Example: `$fun GREET($greeting,$name) $greeting $name, how are you today?`
* (EXPERIMENTAL) To include a (as in one) file containing macros use `$inc <path/to/file>`

### Example of usage
macros/personal.gtpp:
```
$def SCHOOL Lunds Tekniska Högskola (LTH)
$def NAME Simon Tenggren
```
test.md:
```
$inc ./macros/personal.gtpp
$fun INSTALINK($link) https://instagram.com/$link

Hello everyone my name is NAME, and I am currently studying my fourth year at SCHOOL.
Here is a link to my Instagram INSTALINK(simontenggren), follow if you like.
```

When running test.md through `gtpp` the follwing output will be produced.
```
Hello everyone my name is Simon Tenggre, and I am currently studying my fourth year at Lunds Tekniska Högskola (LTH).
Here is a link to my Instagram https://instagram.com/simontenggren, follow if you like.
```

### Customization
**(UNTESTED)**
In the header file `src/definitions.h` I allow some customization of the program.
Follow the rules that the header file specifies in comments.
Editing this file will allow you to change the charcter for definitions of macros and arguments from `$` to something else, or change `$def`, `$fun`, or `$inc` to something else.
### Trello.
Stay up to date with the issues and TODO list on Trello.
https://trello.com/b/L0uiNrn2/preprocessor

### Issues
* When several macros overlap, such as LAMBDA and SMALLLAMBDA the program will use the one that it finds first in the map.
* Only spaces can be used as white space character.
