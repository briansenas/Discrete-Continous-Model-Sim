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

fileposfix = "10 100 500 1000 10000 100000"

file_exists(file ) = system("[ -f '".file."' ] && echo '1' || echo '0'") + 0
print "Enter 1 for Expected Profit, or 2 for Average Throws"
read_row = system("read a; echo $a") + 1

if (read_row==2){
    lbt = "Expected Profit"
    pfx = "Ep"
}else{
    lbt = "Average Throws"
    pfx = "At"
}

set palette defined ( 0 "red", 1 "green")
set cbrange[0:1]
# color(y) = y >= 0 ? (255*65535) : (255*255)

set xlabel "Iteración"
set ylabel "Promedio Esperado"

dir = "a_graphs"
system "mkdir ".dir

do for [file in fileposfix]{
    name="A-Coin-".file
    filename = name.".txt"
    if (file_exists(filename)) {
        set title file." Tiradas"
        print "Targetting: ".filename
        outname = dir."/".name.pfx.".png"
        set output outname
        command = "head -n1 ".filename." | cut -d ' ' -f 1"
        rows_ahead = system(command) + 2
        if(read_row==2){
            plot filename every ::rows_ahead:::-1 u 1:read_row:($2>0? 1:0) with lp lc palette z lw 1.5 t lbt, 0 lw 2 lt -1 t "base line"
        }else{
            plot filename every ::rows_ahead:::-1 u 1:read_row with lp lw 1.5 t lbt, 0 lw 2 lt -1 t "base line"
        }
        unset title
    }
}
