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
2) Taking a hub as starting point it reachs the nearest house if its hub is visited and house is unvisited else it vists nearest hub if it is unvisited else
   it vists 2nd nearest house if its hub is visited and house is unvisited else it vists 2nd nearest hub if it is unvisited so on.. untill one is selected
3) while reaching a node it also checks the feasibility of going to that node if going to the node is not feasible then it goes to the nearest fuel station
   and again try to reach the previous node.if this path is not feasible then it removes the current node and goes back to its parent node and reaches next node after removed node in step 2.
4) if no next node is selected it removes the current node and goes back to its parent node and reaches next node after removed node in step 2.
5) the shortest distance is calculated  by calculating the shortest distance for each hub as starting point


