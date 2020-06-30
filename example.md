$FUN TWO($a, $b)                         $a $b
$FUN THREE($a, $b, $c)                   $a TWO($b,$c)
$FUN FOUR($arg, $barg, $carg, $darg)     $arg THREE($barg,$carg,$darg)
$DEF one                                 1
$DEF two                                 2
$DEF three                               3
$DEF four                                4
FOUR(one,two,three,four)
FOUR(Are you having a,good,day,today?)
FOUR(TWO(Are,THREE(you,having,a)),THREE(good,day,today?),one,two)
