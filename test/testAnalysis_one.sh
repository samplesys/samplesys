#!/bin/bash

log_path="../results/twitch_edges_one.txt"
graph_path="../example/input/twitch_edges.csv"

echo "Graph name: twitch_edges" > ${log_path}
i=0
for ((i=1;i<=3;i++))
    do
    echo "Turn ${i} start."
    echo "Trun ${i}:" >> ${log_path}
    ../build/testAnalysis ${graph_path} directed \
        -cncomp \
        -hot_plot \
        -degree \
        -cluster \
        -linear \
        -timer \
        >> ${log_path}
    echo "" >> ${log_path}
    done
echo "Finished."