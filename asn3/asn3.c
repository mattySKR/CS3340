// Student: Matey Skripchenko
// Student number: 250899673

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Struct representing the node
// in adj list
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

// Adj list itself
struct AdjList
{

    // Head node pointer
    struct AdjListNode *head;
};


// Graph representation structure, which
// is an array of adj lists. Array size is
// V, which is the number of vertices
struct Graph
{
    int V;
    struct AdjList* array;
};

// For creating the new adj node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Creates a graph that has V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    // Array of adj lists whose size is V
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));

    // Adj list initialization
    for(int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}
// Will add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Adding an edge from source to destination.

    struct  AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    // Adding an edge from destination to source,
    // but it will be undirected. Let's try
    // if it works
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}


// Min heap node struct
struct MinHeapNode
{
    int v;
    int dist;
};

// Min heap struct
struct MinHeap
{

    // # of current heap nodes
    int size;

    // Min heap capacity
    int capacity;

    // For decreasing the key later
    int *pos;
    struct MinHeapNode **array;
};

// Will create a new min heap node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
// Will create min heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
// Will swap the 2 nodes,
// gotta remember that I need it for heapification
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
// Will heapify at idx and updates
// node position after swapping. Then, I gotta
// use position for decreasing the key
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )
        smallest = right;
    if (smallest != idx)
    {
        // Swapping the nodes that are in the min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
        // Position swapper
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        // Node swapper
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
// Will check for emptiness of the
// min heap
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
// Will extract the min node from heap
struct MinHeapNode* extractMin(struct MinHeap*minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
    // Root storage
    struct MinHeapNode* root = minHeap->array[0];
    // Replacement of root with the last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    // Position update for the last guy
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    // Now, reducing the heap and
    // performing heapification on the root node
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}



// Will decrease the the distance for the vertex v.
// Have to use thee pos[] of min heap to get the node's
// current index in the min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // index for the vertex
    int i = minHeap->pos[v];
    // Distance value update for the node
    minHeap->array[i]->dist = dist;
    // Going up while the full tree
    // is not heaped
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Node swap with the parent node
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        // Now, want to move to the index of a parent
        i = (i - 1) / 2;
    }
}

