#!/bin/bash

log_path="../results/github_edges.txt"
graph_path="../example/input/github_edges.csv"
sample_path="../results/subgraph.csv"

echo "Graph name: github_edges" > ${log_path}
i=0
for ((i=1;i<=3;i++))
    do
    echo "Turn ${i}:"
    echo "  start sampling."
    ../build/testSampler ${graph_path} ${sample_path} \
        -directed \
        -method rn \
        -percent 0.1 \
        -seed ${i}

    echo "  sample done."
    echo "Trun ${i}: " >> ${log_path}
    ../build/testAnalysis ${sample_path} directed \
        -origin ${graph_path} \
        -cncomp \
        -degree \
        -cluster \
        -hot_plot \
        >> ${log_path}
    echo "" >> ${log_path}
    echo "  analysis done."
    done
