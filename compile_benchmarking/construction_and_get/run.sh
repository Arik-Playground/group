echo "group time"
time g++-11 -c compile_benchmarking/construction_and_get/group.cpp --std=c++2a -O9 -o /dev/stdout > /dev/null 

echo "tuple time"
time g++-11 -c compile_benchmarking/construction_and_get/tuple.cpp --std=c++2a -O9 -o /dev/stdout > /dev/null 