#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LOC 12 // Number of Locations
#define INF INT_MAX // Represents infinity (no direct path)
#define Q_SIZE (NUM_LOC * 2) // Queue size for BFS
#define ALG_COUNT 5 // Number of available algorithms

// Location names with index reference
const char *Loc_names[NUM_LOC] = {
    "Gachibowli", // 0
    "Hitech City", // 1
    "Kondapur", // 2
    "Jubilee Hills", // 3
    "Banjara Hills", // 4
    "Mehdipatnam", // 5
    "Charminar", // 6
    "Secunderabad Sth", // 7
    "RGIA Airport", // 8
    "Necklace Road", // 9
    "Vasavi College", // 10
    "Financial District" // 11
};

// Algorithm names for selection
const char *alg_names[ALG_COUNT] = {
    "Fastest Route (A* with traffic)",
    "Shortest Route (distance-based)",
    "Fewest Stops (BFS)",
    "Scenic Route (weighted scenic value)",
    "Balanced Route (multi-criteria optimization)"
};

// Base distances in kilometers
int base_dist[NUM_LOC][NUM_LOC] = {
    {0, 6, 5, 8, 14, 16, 18, 25, 20, 35, 15, 19},
    {6, 0, 9, 17, 24, 22, 34, 14, 20, 8, 12, 18},
    {5, 9, 0, 7, 6, 15, 22, 18, 32, 12, 17, 14},
    {8, 17, 7, 0, 3, 10, 18, 15, 30, 6, 13, 16},
    {14, 24, 6, 3, 0, 4, 15, 13, 28, 5, 11, 16},
    {16, 22, 15, 10, 4, 0, 8, 12, 25, 9, 6, 25},
    {18, 34, 22, 18, 15, 8, 0, 18, 14, 16, 20, 18},
    {25, 14, 18, 15, 13, 12, 18, 0, 18, 33, 17, 20},
    {20, 20, 32, 30, 28, 25, 14, 18, 0, 18, 33, 26},
    {35, 8, 12, 6, 5, 9, 19, 17, 20, 0, 13, 14},
    {15, 12, 17, 13, 11, 6, 14, 7, 26, 13, 0, 9},
    {19, 18, 14, 16, 6, 25, 18, 20, 26, 14, 9, 0}
};

// Traffic factors (higher values mean more congestion)
float traf_fact[NUM_LOC][NUM_LOC] = {
    {1.0, 1.6, 1.5, 1.2, 1.3, 1.4, 1.3, 1.2, 1.1, 1.0, 1.2, 1.2},
    {1.6, 1.0, 1.4, 2.2, 2.0, 1.8, 1.5, 1.3, 1.0, 1.6, 1.7, 1.3},
    {1.5, 1.4, 1.0, 1.6, 1.7, 1.5, 1.4, 1.2, 1.0, 1.3, 1.4, 1.2},
    {1.2, 2.2, 1.6, 1.0, 1.3, 1.8, 1.6, 1.4, 1.0, 1.5, 1.6, 1.2},
    {1.3, 2.0, 1.7, 1.3, 1.0, 2.5, 1.9, 1.5, 1.0, 1.4, 1.5, 1.3},
    {1.4, 1.8, 1.5, 1.8, 2.5, 1.0, 1.7, 1.6, 1.2, 1.6, 1.3, 1.8},
    {1.3, 1.5, 1.4, 1.6, 1.9, 1.7, 1.0, 1.5, 1.4, 1.7, 1.2, 1.3},
    {1.2, 1.3, 1.2, 1.4, 1.5, 1.6, 1.5, 1.0, 1.0, 1.3, 1.5, 1.2},
    {1.1, 1.0, 1.0, 1.0, 1.0, 1.2, 1.4, 1.0, 1.0, 1.0, 1.1, 1.1},
    {1.0, 1.6, 1.3, 1.5, 1.4, 1.6, 1.7, 1.3, 1.0, 1.0, 1.4, 1.4},
    {1.2, 1.7, 1.4, 1.6, 1.5, 1.3, 1.2, 1.5, 1.1, 1.4, 1.0, 1.3},
    {1.2, 1.3, 1.2, 1.2, 1.3, 1.8, 1.3, 1.2, 1.1, 1.4, 1.3, 1.0}
};

