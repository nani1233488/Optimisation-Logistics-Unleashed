# Optimisation-Logistics-Unleashed

Problem Statement: https://drive.google.com/file/d/1FQTwungBC_pTOypwuLhny6jmMsmEGq3M/view?usp=sharing

## Overview

In this project, we implement a C++ solution that plans an end-to-end delivery route under strict fuel constraints. By modeling the city as a weighted undirected graph, the program identifies optimal sequences of hub visits and package drop-offs, dynamically detouring to fuel stations only when necessary. Leveraging all-pairs shortest-path precomputation and on-the-fly feasibility checks, the algorithm balances route length with fuel usage to minimize total travel cost while ensuring that each house is served only after its corresponding hub.

---

## Input Format

```
n t m k f
h[0] h[1] ... h[n-1]      // hub nodes
d[0] d[1] ... d[n-1]      // house nodes
fs[0] fs[1] ... fs[k-1]   // fuel station nodes
m lines of: a b c         // undirected edge between node a and b with fuel cost c
```

* `n`: number of hub-house pairs
* `t`: total number of nodes
* `m`: total number of edges
* `k`: total number of fuel stations
* `f`: capacity of fuel tank

---

## Output Format

```
<length of the optimized path>
<sequence of node indices (comma seperated)>
```

* First line: the number of nodes in the final delivery route.
* Second line: the full route as a sequence of node indices.

---

Compile and run:

```bash
g++ -std=c++17 -o optimize logistics_optimization.cpp
./optimize < tc/input_0.txt > output/output_0.txt
```

---

## Idea of our Approach

1. Taking the hubs as starting points for optimizing distance and fuel
2. Choose the next node based on the following priority:

   - Nearest **house** (if its hub has been visited and the house is unvisited)
   - Else, nearest **hub**(if it is unvisited)
   - Else, second nearest house, and so on...
3. If there is no enough fuel to reach the node:

   - Go to the **nearest fuel station**, refuel, then try again.
   - If still not possible, **backtrack** to the parent node and skip this one.
4. if no vaild next node backtrack to previous node and skip this one.
5. the shortest distance is calculated by calculating the shortest distance for each hub as starting point

---
## Key Components

### Algorithms Used

* **Dijkstra Algorithm**: Computes shortest paths from a source node to all other nodes.
* **Greedy Heuristic**: For each hub, the algorithm attempts to greedily deliver all packages while managing fuel.

### Important Functions in `logistics_optimization.cpp`

* `Minfuelforpathfunction()`: For a given path it calculates the minimum fuel required at starting point to make the path feasible.
* `DijkstraAlgo()`: For a given node it calculates the shortest distance and the shortest path for each other node while seeing the feasibility of path considering full fuel at starting point

---




