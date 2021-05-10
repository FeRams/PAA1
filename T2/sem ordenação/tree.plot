reset
unset key
unset tics 
unset border 
set pointsize 0.2
set term pdf enhanced 
set out 'arvore.pdf'
plot 'tree.txt' with linespoints ls 7 lw 0.1
