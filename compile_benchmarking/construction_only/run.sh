echo "group time"
time g++-11 -c compile_benchmarking/construction_only/group.cpp -DNASSGROUP_BENCHMARK_GIANT_NUMBER=300 --std=c++2a -O9 -o /dev/stdout > /dev/null 

echo "tuple time"
time g++-11 -c compile_benchmarking/construction_only/tuple.cpp -DNASSGROUP_BENCHMARK_GIANT_NUMBER=300--std=c++2a -O9 -o /dev/stdout > /dev/null 