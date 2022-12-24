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

file_exists(file ) = system("[ -f '".file."' ] && echo '1' || echo '0'") + 0

set palette defined ( 0 "red", 1 "green")
set cbrange[0:1]
# color(y) = y >= 0 ? (255*65535) : (255*255)

# set xlabel "Iteración"
# set ylabel "Promedio Esperado"


dir = "river_graphs"
system "mkdir ".dir

# https://stackoverflow.com/questions/37674787/gnuplot-get-value-of-a-particular-data-in-a-datafile-with-or-without-using-sta
getValue(row,col,filename) = system('awk ''{if (NR == '.row.') print $'.col.'}'' '.filename.'')

# lines = floor(system("wc -l media1-120-0-40.txt"))

fish = "25000 500000 1000000 3000000"
shark  = "250 500 750"
frecuency = "4 5"

n_fish = words(fish)
n_shark = words(shark)
do for [r in frecuency]{
    do for [f in fish]{
        do for [s in shark]{
            name="values-".r."-".f."-".s
                filename = name.".txt"
                if (file_exists(filename)) {
                    out_N = dir."/"."Comportamiento".r."-".f."-".s
                        outname = out_N.".png"
                        name = "Proyección del entorno para 10 años con F=".r
                        set title name
                        set output outname
                        fs = getValue(1,2,filename)
                        ss = getValue(1,3,filename)
                        set ytics nomirror tc ls 2
                        set y2tics nomirror tc ls 1
                        set xlabel "Días transcurridos"
                        set ylabel "Peces pequeños"
                        set y2label "Peces grandes"
                        plot filename every ::2:::-1 u 1:2 w l ls 2 t "Peces pequeños: ".fs, \
                        ""  every ::2:::-1 u 1:3 w l ls 1 t "Peces grandes: ".ss axes x1y2
                        set output out_N."W.png"
                        set ytics nomirror tc ls 4
                        unset y2label
                        unset y2tics
                        set title "Convergencia del Sistema con F=".r
                        set ylabel "Peces grandes, inicial=".s
                        set xlabel "Peces pequeños, inicial=".f
                        plot filename every ::2:::-1 u 2:3 w l ls 4 t ""
# outname = dir."/"."Comportamiento".s."-".f.".png"
# set output outname
# plot filename every ::2:::-1 u 1:3 w l lw 2 title "Pesces grandes: ".ss
                        unset title
                        unset ylabel
                        unset xlabel
                }
        }
    }
}