// Will check if the vertex v is in min heap
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}
// Solution printing, I need to figure out
// the way to print things out in a more
// organized fashion
void printArr(int dist[], int n)
{
    printf("Vertex #     Distance From Given Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
// Shortest path function
void dijkstra(struct Graph* graph, int src)
{

    // Obtaining the number of vertices for the graph
    int V = graph->V;

    // Distance values for minimal weight
    // edge picking
    int dist[V];

    struct MinHeap* minHeap = createMinHeap(V);
    // Init min heap with all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    // Source vertex distance initialized to zero,
    // because it considered first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    // Min heap init size is V
    minHeap->size = V;
    // Loop for not yet final
    // nodes in the min heap
    while (!isEmpty(minHeap))
    {
        // Min distance vertex extraction
        struct MinHeapNode* minHeapNode = extractMin(minHeap);

        // That vertex number is stored (the one extracted)
        int u = minHeapNode->v;
        // Going through all the adj vertices to the one
        // extracted which is u here and then updating
        // the vals for the distance (traversal step)
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // Checking if the shortest dist to v is not yet found,
            // and making sure that the distance v -> u is
            // smaller than previously obtained one
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                // Not forgetting  to update the distance
                // in the min heap
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    // Finally, printing the found
    // shortest distance to the screen
    printArr(dist, V);


}


int main() {

    int V = 25;

    struct Graph* graph = createGraph(V);
    addEdge(graph, 20, 23, 20);
    addEdge(graph, 11, 7, 32);
    addEdge(graph, 15, 17, 23);
    addEdge(graph, 16, 21, 24);
    addEdge(graph, 3, 12, 47);
    addEdge(graph, 6, 24, 54);
    addEdge(graph, 1, 20, 63);
    addEdge(graph, 6, 9, 44);
    addEdge(graph, 16, 12, 11);
    addEdge(graph, 10, 18, 69);
    addEdge(graph, 25, 24, 54);
    addEdge(graph, 2, 13, 39);
    addEdge(graph, 1, 18, 41);
    addEdge(graph, 16, 6, 56);
    addEdge(graph, 3, 7 , 70);
    addEdge(graph, 6, 25, 73);
    addEdge(graph, 23, 22, 39);
    addEdge(graph, 4, 20, 13);
    addEdge(graph, 12, 22, 39);
    addEdge(graph, 16, 14, 49);
    addEdge(graph, 7, 22, 39);
    addEdge(graph, 23, 9, 54);
    addEdge(graph, 15, 14, 47);
    addEdge(graph, 18, 5, 49);
    addEdge(graph, 10, 5, 51);
    addEdge(graph, 5, 23, 8);
    addEdge(graph, 17, 11, 49);
    addEdge(graph, 19, 2, 76);
    addEdge(graph, 9, 18, 11);
    addEdge(graph, 4, 1, 5);
    addEdge(graph, 3, 11, 74);
    addEdge(graph, 9, 17, 70);
    addEdge(graph, 9, 14, 46);
    addEdge(graph, 17, 25, 51);
    addEdge(graph, 8, 14, 57);
    addEdge(graph, 14, 22, 49);
    addEdge(graph, 2, 21, 30);
    addEdge(graph, 4, 23, 68);
    addEdge(graph, 19, 12, 35);
    addEdge(graph, 20, 8, 26);
    addEdge(graph, 10, 23, 65);
    addEdge(graph, 17, 13, 77);
    addEdge(graph, 17, 8, 70);
    addEdge(graph, 14, 24, 39);
    addEdge(graph, 11, 21, 31);
    addEdge(graph, 2, 3, 51);
    addEdge(graph, 8, 25, 46);
    addEdge(graph, 24, 5, 38);
    addEdge(graph, 18, 21, 55);
    addEdge(graph, 9, 5, 8);
    addEdge(graph, 7, 16, 37);
    addEdge(graph, 4, 13, 60);
    addEdge(graph, 12, 3, 47);
    addEdge(graph, 15, 16, 60);
    addEdge(graph, 13, 21, 57);
    addEdge(graph, 24, 22, 66);
    addEdge(graph, 15, 5, 13);
    addEdge(graph, 15, 2, 44);
    addEdge(graph, 4, 14, 1);
    addEdge(graph, 1, 9, 47);
    addEdge(graph, 16, 5, 38);
    addEdge(graph, 12, 13, 63);
    addEdge(graph, 21, 17, 2);
    addEdge(graph, 12, 20, 55);
    addEdge(graph, 5, 25, 0);
    addEdge(graph, 8, 17, 0);
    addEdge(graph, 5, 8, 41);
    addEdge(graph, 21, 6, 54);
    addEdge(graph, 3, 10, 46);
    addEdge(graph, 22, 24, 75);
    addEdge(graph, 8, 5, 54);
    addEdge(graph, 6, 12, 65);
    addEdge(graph, 16, 25, 27);
    addEdge(graph, 21, 9, 18);
    addEdge(graph, 10, 8, 26);
    addEdge(graph, 15, 6, 66);
    addEdge(graph, 19, 8, 59);
    addEdge(graph, 22, 11, 73);
    addEdge(graph, 17, 22, 14);
    addEdge(graph, 25, 2, 5);
    addEdge(graph, 20, 21, 12);
    addEdge(graph, 17, 15, 17);
    addEdge(graph, 1, 8, 20);
    addEdge(graph, 13, 10, 61);
    addEdge(graph, 5, 11, 76);
    addEdge(graph, 23, 12, 13);
    addEdge(graph, 2, 1, 73);
    addEdge(graph, 17, 23, 15);
    addEdge(graph, 2, 14, 14);
    addEdge(graph, 11, 10, 57);
    addEdge(graph, 10, 21, 54);
    addEdge(graph, 18, 6, 37);
    addEdge(graph, 23, 13, 17);
    addEdge(graph, 23, 20, 55);
    addEdge(graph, 16, 2, 33);
    addEdge(graph, 7, 9, 58);
    addEdge(graph, 19, 3, 10);
    addEdge(graph, 8, 19, 20);
    addEdge(graph, 17, 14, 73);
    addEdge(graph, 6, 5, 48);
    addEdge(graph, 6, 16, 73);
    addEdge(graph, 17, 21, 10);
    addEdge(graph, 1, 24, 48);
    addEdge(graph, 22, 6, 63);
    addEdge(graph, 8, 6, 46);
    addEdge(graph, 1, 19, 66);
    addEdge(graph, 14, 2, 5);
    addEdge(graph, 23, 16, 66);
    addEdge(graph, 3, 13, 40);
    addEdge(graph, 18, 3, 1);
    addEdge(graph, 5, 20, 69);
    addEdge(graph, 18, 17, 51);
    addEdge(graph, 22, 3, 7);
    addEdge(graph, 24, 15, 52);
    addEdge(graph, 20, 24, 73);
    addEdge(graph, 13, 6, 44);
    addEdge(graph, 4, 8, 70);
    addEdge(graph, 5, 6, 39);
    addEdge(graph, 22, 19, 50);
    addEdge(graph, 20, 6, 26);
    addEdge(graph, 20, 17, 29);
    addEdge(graph, 11, 15, 44);
    addEdge(graph, 5, 15, 23);
    addEdge(graph, 24, 6, 74);
    addEdge(graph, 11, 8, 34);
    addEdge(graph, 5, 12, 70);
    addEdge(graph, 16, 9, 55);
    addEdge(graph, 7, 4, 58);
    addEdge(graph, 10, 9, 74);
    addEdge(graph, 22, 17, 32);
    addEdge(graph, 19, 4, 48);
    addEdge(graph, 15, 7, 52);
    addEdge(graph, 6, 21, 23);
    addEdge(graph, 14, 4, 46);
    addEdge(graph, 25, 19, 72);
    addEdge(graph, 13, 11, 35);
    addEdge(graph, 3, 17, 62);
    addEdge(graph, 17, 2, 19);
    addEdge(graph, 5, 21, 71);
    addEdge(graph, 17, 7, 42);
    addEdge(graph, 24, 12, 26);
    addEdge(graph, 10, 19, 29);
    addEdge(graph, 7, 20, 58);
    addEdge(graph, 7, 14, 33);
    addEdge(graph, 15, 19, 4);
    addEdge(graph, 23, 6, 48);
    addEdge(graph, 8, 9, 18);
    addEdge(graph, 19, 9, 59);
    addEdge(graph, 2, 16, 17);
    addEdge(graph, 5, 4, 19);
    addEdge(graph, 9, 22, 27);
    addEdge(graph, 8, 18, 14);
    addEdge(graph, 2, 17, 19);
    addEdge(graph, 14, 19, 53);
    addEdge(graph, 15, 21, 46);
    addEdge(graph, 8, 20, 24);
    addEdge(graph, 4, 18, 30);
    addEdge(graph, 25, 12, 30);
    addEdge(graph, 10, 17, 48);
    addEdge(graph, 11, 1, 62);
    addEdge(graph, 20, 25, 26);
    addEdge(graph, 23, 4, 79);
    addEdge(graph, 20, 13, 12);
    addEdge(graph, 24, 14, 6);
    addEdge(graph, 21, 13, 26);
    addEdge(graph, 12, 18, 7);
    addEdge(graph, 2, 22, 32);
    addEdge(graph, 12, 6, 15);
    addEdge(graph, 16, 15, 51);
    addEdge(graph, 10, 7, 28);
    addEdge(graph, 25, 17, 39);
    addEdge(graph, 18, 15, 0);
    addEdge(graph, 14, 8, 72);
    addEdge(graph, 2, 12, 78);
    addEdge(graph, 6, 10, 42);
    addEdge(graph, 9, 16, 71);
    addEdge(graph, 13, 24, 5);
    addEdge(graph, 3, 22, 57);
    addEdge(graph, 4, 25, 16);
    addEdge(graph, 23, 25, 14);

    dijkstra(graph, 20);

    return 0;

}