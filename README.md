# SmartMap Navigation

SmartMap Navigation is a C-based program that calculates optimal routes between locations using various algorithms. It supports multiple criteria such as fastest route, shortest route, fewest stops, scenic route, and a balanced route that combines multiple factors.

---

## Features

- **Fastest Route**: Uses traffic factors to calculate the quickest path.
- **Shortest Route**: Calculates the shortest distance between two locations.
- **Fewest Stops**: Finds the path with the least number of stops using BFS.
- **Scenic Route**: Prioritizes scenic value for a more enjoyable journey.
- **Balanced Route**: Combines distance, traffic, and scenic value for an optimized route.

---

## Data Structures Used

### 1. **Cost Matrices**
- **`base_dist[NUM_LOC][NUM_LOC]`**: Stores the base distances between locations in kilometers.
- **`traf_fact[NUM_LOC][NUM_LOC]`**: Stores traffic factors (higher values indicate more congestion).
- **`scenic_val[NUM_LOC][NUM_LOC]`**: Stores scenic values (higher values indicate more scenic routes).
- **`eff_cost[NUM_LOC][NUM_LOC]`**: Precomputed matrix for fastest routes (includes traffic factors).
- **`base_cost[NUM_LOC][NUM_LOC]`**: Precomputed matrix for shortest routes.
- **`scenic_cost[NUM_LOC][NUM_LOC]`**: Precomputed matrix for scenic routes.
- **`balanced_cost[NUM_LOC][NUM_LOC]`**: Precomputed matrix for balanced routes.

### 2. **Queue for BFS**
- **`queue[Q_SIZE]`**: Circular queue for BFS operations.
- **`head` and `tail`**: Indices for enqueue and dequeue operations.

### 3. **Path Arrays**
- **`parent[NUM_LOC]`**: Stores the parent of each node in the path.
- **`path[NUM_LOC]`**: Stores the final path from the source to the destination.
- **`dist[NUM_LOC]`**: Stores the shortest distance from the source to each node.

---

## Algorithms Used

1. **Dijkstra's Algorithm**: For shortest and fastest routes.
2. **A***: For heuristic-based pathfinding.
3. **Breadth-First Search (BFS)**: For finding paths with the fewest stops.

---

## Project Structure

- **`map.c`**: The main source file containing all the logic for route calculation and navigation.
- **Cost Matrices**:
  - `base_dist`: Base distances between locations.
  - `traf_fact`: Traffic factors for congestion.
  - `scenic_val`: Scenic values for routes.
  - `eff_cost`, `base_cost`, `scenic_cost`, `balanced_cost`: Precomputed cost matrices for different algorithms.

---

## How to Run

1. **Compile the Program**:
   Use the following command to compile the program:
   ```bash
   gcc -o map map.c
