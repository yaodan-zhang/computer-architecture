.text
Fin: 	lw	$t1, -400($fp)#bakadfv
   #blala



lw   $t2, 400($fp)
lw   $t3, 0($fp)




sw   $t1, -400($fp)
sw   $t2,	400($fp)
sw $t3,    0($fp)
lwl $t1, -401($fp)
lwr $t2, 403($fp)
swl $t3, 1($fp)
swr $t3, -1($fp)
lb  $t0, -5($gp)
sb  $t0, -11560($gp)
