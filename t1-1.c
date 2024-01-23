#include "t1.h"
#include <stdio.h>
#include <stdlib.h>

int maxNeighbours = 4;

Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(capacity * sizeof(int));
    return queue;
}

// Function to enqueue a node
void enqueue(Queue* queue, int item) {

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to dequeue a node
int dequeue(Queue* queue) {
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

static int char2val ( char c ) {
	if ( c < 'A' || c > 'Z' ) {
		return -1;
	}

	return c-'A';
}

static char val2char(int i){
    return i+'A';
}

Graph* create_graph(int num_nodes){
    Graph* newGraph = (Graph*)malloc(sizeof(Graph));

    newGraph->numNodes = num_nodes;

    //allocate memory for array of nodes
    newGraph->nodes = (Node**)malloc(num_nodes*sizeof(Node));

    for(int i=0; i<num_nodes; i++){
        newGraph->nodes[i] = (Node*)malloc(sizeof(Node));
        newGraph->nodes[i]->value = i;
        newGraph->nodes[i]->numNeighbours = -1;
        newGraph->nodes[i]->neighbours = (Node**)malloc(maxNeighbours* sizeof(Node*));
        newGraph->nodes[i]->neighbours[0] = NULL;
        newGraph->nodes[i]->visited = 0;
    }

    return newGraph;
}

void add_edge(Graph *g, int from, int to){
    int i = 0;
    while(g->nodes[from]->neighbours[i] != NULL){
        i++;
    }

    g->nodes[from]->neighbours[i] = g->nodes[to];
    g->nodes[from]->numNeighbours++;
    g->nodes[from]->neighbours[i+1] = NULL;
}

void dfsRecursive(Graph* g, int current){
    g->nodes[current]->visited = 1;
    printf(" %c ",val2char(g->nodes[current]->value));

    int i = g->nodes[current]->numNeighbours;
    for(int j = i; j>=0; j--){
        if(g->nodes[current]->neighbours[j]->visited != 1){
            dfsRecursive(g, g->nodes[current]->neighbours[j]->value);
        }
    }
}

void dfs(Graph* g, int origin){
    for(int i = 0; i<(g->numNodes); i++){
        g->nodes[i]->visited = 0;
    }

    printf("DFS:");
    dfsRecursive(g,origin);
    printf("\n");
}


void bfs(Graph* g, int origin){

    for(int i = 0; i<(g->numNodes); i++){
        g->nodes[i]->visited = 0;
    }

    //mark v as visited
    g->nodes[origin]->visited = 1;
    //new empty queue Q
    Queue* Q = createQueue(g->numNodes);

    //add v to queue
    enqueue(Q, origin);

    printf("BFS  %c ", val2char(origin));

    //while Q is not empty
    while(Q->size != 0){
        int current = dequeue(Q);
        int numNeighbours = g->nodes[current]->numNeighbours;
        for(int i = numNeighbours; i >= 0; i--){
            if(g->nodes[current]->neighbours[i]->visited == 0){
                g->nodes[current]->neighbours[i]->visited = 1;
                printf(" %c ", val2char(g->nodes[current]->neighbours[i]->value));
                enqueue(Q, g->nodes[current]->neighbours[i]->value);
            }
        }
    }

    free(Q->array);
    free(Q);
    printf("\n");
}

void delete_graph(Graph* g){
    for(int i = 0; i < (g->numNodes); i++){
        free(g->nodes[i]->neighbours);
        free(g->nodes[i]);
    }
    free(g->nodes);
    free(g);
}



// int main(){
//     int num_nodes = 6;
//     Graph *graph = create_graph(num_nodes);
   
//     add_edge(graph, char2val('A'), char2val('E'));
//     add_edge(graph, char2val('B'), char2val('D'));
//     add_edge(graph, char2val('A'), char2val('D'));
//     add_edge(graph, char2val('A'), char2val('B'));
//     add_edge(graph, char2val('B'), char2val('C'));
//     add_edge(graph, char2val('C'), char2val('B'));
//     add_edge(graph, char2val('E'), char2val('D'));
//     add_edge(graph, char2val('D'), char2val('F'));
//     add_edge(graph, char2val('F'), char2val('C'));
//     add_edge(graph, char2val('C'), char2val('D'));

//     // print(graph, num_nodes);
//     // dfsRecursive(graph, 0);
//     dfs(graph, 0);
//     bfs(graph, 0);

//     delete_graph(graph);

//     return 0;
// }