set encoding utf8
set terminal png size 1280, 720 enhanced font "Alegreya,18"
# set terminal pdfcairo enhanced color dashed font "Alegreya, 14" \
# set terminal epslatex color colortext
# rounded size 16 cm, 9.6 cm
set print "-"
set key on
set key bottom right
set auto
set grid
set size 1,1
set macro

# Standard border
set style line 11 lc rgb '#808080' lt 1 lw 3
set border 0 back ls 11
set tics out nomirror

# Standard grid
set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12
# unset grid

set style line 1 lc rgb '#E41A1C' pt 1 ps 1 lt 1 lw 2 # red
set style line 2 lc rgb '#377EB8' pt 6 ps 1 lt 1 lw 2 # blue
set style line 3 lc rgb '#4DAF4A' pt 2 ps 1 lt 1 lw 2 # green
set style line 4 lc rgb '#984EA3' pt 3 ps 1 lt 1 lw 2 # purple
set style line 5 lc rgb '#FF7F00' pt 4 ps 1 lt 1 lw 2 # orange
set style line 6 lc rgb '#FFFF33' pt 5 ps 1 lt 1 lw 2 # yellow
set style line 7 lc rgb '#A65628' pt 7 ps 1 lt 1 lw 2 # brown
set style line 8 lc rgb '#F781BF' pt 8 ps 1 lt 1 lw 2 # pink

set palette defined ( 0 "red", 1 "green")
set cbrange[0:1]
# color(y) = y >= 0 ? (255*65535) : (255*255)

dir = "b_graphs"
system "mkdir ".dir

name="B-Coin"
filename = name.".txt"
outname = dir."/".name.".png"

set output outname
command = "head -n1 ".filename." | cut -d ' ' -f 1"
rows_ahead = system(command) + 2

stats filename u 1:2 nooutput
blocks = STATS_blocks

# https://stackoverflow.com/questions/37674787/gnuplot-get-value-of-a-particular-data-in-a-datafile-with-or-without-using-sta
getValue(row,col,filename) = system('awk ''{if (NR == '.row.') print $'.col.'}'' '.filename.'')
steps = getValue(3,3,filename)

set xrange [10:40]
set yrange [-100:STATS_max_y]

array valores[blocks]
do for[i=1:blocks]{
    val = getValue((i-1)*(2+steps)+rows_ahead,2,filename)
    valores[i]= val

    # print j
    # set title val
}

plot filename  every ::rows_ahead-1:::0 u 1:3 with lp lw 1 title valores[1], \
            for[i=1:blocks-1] filename i i u 1:3 with lp lw 1 title valores[i+1], \
            0 lw 2 lt -1 t "base line"


outname = dir."/".name."2.png"
set output outname

set xrange [10:15]
set yrange [-40:40]
plot filename  every ::rows_ahead-1:::0 u 1:3 with lp lw 1 title valores[1], \
            for[i=1:blocks-1] filename i i u 1:3 with lp lw 1 title valores[i+1],\
            0 lw 2 lt -1 t "base line"
