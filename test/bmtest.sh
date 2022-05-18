[ ! -d build ] && mkdir -p build

(cd build && cmake .. -DCMAKE_BUILD_TYPE=Release)

files=(facebook_edges deezer twitch_edges)
tests=(bmNodeSample bmEdgeSample bmExploreSample)
percents=(0.1 0.5)

LOG_FILE="test/bm.csv"

echo -e "percent,dataset,method,threads,time" >"$LOG_FILE"

for percent in ${percents[*]}; do
  for file in ${files[*]}; do
    for test in ${tests[*]}; do
      sed -e "s/input = \".*\"/input = \"example\/input\/$file.csv\"/" -e "s/percent = .*;/percent = $percent;/" "./test/$test.cpp" >"./test/$test.cpp.tmp"
      mv "./test/$test.cpp.tmp" "./test/$test.cpp"
      cmake --build build
      eval "./build/$test" | grep "threads" | sed -E "s/.*\/(.*)\/threads:([^ ]) *([^ms]*) ms.*/$percent,$file,\1,\2,\3/" >>"$LOG_FILE"
      echo "$file-$test-$percent done"
      sleep 1
    done
  done
done
