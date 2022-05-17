#!/bin/bash

log_path="../results/facebook_edges_cmplog.txt"
graph_path="../example/input/facebook_edges.csv"
sample_path="../results/subgraph.csv"

echo "Graph name: facebook_edges" > ${log_path}
i=0
for ((i=1;i<=3;i++))
    do
    echo "Turn ${i}:"
    echo "  start sampling."
    ../build/testSampler ${graph_path} ${sample_path} \
        -directed \
        -method rn \
        -percent 0.1

    echo "  sample done."
    echo "Trun ${i}: " >> ${log_path}
    ../build/testAnalysis ${sample_path} directed \
        -degree \
        -cluster \
        -hot_plot \
        -cncomp \
        -origin ${graph_path} \
        >> ${log_path}
    echo "  analysis done."
    done
