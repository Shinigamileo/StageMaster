#!/bin/sh

SAT=lingeling
TIMEOUTS=600

while getopts s: o; do
    case "$o" in
	s)   SAT="$OPTARG";;
	[?]) exit 1;;
    esac
done
shift $(( $OPTIND - 1 ))

echo "SAT-solver used : $SAT"
echo ""

######################################################################################
if [ "$1" == poulet ]; then
    shift
    for warmup in $(seq 0 10 $3); do
	for size in $(seq $1 40 $2); do
	    echo -e "\033[1;4;36mTest: warmup=$warmup; size=$size\033[0m"
	    echo -e "$size\n$(../utils/random_stream)\n$(../utils/random_stream -seed $(date +%N))" > data1
	    START=$(date +"%s.%3N")
	    python trivium.py -d data1 $(../utils/trivium data1) $warmup | $SAT > report.data
	    END=$(date +"%s.%3N")
	    python trivium.py -r $size report.data > data2
	    cat data1
	    DIFF=$(echo "scale=3; $END - $START"|bc -l)
	    python cmpfiles.py data1 data2 && echo -e "$DIFF ms\n"
	done
    done
######################################################################################
elif [ "$1" == unicity ]; then
    shift
    TOTALUNI=0
    for i in $(seq $2); do
	echo -e "$1\n$(../utils/random_stream)\n$(../utils/random_stream -seed $(date +%N))" > data1
	START=$(date +"%s.%3N")
	python trivium.py -o data1 $(../utils/trivium data1) $3 | $SAT > report.data
	END=$(date +"%s.%3N")
	DIFF=$(echo "scale=3; $END - $START"|bc -l)
	if python trivium.py -r $1 report.data > data2; then
	    python cmpfiles.py data1 data2
	else
	    echo -e "\033[1;36mNone\033[0m" && TOTALUNI=$((TOTALUNI + 1))
	fi
    done
    echo -e "$(echo "scale=2; 100 * $TOTALUNI / $2"|bc -l)%\n"
######################################################################################
else
    TOTALTIME=0
    for i in $(seq $2); do
	echo -e "\033[1;4;36mTest n°$i\033[0m"
	echo -e "$1\n$(../utils/random_stream)\n$(../utils/random_stream -seed $(date +%N))" > data1
	cat data1
	START=$(date +"%s.%3N")
	python trivium.py -d data1 $(../utils/trivium data1) $3 | $SAT > report.data
	END=$(date +"%s.%3N")
	DIFF=$(echo "scale=3; $END - $START"|bc -l)
	python trivium.py -r $1 report.data > data2 && python cmpfiles.py data1 data2 && TOTALTIME=$(echo "scale=3; $TOTALTIME + $DIFF"|bc -l) && echo -e "$DIFF ms\n"
    done
    echo -e "average: $(echo "scale=3; $TOTALTIME / $2"|bc -l) ms\n"
fi
