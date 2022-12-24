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

fileposfix = "0-40 0-60 0-80 0-100 20-40 20-60 20-80 20-100 40-60 40-80 40-100 60-80 60-100"
timesposfix = "1 5 10 50 100 120 500"

file_exists(file ) = system("[ -f '".file."' ] && echo '1' || echo '0'") + 0

set palette defined ( 0 "red", 1 "green")
set cbrange[0:1]
# color(y) = y >= 0 ? (255*65535) : (255*255)

# set xlabel "Iteración"
# set ylabel "Promedio Esperado"


dir = "inv1_graphs"
system "mkdir ".dir

# https://stackoverflow.com/questions/37674787/gnuplot-get-value-of-a-particular-data-in-a-datafile-with-or-without-using-sta
getValue(row,col,filename) = system('awk ''{if (NR == '.row.') print $'.col.'}'' '.filename.'')

# lines = floor(system("wc -l media1-120-0-40.txt"))

sz_file = words(fileposfix)
sz_times = words(timesposfix)

array titulos[sz_file*sz_times]
array archivos[sz_file*sz_times]
array cabeceras[sz_file*sz_times]
array outnames[sz_file*sz_times]

set key outside;
set key right bottom;

i = 1
j = 1
c = 1
times_max = 0
file_max = 0
array max_files[sz_times]
do for [times in timesposfix]{
    do for [file in fileposfix]{
        name="media1-".times."-".file
        filename = name.".txt"
        if (file_exists(filename)) {
            outname = dir."/"."media1-".times.".png"
            name = "Comportamiento de Medias Obtenidas (s,S) para ".times." meses"
            cabeceras[i] = name
            outnames[i] = outname
            archivos[j] = filename
            if(i<sz_file){
                stats filename u 1:3 nooutput
                ttl = getValue(1,1,filename)."-".getValue(1,2,filename).":M=".sprintf("%.2f",STATS_mean_y)." STD=".sprintf("%.2f",STATS_stddev_y)
                titulos[j] = ttl
            }
            j= j + 1
            c = c +1
        }
    }
    max_files[i] = j-1
    if(c>1){
        times_max = times_max + 1
        i= i + 1
        c = 1
    }
}


do for[j=1:times_max]{
    set title cabeceras[j]
    set output outnames[j]
    if(j==1){
        plot for[i=1:max_files[j]] archivos[i] u :3 with lp lw 2 title titulos[i]
    }else{
        plot for[i=1:max_files[j]-max_files[j-1]] archivos[max_files[j-1]+i] u :3 with lp lw 2 title titulos[max_files[j-1]+i]
    }
    unset title
}

array contador[sz_file]
array valores[sz_file]

i = 1
j = 1
file_max = 0
do for[file in fileposfix]{
    filename = "VariacionMedia".file.".txt"
    if(file_exists(filename)){
    system("rm ".filename)
    }
}
do for [file in fileposfix]{
    do for [times in timesposfix]{
        name="media1-".times."-".file
        filename = name.".txt"
        if (file_exists(filename)) {
            set print "-"
            cabeceras[i] = "Comportamiento medio versus incremento de repetición"
            outname = "VariacionMedia".file.".txt"
            titulos[i] = "(s-S)=(".file.")"
            archivos[i] = outname
            set print outname append
            stats filename u 1:3 nooutput
            print STATS_mean_y
            j = j + 1
        }
    }
    outnames[i] = dir."/VariacionMedia".file.".png"
    contador[i] = j
    if(j>1){
        file_max = file_max + 1
        i = i + 1
    }
    j = 1
}

set print "-"
set title "Comportamiento de variación de repetición"
set output dir."/VariacionMedia.png"
plot for[i=1:file_max] archivos[i] u :1 with lp lw 2 title titulos[i]
unset title


name = "Medias Obtenidas [2]"
filename = "media2.txt"
outname = dir."/media2.png"
set output outname
main_ttl = "Comportamiento de Medias Obtenidas Dinámico"
i = 1
do for[times in timesposfix]{
        name="media2-".times
        filename = name.".txt"
        if (file_exists(filename)) {
            archivos[i] = filename
            stats filename u 1:3 nooutput
            ttl = "N=".times." M=".sprintf("%.2f",STATS_mean_y)." STD=".sprintf("%.2f",STATS_stddev_y)
            titulos[i] = ttl
            i= i + 1
        }
}
# plot filename u :3 with lp lw 2 title "M=".sprintf("%.2f",STATS_mean_y)." STD=".sprintf("%.2f",STATS_stddev_y)
set print "-"
set title main_ttl
if(i>1){
    plot for[z=1:i-1] archivos[z] u :3 with lp lw 2 title titulos[z]
}
unset title

# name = "Medias Obtenidas [3] s-S"
# outname = dir."/".name.".png"
# set output outname
#
# set key outside;
# set key right bottom;
#
# i = 1
# do for [file in fileposfix]{
#     name="media3-".file
#     filename = name.".txt"
#     if (file_exists(filename)) {
#         stats filename u 1:3 nooutput
#         ttl = getValue(1,1,filename)."-".getValue(1,2,filename).":M=".sprintf("%.2f",STATS_mean_y)." STD=".sprintf("%.2f",STATS_stddev_y)
#         titulos[i] = ttl
#         archivos[i] = filename
#         i= i + 1
#     }
# }
#
# if(i>1){
#     plot for[x=1:i-1] archivos[x] u :3 with lp lw 2 title titulos[x]
# }

i = 1
j = 1
c = 1
times_max = 0
file_max = 0
array max_files[sz_times]
do for [times in timesposfix]{
    do for [file in fileposfix]{
        name="media3-".times."-".file
        filename = name.".txt"
        if (file_exists(filename)) {
            outname = dir."/media3-".times.".png"
            name = "Comportamiento de Medias Obtenidas (s,S) para ".times." meses"
            cabeceras[i] = name
            outnames[i] = outname
            archivos[j] = filename
            if(i<sz_file){
                stats filename u 1:3 nooutput
                ttl = getValue(1,1,filename)."-".getValue(1,2,filename).":M=".sprintf("%.2f",STATS_mean_y)." STD=".sprintf("%.2f",STATS_stddev_y)
                titulos[j] = ttl
            }
            j= j + 1
            c = c +1
        }
    }
    max_files[i] = j-1
    if(c>1){
        times_max = times_max + 1
        i= i + 1
        c = 1
    }
}


do for[j=1:times_max]{
    set title cabeceras[j]
    set output outnames[j]
    if(j==1){
        plot for[i=1:max_files[j]] archivos[i] u :3 with lp lw 2 title titulos[i]
    }else{
        plot for[i=1:max_files[j]-max_files[j-1]] archivos[max_files[j-1]+i] u :3 with lp lw 2 title titulos[max_files[j-1]+i]
    }
    unset title
}
