set style line 1 lc rgb 'red' lw 2
set style line 2 lc rgb 'orange' lw 2
set style line 3 lc rgb 'purple' lw 2
set style line 4 lc rgb 'blue' lw 2
set style line 5 lc rgb 'green' lw 2
set style line 6 lc rgb 'black' lw 2

#set yrange [0:1]
set title 'Codeword number (Image), dictionary size = 25000'
set xlabel 'Input size (kB)'
set ylabel 'Codewords'

plot 'incr_img.dat'  u 1:3 w line ls 1 title 'LZ78 SMRU', \
     ''                  u 1:6 w line ls 2 title 'LZ78 WMRU', \
     ''                  u 1:9 w line ls 3 title 'LZ78 MRA', \
     ''                  u 1:12 w line ls 4 title 'LZW SMRU', \
     ''                  u 1:15 w line ls 5 title 'LZW WMRU', \
     ''                  u 1:18 w line ls 6 title 'LZW MRA'
