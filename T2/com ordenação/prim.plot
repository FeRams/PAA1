reset
unset key
unset tics 
unset border 
set pointsize 0.05
set term pdf enhanced 
set out 'prim.pdf'
plot 'prim.txt' with linespoints ls 7 lw 0.1
