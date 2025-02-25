#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define NAME_LEN 50

typedef struct {
    int vertex;
    int weight;
} Edge;

typedef struct {
    Edge edges[MAX_NODES];
    int edge_count;
} Node;

Node graph[MAX_NODES];
char location_names[MAX_NODES][NAME_LEN];
int nodes_count;

void add_edge(int u, int v, int weight) {
    graph[u].edges[graph[u].edge_count].vertex = v;
    graph[u].edges[graph[u].edge_count].weight = weight;
    graph[u].edge_count++;

    graph[v].edges[graph[v].edge_count].vertex = u;
    graph[v].edges[graph[v].edge_count].weight = weight;
    graph[v].edge_count++;
}

int get_location_index(char *name) {
    for (int i = 0; i < nodes_count; i++) {
        if (strcmp(location_names[i], name) == 0)
            return i;
    }
    return -1;
}

int min_distance(int dist[], int visited[]) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < nodes_count; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int start, int target) {
    int dist[MAX_NODES], visited[MAX_NODES], parent[MAX_NODES];

    for (int i = 0; i < nodes_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[start] = 0;

    for (int count = 0; count < nodes_count - 1; count++) {
        int u = min_distance(dist, visited);
        if (u == -1) break;  
        visited[u] = 1;

        for (int i = 0; i < graph[u].edge_count; i++) {
            int v = graph[u].edges[i].vertex;
            int weight = graph[u].edges[i].weight;

            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    printf("Shortest path to target: ");
    int track = target;
    int path[MAX_NODES], path_size = 0;

    while (track != -1) {
        path[path_size++] = track;
        track = parent[track];
    }

    for (int i = path_size - 1; i >= 0; i--) {
        printf("%s ", location_names[path[i]]);
        if (i > 0) printf("-> ");
    }

    printf("\nMinimum Chasing Distance: %d KM\n", dist[target]);
}

int main() {
    
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    printf("Enter number of locations: ");
    scanf("%d", &nodes_count);
    getchar();  

    printf("Enter locations where ChatroLeague Leader's presence intel was found:\n");
    for (int i = 0; i < nodes_count; i++) {
        fgets(location_names[i], NAME_LEN, stdin);
        location_names[i][strcspn(location_names[i], "\n")] = 0; 
    }

    int edges;
    printf("Enter number of roads: ");
    scanf("%d", &edges);
    getchar();

    printf("Enter connections (firstLocation secondLocation distance):\n");
    for (int i = 0; i < edges; i++) {
        char start[NAME_LEN], end[NAME_LEN];
        int weight;
        scanf("%s %s %d", start, end, &weight);

        int u = get_location_index(start);
        int v = get_location_index(end);
        if (u != -1 && v != -1) {
            add_edge(u, v, weight);
        } else {
            printf("Invalid location name!\n");
            i--; 
        }
    }

    char start_loc[NAME_LEN], target_loc[NAME_LEN];
    printf("Enter NSI Operatives current location: ");
    scanf("%s", start_loc);
    printf("Enter ChatroLeague Leader's last known location: ");
    scanf("%s", target_loc);

    int start = get_location_index(start_loc);
    int target = get_location_index(target_loc);

    if (start == -1 || target == -1) {
        printf("Invalid start or target location!\n");
        return 1;
    }

    dijkstra(start, target);
    
    return 0;
}
