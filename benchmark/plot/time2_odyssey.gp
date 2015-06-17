set style line 1 lc rgb 'red' lw 2
set style line 2 lc rgb 'orange' lw 2
set style line 3 lc rgb 'purple' lw 2
set style line 4 lc rgb 'blue' lw 2
set style line 5 lc rgb 'green' lw 2
set style line 6 lc rgb 'black' lw 2

set yrange [0:400]
set title 'Time (Image), dictionary size = 25000'
set xlabel 'Input size (kB)'
set ylabel 'Encoding time (ms)'

plot 'time2_odyssey.dat'      u 1:2 w line ls 1 title 'LZ78 SMRU', \
     ''                   u 1:3 w line ls 2 title 'LZ78 SMRU2', \
     ''                   u 1:4 w line ls 4 title 'LZW SMRU', \
     ''                   u 1:5 w line ls 5 title 'LZW SMRU2'
