
#ifndef T3_H_
#define T3_H_

#define MAX_STRING_SIZE 40 //max length of a string


typedef struct Graph{
    //TODO
    int numNodes;
    struct Node** nodes; //array of pointers to neighbours
} Graph;

typedef struct Node{
    int stopID;
    float latitude;
    float longitude;
    char name[MAX_STRING_SIZE];    
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


int load_edges ( char *fname ); //loads the edges from the CSV file of name fname
int load_vertices ( char *fname );  //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory ( void ) ; // frees any memory that was used

void insertNode(Graph* g, Node* nodeToInsert); // creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
Graph* createGraph(Node* firstNode);
void add_edge(Graph *g, int from, int to, int weight); // adds an undirected weighted edge between from and to
void dijkstra(Graph* g, int origin); // implements the dijkstra algorithm and prints the order in which the nodes are made permament, and the length of the shortest path between the origin node and all the other nodes
void delete_graph(Graph* g);

#endif