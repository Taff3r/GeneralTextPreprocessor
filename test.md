$DEF HELLO                               hello there
$DEF SHIT                                fucking fuckfuck
$FUN ADD($a,$b)                          $b + $a + $a - $b
$FUN GREET($a)                           $a, hur mår du?
$DEF MYNAME                              Simon
$DEF MYAGE                               22 years old.
$FUN TEST($arg)                          The arg evaluates to: $arg
$FUN YTC($c)                             https://www.youtube.com/channel/$c
$FUN TWO($a, $b)                         $a $b
$FUN THREE($a, $b, $c)                   $a TWO($b,$c)
$FUN FOUR($arg, $barg, $carg, $darg)     $arg THREE($barg,$carg,$darg)
$FUN INSTA($c)                           https://instagram.com/$c FOUR(one, two, three, four)
$DEF PROF                                simontenggren
$INC /home/simont/git/MarkdownPreprocessor/macros/recinc.gtpp

# Intro
How are you?
$DEF SMALLLAMBDA                         l
$DEF LAMBDA                              L
SMALLLAMBDA LAMBDA SMALLLAMBDA
$FUN CAT($a, $b)                        $a$b
$FUN CAT3($a, $b, $c)                   $aCAT($b,$c)
INSTA(CAT(simon,tenggren))
FOUR(Are you having a,CAT(simon,tenggren),day,INSTA(CAT(MYNAME,MYAGE)))

## Another header
3C
BAJS
COOL
some other text
HELLO
more text...
Another: HELLO, MYNAME
SHIT
LAMDA
CAPL
Hello there my name is MYNAME and I am MYAGE

#### SHIT MYNAME
Some more text
text
txt
ttt
t BIGEPS
asdfasf asdfasdfasd
asdfasdfasdfasdfasdfasdf
test
test
test
test
test
test
SOME
