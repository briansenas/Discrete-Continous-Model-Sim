#!/bin/bash
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

declare -a arr=( 0 0 0 0 20 20 20 20 40 40 40 60 60 )
declare -a arr2=( 40 60 80 100 40 60 80 100 60 80 100 80 100 )
declare -a arr3=( 1 5 10 50 100 500 )
length=${#arr[@]}
reps=${#arr3[@]}
for (( j = 0; j < reps; j++ )); do
    for (( i = 0; i < length; i++ )); do
        $SCRIPT_DIR/inventario ${arr3[j]} ${arr[i]} ${arr2[i]}
    done
done

