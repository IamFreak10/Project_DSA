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
    int edgeCount;
} Node;

Node graph[MAX_NODES];
char locationNames[MAX_NODES][NAME_LEN];
int nodesCount;

void addEdge(int u, int v, int weight) {
    graph[u].edges[graph[u].edgeCount].vertex = v;
    graph[u].edges[graph[u].edgeCount].weight = weight;
    graph[u].edgeCount++;

    graph[v].edges[graph[v].edgeCount].vertex = u;
    graph[v].edges[graph[v].edgeCount].weight = weight;
    graph[v].edgeCount++;
}

int getLocationIndex(char *name) {
    for (int i = 0; i < nodesCount; i++) {
        if (strcmp(locationNames[i], name) == 0)
            return i;
    }
    return -1;
}

int minDistance(int dist[], int visited[]) {
    int min = INT_MAX, minIndex = -1;
    for (int v = 0; v < nodesCount; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void dijkstra(int start, int target) {
    int dist[MAX_NODES], visited[MAX_NODES], parent[MAX_NODES];

    for (int i = 0; i < nodesCount; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[start] = 0;

    for (int count = 0; count < nodesCount - 1; count++) {
        int u = minDistance(dist, visited);
        if (u == -1) break;
        visited[u] = 1;

        for (int i = 0; i < graph[u].edgeCount; i++) {
            int v = graph[u].edges[i].vertex;
            int weight = graph[u].edges[i].weight;

            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    printf("Shortest path to target Chatroleague Leader: ");
    int track = target;
    int path[MAX_NODES], pathSize = 0;

    while (track != -1) {
        path[pathSize++] = track;
        track = parent[track];
    }

    for (int i = pathSize - 1; i >= 0; i--) {
        printf("%s", locationNames[path[i]]);
        if (i > 0) printf(" -> ");
    }

    printf("\nMinimum Chasing Distance: %d KM\n", dist[target]);
}

int main() {
    printf("Enter number of locations: ");
    scanf("%d", &nodesCount);
    getchar();

    printf("Enter locations where ChatroLeague Leader's presence intel was found:\n");
    for (int i = 0; i < nodesCount; i++) {
        fgets(locationNames[i], NAME_LEN, stdin);
        locationNames[i][strcspn(locationNames[i], "\n")] = 0;
    }

    int edges;
    printf("Enter number of paths between locations: ");
    scanf("%d", &edges);
    getchar();

    for (int i = 0; i < edges; i++) {
        char start[NAME_LEN], end[NAME_LEN];
        int weight;
        printf("Enter path (location1 location2 distance): ");
        scanf("%s %s %d", start, end, &weight);
        
        int u = getLocationIndex(start);
        int v = getLocationIndex(end);
        if (u != -1 && v != -1) {
            addEdge(u, v, weight);
        }
    }

    char startLoc[NAME_LEN], targetLoc[NAME_LEN];
    printf("Enter start and target locations: ");
    scanf("%s %s", startLoc, targetLoc);
    
    int start = getLocationIndex(startLoc);
    int target = getLocationIndex(targetLoc);

    if (start == -1 || target == -1) {
        printf("Invalid start or target location!\n");
    } else {
        dijkstra(start, target);
    }
    
    return 0;
}
