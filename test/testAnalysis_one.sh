#!/bin/bash

log_path="../results/facebook_edges_onelog.txt"
graph_path="../example/input/simple.csv"

echo "Graph name: facebook_edges" > ${log_path}
i=0
for ((i=1;i<=1;i++))
    do
    echo "Turn ${i} start."
    echo "Trun ${i}:" >> ${log_path}
    ../build/testAnalysis ${graph_path} directed \
        -cncomp \
        -hot_plot  
        # -degree \
        # -cluster \
        # -linear \
        # -timer \
        # >> ${log_path}
    echo "" >> ${log_path}
    done
echo "Finished."