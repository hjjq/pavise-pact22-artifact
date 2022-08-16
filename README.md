# Artifact repository for Pavise

Pavise: Integrating Fault Tolerance Support for Persistent Memory Applications

> Han Jie Qiu, Sihang Liu, Xinyang Song, Samira Khan, Gennady Pekhimenko
> 31th International Conference on Parallel Architectures and Compilation Techniques (PACT), 2022

We will provide access to our non-volatile memory server for artifact evaluation.

Figure 11 is our major result. To reproduce them:
```bash
git clone https://github.com/kevins981/pavise-pact22-tmp.git
cd pavise-pact22-tmp
# run experiments using screen to prevent progress loss.
# if for some reason the screen session is disconnected, use screen -r to reattach to the session
screen 
# repro.sh runs all key experiments
source repro.sh 
# "All experiments reproduced successfully" indicates completion
```
After `repro.sh` completes, the experiment results will be placed in `$PAVISE_ROOT/results`, where `$PAVISE_ROOT` is the root directory of the git repository.

`$PAVISE_ROOT/results/summary.csv` summarizes the results of all experiments. To view the results of each individual experiment, please see the rest of the files in `$PAVISE_ROOT/results`:

| File in `$PAVISE_ROOT/results`  | Corresponding results in Figure 11 |
| ------------- | ------------- |
| `microbench_ignorelist.csv`  | Pavise-ignore-list bars for hm-tx, ctree, btree, rbtree, rtree, and hm-atomic |
| `microbench_conservative.csv`  | Pavise-conservative bars for hm-tx, ctree, btree, rbtree, rtree, and hm-atomic  |
| `microbench_no_pavise.csv`  | Original Application bars for hm-tx, ctree, btree, rbtree, rtree, and hm-atomic |
| `{memcached-L\|memcached-W\|redis\|vacation}_ignorelist`  | Pavise-ignore-list bars for {memcached-L \| memcached-W \| redis \| vacation}|
| `{memcached-L\|memcached-W\|redis\|vacation}_conservative`  | Pavise-conservative bars for {memcached-L \| memcached-W \| redis \| vacation}|
| `{memcached-L\|memcached-W\|redis\|vacation}_no_pavise`  | Original Application bars for {memcached-L \| memcached-W \| redis \| vacation}|


![alt text](https://github.com/kevins981/pavise-pact22-tmp/blob/main/fig11.png)

## Summary CSV Format
`$PAVISE_ROOT/results/summary.csv` will have the format as shown below. Note that in Figure 11, all bars are plotted as relative runtime to the Original Appplication. 
For applications that measure throughput (redis, memcached-L, memcached-W), the relative runtime is computed using the inverse of the measured throughput. 
E.g. relative runtime of Pavise-ignore-list redis = (Original Appplication throughput) / (Pavise-ignore-list throughput)

Runtime applications (lower is better) 
|benchmark  | Pavise-ignore-list runtime(s) | Pavise-conservative|Original Appplication
| ------------- | ------------- | ------------- | ------------- |		
|hashmap_atomic|19.921474|25.126959|10.527226|
|hashmap_tx|8.573652|10.705328|6.090657|
|ctree|8.632038|11.064522|5.503845|
|btree|7.677068|8.916856|3.266477|
|rbtree|17.290886|19.553796|7.733091|
|rtree|25.154142|27.932917|6.111267|
|vacation|5135|6179|1928|

Throughput applications (higher is better)
|benchmark|Pavise-ignore-list throughput|Pavise-conservative|Original Appplication|
| ------------- | ------------- | ------------- | ------------- |		
|redis|15856.88|11927.03|29528.63|
|memcached-L|15848.01|15844.94|25623.63|
|memcached-W|12.3|9.3|22.0|


