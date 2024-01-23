#ifndef T1_H_
#define T1_H_

typedef struct Graph{
    //TODO
    int numNodes;
    struct Node** nodes; //array of pointers to neighbours
} Graph;

typedef struct Node{
    int value;
    int visited;
    int numNeighbours;
    struct Node** neighbours; //array of pointers to nodes neighbours
}Node;

typedef struct Queue{
    int front,rear,size;
    int capacity;
    int* array;
}Queue;

Queue* createQueue(int capacity);
void enqueue(Queue* queue, int item);
int dequeue(Queue* queue);

Graph* create_graph(int num_nodes); // creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
void add_edge(Graph *g, int from, int to); // adds a directed edge
void bfs(Graph* g, int origin); //implements breath first search and prints the results
void dfs(Graph* g, int origin); //implements depth first search and prints the results
void delete_graph(Graph* g);

#endif