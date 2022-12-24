SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

declare -a arr=(250 500 750 1000 1100)
declare -a arr2=(2 5 7 10)
declare -a arr3=(5)

length=${#arr[@]}
reps=${#arr2[@]}
fre=${#arr3[@]}
for (( x = 0; x < fre; x++ )); do
    for (( j = 0; j < reps; j++ )); do
        for (( i = 0; i < length; i++ )); do
            $SCRIPT_DIR/Simulacion_lago3 3650 3000000 750 ${arr[i]} ${arr2[j]} ${arr3[x]}
        done
    done
done

