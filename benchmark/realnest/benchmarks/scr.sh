#!/bin/bash

# for file in *.benchmark; do
#     mv -- "$file" "${file%.benchmark}.sql"
# done

# for file in *.sql; do
#     sed -i '' '1,16d' "$file"
# done

# for file in *.benchmark; do
#     sed -i '' '16,28c\
# template benchmark/appian_benchmarks/appian.benchmark.in\
# QUERY_NUMBER=1\
# QUERY_NUMBER_PADDED=01' "$file"
# done

# for file in *.benchmark; do
#     sed -i '' '5,15d' "$file"
# done

for file in *.benchmark; do
    sed -i '' 's|template benchmark/appian_benchmarks/appian.benchmark.in|template benchmark/realnest/benchmarks/realnest.benchmark.in|' "$file"
    echo "Updated $file"
done