// Scenic value ratings (0-10, higher is more scenic)
int scenic_val[NUM_LOC][NUM_LOC] = {
    {0, 3, 4, 5, 4, 2, 3, 1, 2, 7, 3, 5},
    {3, 0, 3, 6, 5, 3, 2, 1, 2, 6, 3, 4},
    {4, 3, 0, 5, 4, 2, 1, 1, 2, 5, 2, 4},
    {5, 6, 5, 0, 8, 4, 3, 3, 2, 8, 4, 3},
    {4, 5, 4, 8, 0, 5, 4, 3, 2, 8, 5, 2},
    {2, 3, 2, 4, 5, 0, 6, 3, 3, 5, 4, 3},
    {3, 2, 1, 3, 4, 6, 0, 4, 4, 5, 4, 2},
    {1, 1, 1, 3, 3, 3, 4, 0, 2, 6, 3, 1},
    {2, 2, 2, 2, 2, 3, 4, 2, 0, 3, 2, 3},
    {7, 6, 5, 8, 8, 5, 5, 6, 3, 0, 5, 4},
    {3, 3, 2, 4, 5, 4, 4, 3, 2, 5, 0, 3},
    {5, 4, 4, 3, 2, 3, 2, 1, 3, 4, 3, 0}
};

// Matrices to store costs
int eff_cost[NUM_LOC][NUM_LOC]; // For fastest (includes traffic)
int base_cost[NUM_LOC][NUM_LOC]; // For shortest and BFS
int scenic_cost[NUM_LOC][NUM_LOC]; // For scenic route
int balanced_cost[NUM_LOC][NUM_LOC]; // For balanced route

// Calculate effective distances with traffic
void calc_eff_dist(int target[NUM_LOC][NUM_LOC]) {
    for (int i = 0; i < NUM_LOC; i++) {
        for (int j = 0; j < NUM_LOC; j++) {
            if (i == j) {
                target[i][j] = 0;
            } else if (base_dist[i][j] == INF) {
                target[i][j] = INF;
            } else {
                float eff_dist = base_dist[i][j] * traf_fact[i][j];
                target[i][j] = (int)(eff_dist + 0.5); // Round to nearest

                if (target[i][j] < base_dist[i][j]) {
                    target[i][j] = base_dist[i][j];
                }
                if (base_dist[i][j] != 0 && traf_fact[i][j] > (float)INF / base_dist[i][j]) {
                    target[i][j] = INF;
                } else if (eff_dist > INF) {
                    target[i][j] = INF;
                }
            }
        }
    }
}

// Copy base distances to target matrix
void use_base_dist(int target[NUM_LOC][NUM_LOC]) {
    for (int i = 0; i < NUM_LOC; i++) {
        memcpy(target[i], base_dist[i], NUM_LOC * sizeof(int));
    }
}

// Calculate scenic route costs (lower cost for higher scenic value)
void calc_scenic_cost(int target[NUM_LOC][NUM_LOC]) {
    for (int i = 0; i < NUM_LOC; i++) {
        for (int j = 0; j < NUM_LOC; j++) {
            if (i == j) {
                target[i][j] = 0;
            } else if (base_dist[i][j] == INF) {
                target[i][j] = INF;
            } else {
                // Inverse relationship - more scenic means lower cost
                // Scale factor of 10 to keep distances meaningful
                target[i][j] = base_dist[i][j] * 10 / (scenic_val[i][j] + 1);
            }
        }
    }
}

// Calculate balanced cost (combination of distance, traffic, and scenic value)
void calc_balanced_cost(int target[NUM_LOC][NUM_LOC]) {
    for (int i = 0; i < NUM_LOC; i++) {
        for (int j = 0; j < NUM_LOC; j++) {
            if (i == j) {
                target[i][j] = 0;
            } else if (base_dist[i][j] == INF) {
                target[i][j] = INF;
            } else {
                // Weighted sum of factors:
                // 40% base distance, 30% traffic factor, 30% scenic factor (inverse)
                float dist_component = base_dist[i][j] * 0.4;
                float traffic_component = base_dist[i][j] * traf_fact[i][j] * 0.3;
                float scenic_component = base_dist[i][j] * 10.0 / (scenic_val[i][j] + 1) * 0.3;
                target[i][j] = (int)(dist_component + traffic_component + scenic_component + 0.5);
            }
        }
    }
}

