SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

declare -a arr=(25000 500000 1000000 3000000)
declare -a arr2=(250 500 750)
declare -a arr3=(4 5 )

length=${#arr[@]}
reps=${#arr2[@]}
fre=${#arr3[@]}
for (( x = 0; x < fre; x++ )); do
    for (( j = 0; j < reps; j++ )); do
        for (( i = 0; i < length; i++ )); do
            $SCRIPT_DIR/Simulacion_lago2 3650 ${arr[i]} ${arr2[j]} ${arr3[x]}
        done
    done
done

