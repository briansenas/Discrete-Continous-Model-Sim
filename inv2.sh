#!/bin/bash
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

declare -a arr3=( 1 5 10 50 100 500 )
length=${#arr[@]}
reps=${#arr3[@]}
for (( j = 0; j < reps; j++ )); do
    $SCRIPT_DIR/inventario2 ${arr3[j]}
done
