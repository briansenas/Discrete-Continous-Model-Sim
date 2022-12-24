set terminal png size 1280, 720 enhanced font "Times-New-Roman,18"
set output "COIN.png"

set key on
set key bottom right
set auto
set grid
set size 1,1
# set yrange[0:1]
# set ytics 0, 0.1, 1
# stats "COIN.txt" u 1:4 i 0 name "a"
# set xrange[0:a_max_x]

set title 'Testeando'
set palette defined ( 0 "red", 1 "green")
set cbrange[0:1]
color(y) = y >= 0 ? (255*65535) : (255*255)

plot "COIN.txt" every 1000::::-1 using 1:2:($2>0? 1:0) with lp lc palette z lw 1.5, 0 lw 2
