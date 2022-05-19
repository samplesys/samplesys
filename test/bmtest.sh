[ ! -d build ] && mkdir -p build

(cd build && cmake .. -DCMAKE_BUILD_TYPE=Release)

files=("facebook_edges.csv" "deezer.csv" "twitch_edges.csv" "blogcatalog_edge.txt")
patterns=("%zd,%zd" "%zd,%zd" "%zd,%zd" "%zd %zd")

tests=(bmNodeSample bmEdgeSample bmExploreSample)
percents=(0.1 0.5)

LOG_FILE="test/bm.csv"

echo -e "percent,dataset,method,threads,time" >"$LOG_FILE"

for percent in ${percents[*]}; do
  for i in ${!files[*]}; do
    file=${files[$i]}
    pattern=${patterns[$i]}
    for test in ${tests[*]}; do
      sed -e "s/input = \".*\"/input = \"example\/input\/$file\"/" -e "s/percent = .*;/percent = $percent;/" -e "s/%zd.*%zd/$pattern/" "./test/$test.cpp" >"./test/$test.cpp.tmp"
      mv "./test/$test.cpp.tmp" "./test/$test.cpp"
      cmake --build build
      eval "./build/$test" | grep "threads" | sed -E "s/.*\/(.*)\/threads:([^ ]) +([^ ]+) ([^ ]+).+/$percent,$file,\1,\2,\3\4/" >>"$LOG_FILE"
      echo "$file-$test-$percent done"
      sleep 1
    done
  done
done
