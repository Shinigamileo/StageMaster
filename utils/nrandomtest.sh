#!/bin/bash

key=$(./random_stream 80)

for i in $(seq $2); do
    iv[$i]=$(./random_stream -seed $(date +%N) 80)
    stream[$i]=$(../trivium/trivium $1 $key ${iv[$i]})
done

echo $key > key.data

echo $(for i in $(seq $2); do printf " ${iv[$i]} ${stream[$i]}"; done)