// Get path from parent array and store in path_array
int get_path(int parent[], int dest, int path_array[]) {
    int path_len = 0;
    int curr = dest;

    // Build path backwards
    while (curr != -1) {
        path_array[path_len++] = curr;
        curr = parent[curr];
    }

    // Reverse the path
    for (int i = 0; i < path_len / 2; i++) {
        int temp = path_array[i];
        path_array[i] = path_array[path_len - 1 - i];
        path_array[path_len - 1 - i] = temp;
    }

    return path_len;
}

// Print path from path array
void print_path(int path[], int path_len) {
    for (int i = 0; i < path_len; i++) {
        printf("%s", Loc_names[path[i]]);
        if (i < path_len - 1) printf(" -> ");
    }
}

// Heuristic function for A* (straight-line distance approximation)
int heuristic(int node, int goal) {
    return 0; // Using 0 makes A* behave like Dijkstra
}

// Find node with minimum cost for Dijkstra/A*
int find_min_node(int cost[], bool visited[], int goal, bool use_h, int n) {
    int min_cost = INF;
    int min_f = INF;
    int min_idx = -1;

    for (int v = 0; v < n; v++) {
        if (!visited[v] && cost[v] != INF) {
            int f_score = cost[v]; // g_score

            if (use_h) {
                int h = heuristic(v, goal);
                if (h != INF && cost[v] <= INF - h) {
                    f_score += h;
                } else {
                    f_score = INF;
                }
            }

            if (f_score < min_f) {
                min_f = f_score;
                min_idx = v;
            }
        }
    }

    return min_idx;
}

// Dijkstra/A* algorithm with specified cost matrix
bool shortest_path(int src, int goal, int cost_matrix[NUM_LOC][NUM_LOC], 
    int dist[], int parent[], bool use_h) {
bool visited[NUM_LOC];

for (int i = 0; i < NUM_LOC; i++) {
dist[i] = INF;
visited[i] = false;
parent[i] = -1;
}

dist[src] = 0;

for (int count = 0; count < NUM_LOC; count++) {
int u = find_min_node(dist, visited, goal, use_h, NUM_LOC);

if (u == -1 || dist[u] == INF) break;
if (use_h && u == goal) break;

visited[u] = true;

for (int v = 0; v < NUM_LOC; v++) {
if (!visited[v] && cost_matrix[u][v] != INF && dist[u] != INF) {
 long long path_dist = (long long)dist[u] + cost_matrix[u][v];
 if (path_dist < dist[v]) {
     if (path_dist <= INF) {
         dist[v] = (int)path_dist;
     } else {
         dist[v] = INF;
     }
     parent[v] = u;
 }
}
}
}

return (dist[goal] != INF);
}

// BFS Queue operations
int queue[Q_SIZE];
int head = 0, tail = 0;

void enqueue(int node) {
queue[tail++] = node;
if (tail >= Q_SIZE) tail = 0;
}

int dequeue() {
int node = queue[head++];
if (head >= Q_SIZE) head = 0;
return node;
}

bool is_queue_empty() {
return head == tail;
}

// BFS to find path with fewest stops
int bfs_shortest(int start, int end, int parent[]) {
bool visited[NUM_LOC];
int hops[NUM_LOC];

for (int i = 0; i < NUM_LOC; i++) {
visited[i] = false;
parent[i] = -1;
hops[i] = -1;
}

head = tail = 0;
visited[start] = true;
hops[start] = 0;
enqueue(start);

while (!is_queue_empty()) {
int u = dequeue();

if (u == end) return hops[u];

for (int v = 0; v < NUM_LOC; v++) {
if (base_dist[u][v] != INF && !visited[v]) {
 visited[v] = true;
 hops[v] = hops[u] + 1;
 parent[v] = u;
 enqueue(v);
}
}
}

return -1;
}

// Calculate total distance of a path
float calc_total_dist(int path[], int path_len) {
    float total = 0;
    for (int i = 0; i < path_len - 1; i++) {
        total += base_dist[path[i]][path[i + 1]];
    }
    return total;
}

// Calculate total time of a path (considering traffic factors)
float calc_total_time(int path[], int path_len) {
    float total = 0;
    for (int i = 0; i < path_len - 1; i++) {
        total += base_dist[path[i]][path[i + 1]] * traf_fact[path[i]][path[i + 1]];
    }
    return total;
}

// Calculate scenic score of a path
float calc_scenic_score(int path[], int path_len) {
    float total = 0;
    for (int i = 0; i < path_len - 1; i++) {
        total += scenic_val[path[i]][path[i + 1]];
    }
    return total;
}

