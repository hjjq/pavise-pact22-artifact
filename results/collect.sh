#!/bin/bash
csv_name="summary"
echo "Runtime applications (lower is better)" > ${csv_name}.csv
echo "benchmark,Pavise-ignore-list runtime(s),Pavise-conservative,Original Appplication" >> ${csv_name}.csv
# Extract microbenchmark results
for microbench in hashmap_atomic hashmap_tx ctree btree rbtree rtree; do
    printf "$microbench," >> ${csv_name}.csv
    for config in ignorelist conservative no_pavise; do
        result=$(cat microbench_${config}.csv | grep "^$microbench" | cut -d"," -f2)
        printf "$result," >> ${csv_name}.csv
    done
    printf "\n" >> ${csv_name}.csv
done

# Extract vacation result
printf "vacation," >> ${csv_name}.csv
for config in ignorelist conservative no_pavise; do
    result=$(cat vacation_${config} | grep "Run time" | cut -d":" -f2)
    printf "$result," >> ${csv_name}.csv
done
printf "\n" >> ${csv_name}.csv
printf "\n" >> ${csv_name}.csv

echo "Throughput applications (higher is better)" >> ${csv_name}.csv
echo "benchmark,Pavise-ignore-list throughput,Pavise-conservative,Original Appplication" >> ${csv_name}.csv
# Extract Redis result
printf "redis," >> ${csv_name}.csv
for config in ignorelist conservative no_pavise; do
    result=$(cat redis_${config} | grep "Totals" | tr -s ' ' | cut -d" " -f6)
    printf "$result," >> ${csv_name}.csv
done
printf "\n" >> ${csv_name}.csv

# Extract memcached-L result
printf "memcached-L," >> ${csv_name}.csv
for config in ignorelist conservative no_pavise; do
    result=$(cat memcached-L_${config} | grep "Totals" | tr -s ' ' | cut -d" " -f6)
    printf "$result," >> ${csv_name}.csv
done
printf "\n" >> ${csv_name}.csv

# Extract memcached-W result
printf "memcached-W," >> ${csv_name}.csv
for config in ignorelist conservative no_pavise; do
    # the last grep extracts floating point number from string
    result=$(cat memcached-W_${config} | grep "Run time" | cut -d":" -f5 | grep -Eo '[+-]?[0-9]+([.][0-9]+)?')
    printf "$result," >> ${csv_name}.csv
done
printf "\n" >> ${csv_name}.csv
