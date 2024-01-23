#ifndef T2_H_
#define T2_H_

#define MAX_VERTICES 10 // you can assume that the graph has at most 10 vertex
typedef struct Graph{
    //TODO
    int numNodes;
    struct Node** nodes; //array of pointers to neighbours
} Graph;

typedef struct Node{
    int value;
    int visited;
    int distToSource;
    int isPermanent;
    int numNeighbours;
    struct Node* prevNode;
    struct Node** neighbours; //array of pointers to nodes neighbours
    int *weights;
}Node;

typedef struct Queue{
    int front,rear,size;
    int capacity;
    struct Node** array;
}Queue;


Graph* create_graph(int num_nodes); // creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
void add_edge(Graph *g, int from, int to, int weight); // adds an undirected weighted edge between from and to
void dijkstra(Graph* g, int origin); // implements the dijkstra algorithm and prints the order in which the nodes are made permament, and the length of the shortest path between the origin node and all the other nodes
void delete_graph(Graph* g);

#endif