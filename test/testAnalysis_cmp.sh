#!/bin/bash
files=(facebook_edges deezer_edges twitch_edges)
percents=(0.1)
methods=(rn prn rdn re rne hrne ties pies bfs dfs ff snb rw rwj rwr)

for percent in ${percents[*]}; do
    for file in ${files[*]}; do
        for method in ${methods[*]}; do
            log_path="../results/$file-$method.txt"
            graph_path="../example/input/$file.csv"
            sample_path="../results/subgraph.csv"

            echo "Graph name: $file" >$log_path
            echo "start sampling $graph_path $method."
            ../build/testSampler $graph_path $sample_path \
                --undirected \
                --method $method \
                --percent $percent

            echo "sample done."
            echo "Trun $i: " >>$log_path
            ../build/testAnalysis $sample_path undirected \
                --origin $graph_path \
                --cncomp \
                --degree \
                --cluster \
                --hot_plot \
                >>$log_path
            echo "" >>$log_path
            echo "$file-$method done."
        done
    done
done
