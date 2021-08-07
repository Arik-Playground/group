echo "group time"
time g++-11 -DNASSGROUP_BENCHMARK_GIANT_NUMBER=440 -c benchmarking/giant_group/group.cpp --std=c++2a -o /dev/stdout > /dev/null 

echo "tuple time"
time g++-11 -DNASSGROUP_BENCHMARK_GIANT_NUMBER=440 -c benchmarking/giant_group/tuple.cpp --std=c++2a -o /dev/stdout > /dev/null 