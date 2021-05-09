reset
unset key
unset tics 
unset border 
set pointsize 0.3
set term pdf enhanced 
set out 'pontos.pdf'
plot 'input.txt' with points pt 7
