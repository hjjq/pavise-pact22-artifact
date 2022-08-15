TODO: move this repo to a new public repo and update remove link below

# Artifact repository for Pavise

Pavise: Integrating Fault Tolerance Support for Persistent Memory Applications

> Han Jie Qiu, Sihang Liu, Xinyang Song, Samira Khan, Gennady Pekhimenko
> 31th International Conference on Parallel Architectures and Compilation Techniques (PACT), 2022

We will provide access to our non-volatile memory server for artifact evaluation.

Figure 11 is our major result. To reproduce them:
```bash
git clone git@github.com:kevins981/pavise-pact22-tmp.git
cd pavise-pact22-tmp
screen # run experiments using screen to prevent progress loss
source repro.sh # repro.sh runs all key experiments
```
After `repro.sh` completes, the experiment results will be placed in `$PAVISE_ROOT/results`

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

