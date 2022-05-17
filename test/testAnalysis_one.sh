#!/bin/bash

log_path="../results/facebook_edges_onelog.txt"
graph_path="../example/input/facebook_edges.csv"

echo "Graph name: facebook_edges" > ${log_path}
i=0
for ((i=1;i<=3;i++))
    do
    echo "Turn ${i} start."
    echo "Trun ${i}:" >> ${log_path}
    ../build/testAnalysis ${graph_path} directed \
        -degree \
        -cluster \
        -hot_plot \
        -cncomp \
        -linear \
        --timer \
        >> ${log_path}
    echo "" >> ${log_path}
    done
echo "Finished."