int main() {
    int start, end, alg_choice;

    printf("----------------------------------------\n");
    printf("          SmartMap Navigation           \n");
    printf("----------------------------------------\n");
    printf("Locations:\n");

    for (int i = 0; i < NUM_LOC; i++) {
        printf("%d: %s\n", i, Loc_names[i]);
    }

    printf("----------------------------------------\n");
    printf("Enter start location ID: ");

    if (scanf("%d", &start) != 1 || start < 0 || start >= NUM_LOC) {
        printf("Invalid start ID.\n");
        return 1;
    }

    printf("Enter end location ID: ");

    if (scanf("%d", &end) != 1 || end < 0 || end >= NUM_LOC) {
        printf("Invalid end ID.\n");
        return 1;
    }

    printf("----------------------------------------\n");
    printf("Available Routing Algorithms:\n");

    for (int i = 0; i < ALG_COUNT; i++) {
        printf("%d: %s\n", i, alg_names[i]);
    }

    printf("----------------------------------------\n");
    printf("Select algorithm (0-%d) or -1 for all: ", ALG_COUNT - 1);

    if (scanf("%d", &alg_choice) != 1 || alg_choice < -1 || alg_choice >= ALG_COUNT) {
        printf("Invalid algorithm choice.\n");
        return 1;
    }

    printf("----------------------------------------\n");
    printf("Calculating routes from %s to %s...\n", Loc_names[start], Loc_names[end]);
    printf("----------------------------------------\n");

    // Precompute cost matrices
    calc_eff_dist(eff_cost);
    use_base_dist(base_cost);
    calc_scenic_cost(scenic_cost);
    calc_balanced_cost(balanced_cost);

    // Arrays for path storage and results
    int parent[NUM_LOC], path[NUM_LOC], dist[NUM_LOC];
    int path_len = 0;
    bool path_found = false;

    // Handle algorithm selection
    if (alg_choice == -1 || alg_choice == 0) {
        printf("Fastest Route:\n");
        path_found = shortest_path(start, end, eff_cost, dist, parent, false);
        if (path_found) {
            path_len = get_path(parent, end, path);
            print_path(path, path_len);
            printf("\nTotal Distance: %.2f km\n", calc_total_dist(path, path_len));
            printf("Total Time: %.2f units\n", calc_total_time(path, path_len));
        } else {
            printf("No path found.\n");
        }
        printf("----------------------------------------\n");
    }

    if (alg_choice == -1 || alg_choice == 1) {
        printf("Shortest Route:\n");
        path_found = shortest_path(start, end, base_cost, dist, parent, false);
        if (path_found) {
            path_len = get_path(parent, end, path);
            print_path(path, path_len);
            printf("\nTotal Distance: %.2f km\n", calc_total_dist(path, path_len));
        } else {
            printf("No path found.\n");
        }
        printf("----------------------------------------\n");
    }

    if (alg_choice == -1 || alg_choice == 2) {
        printf("Fewest Stops:\n");
        int stops = bfs_shortest(start, end, parent);
        if (stops != -1) {
            path_len = get_path(parent, end, path);
            print_path(path, path_len);
            printf("\nTotal Stops: %d\n", stops);
        } else {
            printf("No path found.\n");
        }
        printf("----------------------------------------\n");
    }

    if (alg_choice == -1 || alg_choice == 3) {
        printf("Scenic Route:\n");
        path_found = shortest_path(start, end, scenic_cost, dist, parent, false);
        if (path_found) {
            path_len = get_path(parent, end, path);
            print_path(path, path_len);
            printf("\nScenic Score: %.2f\n", calc_scenic_score(path, path_len));
        } else {
            printf("No path found.\n");
        }
        printf("----------------------------------------\n");
    }

    if (alg_choice == -1 || alg_choice == 4) {
        printf("Balanced Route:\n");
        path_found = shortest_path(start, end, balanced_cost, dist, parent, false);
        if (path_found) {
            path_len = get_path(parent, end, path);
            print_path(path, path_len);
            printf("\nTotal Distance: %.2f km\n", calc_total_dist(path, path_len));
            printf("Total Time: %.2f units\n", calc_total_time(path, path_len));
            printf("Scenic Score: %.2f\n", calc_scenic_score(path, path_len));
        } else {
            printf("No path found.\n");
        }
        printf("----------------------------------------\n");
    }

    return 0;
}

