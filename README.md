# Optimisation-Logistics-Unleashed

Problem Statement: https://drive.google.com/file/d/1FQTwungBC_pTOypwuLhny6jmMsmEGq3M/view?usp=sharing

## Key points

### Important Functions
- `Minfuelforpathfunction()`: For a given path it calculates the minimum fuel required at starting point to make the path feasible.
- `DijkstraAlgo()`: For a given node it calculates the shortest distance and the shortest path for each other node while seeing the feasibility of path considering full fuel at starting point

### Algorithms Used
- **Dijkstra Algorithm**: Computes shortest paths from a source node to all other nodes.
- **Greedy Heuristic**: For each hub, the algorithm attempts to greedily deliver all packages while managing fuel.

### Idea
1) Taking the hubs as starting points for optimizing distance and fuel
2) Choose the next node based on the following priority:
   - Nearest **house** (if its hub has been visited and the house is unvisited)
   - Else, nearest **hub**(if it is unvisited)
   - Else, second nearest house, and so on...
3) If there is no enough fuel to reach the node:
     - Go to the **nearest fuel station**, refuel, then try again.
     - If still not possible, **backtrack** to the parent node and skip this one.
4) if no vaild next node backtrack to previous node and skip this one.
5) the shortest distance is calculated  by calculating the shortest distance for each hub as starting point


