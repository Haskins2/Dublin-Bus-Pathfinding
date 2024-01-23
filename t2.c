#include "t2.h"
#include <stdio.h>
#include <stdlib.h>

#define INF 9999

int maxNeighbours = 6;

Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (Node**)malloc(capacity * sizeof(Node*));
    return queue;
}

void enqueue(Queue* queue, Node* item) {
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

Node* dequeue(Queue* queue) {
    Node* item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

//dequeues node with minimum distToSource
Node* dequeueMIN(Graph* g, Queue* queue){
    Node* minNode = NULL;
    int minDist = 10000;
    int minIndex = -1;
    for(int i=0; i<queue->size; i++){
        Node* curr = queue->array[i];
        if(curr->isPermanent == 0){ //if not permanent
            if(curr->distToSource < minDist){
                minDist = curr->distToSource;
                minNode = curr;
                minIndex = i;
            }
        }
    }

    if(minIndex != -1){
        for(int i = minIndex; i < queue->size - 1; i++){
            queue->array[i] = queue->array[i + 1];
        }
        queue->size = queue->size - 1;
        return minNode;

    }else{
        printf("\nERROR OCCURED IN DEQUEING NEGATIVE NUM\n");
        return NULL;
    }
}



static char val2char(int i){
    return i +'A';
}

Graph* create_graph(int num_nodes){
    Graph* newGraph = (Graph*)malloc(sizeof(Graph));

    newGraph->numNodes = num_nodes;

    //allocate memory for array of nodes
    newGraph->nodes = (Node**)malloc(num_nodes*sizeof(Node));

    //storing numbers from 0-25
    for(int i=0; i<num_nodes; i++){
        newGraph->nodes[i] = (Node*)malloc(sizeof(Node));
        newGraph->nodes[i]->value = i;
        newGraph->nodes[i]->numNeighbours = -1;
        newGraph->nodes[i]->distToSource = 0;
        newGraph->nodes[i]->isPermanent = 0;
        newGraph->nodes[i]->neighbours = (Node**)malloc(maxNeighbours * sizeof(Node*));
        newGraph->nodes[i]->prevNode = NULL;
        newGraph->nodes[i]->weights = (int*)malloc(maxNeighbours * sizeof(int));
        newGraph->nodes[i]->neighbours[0] = NULL;
        newGraph->nodes[i]->visited = 0;
    }

    return newGraph;
}

void add_edge(Graph *g, int from, int to, int weight){
    int i = 0;
    while(g->nodes[from]->neighbours[i] != NULL){
        i++;
    }

    g->nodes[from]->neighbours[i] = g->nodes[to];
    g->nodes[from]->weights[i] = weight;
    g->nodes[from]->numNeighbours++;
    g->nodes[from]->neighbours[i+1] = NULL;

    i = 0;
    while(g->nodes[to]->neighbours[i] != NULL){
        i++;
    }

    g->nodes[to]->neighbours[i] = g->nodes[from];
    g->nodes[to]->weights[i] = weight;
    g->nodes[to]->numNeighbours++;
    g->nodes[to]->neighbours[i+1] = NULL;

}

void print(Graph* g){
    for(int i = 0; i<g->numNodes; i++){
        printf("From: %c DistToSource: %d\n", val2char(i), g->nodes[i]->distToSource);
        int numNeighbours = g->nodes[i]->numNeighbours;
        if(numNeighbours != -1){
            for(int j = numNeighbours; j>=0; j--){
                printf("To: %c", val2char(g->nodes[i]->neighbours[j]->value));
                printf("\t weight: %d", g->nodes[i]->weights[j]);
                printf("\t dist to source: %d\n", g->nodes[i]->neighbours[j]->distToSource);
            }
        }

        printf("---\n");
    }
}


void dijkstra(Graph* g, int origin){
    Queue* Q = createQueue(g->numNodes);

    g->nodes[origin]->distToSource = 0;
    enqueue(Q, g->nodes[origin]);

    for(int i = 0; i<(g->numNodes); i++){
        if(g->nodes[i] != g->nodes[origin]){
            g->nodes[i]->distToSource = INF;
            g->nodes[i]->prevNode = NULL;
            enqueue(Q, g->nodes[i]);
        }
    }

    while(Q->size != 0){
        Node* curr = dequeueMIN(g, Q);
        printf("%c ", val2char(curr->value));

        //visit each unvisited neighbour 
        int numNeighbours = curr->numNeighbours;
        for(int i = numNeighbours; i>=0; i--){
            //if not visited
            if(curr->neighbours[i]->isPermanent != 1){
                //calculate distance to source
                int tmpDist = (curr->distToSource) + (curr->weights[i]);
                //if distance calculated is less than whats already in there
                if(tmpDist < curr->neighbours[i]->distToSource){
                    curr->neighbours[i]->distToSource = tmpDist;
                    curr->neighbours[i]->prevNode = curr;
                }
            }

        }
        curr->isPermanent = 1;
    }

    printf("\n");
    for(int i = 0; i<g->numNodes; i++){
        printf("The length of the shortest path between %c and %c is %d\n", val2char(g->nodes[origin]->value), val2char(i), g->nodes[i]->distToSource);
    }
    //free Q here as cant access Q in delete_graph()
    free(Q->array);
    free(Q);
}


static int char2val ( char c ) {
	if ( c < 'A' || c > 'Z' ) {
		return -1;
	}
	return c-'A';
}



void delete_graph(Graph* g) {
    for (int i = 0; i < g->numNodes; i++) {
        free(g->nodes[i]->neighbours);
        free(g->nodes[i]->weights);
        free(g->nodes[i]);
    }
    free(g->nodes);
    free(g);
}

void printPath(Graph* g, int i){
    Node* node = g->nodes[i];
    while(node->prevNode != NULL){
        printf("%c -> ", val2char(node->value));
        node = node->prevNode;
    }
}


// int main(){
//     int size = 7;
//     Graph* graph = create_graph(size);
//     add_edge(graph, char2val('A'), char2val('B'),1);
//     add_edge(graph, char2val('A'), char2val('C'),3);
//     add_edge(graph, char2val('A'), char2val('F'), 10);
//     add_edge(graph, char2val('B'), char2val('G'), 2);
//     add_edge(graph, char2val('B'), char2val('D'), 7);
//     add_edge(graph, char2val('B'), char2val('E'), 5);
//     add_edge(graph, char2val('B'), char2val('C'), 1);
//     add_edge(graph, char2val('C'), char2val('D'), 9);
//     add_edge(graph, char2val('C'), char2val('E'), 3);
//     add_edge(graph, char2val('D'), char2val('G'), 12);
//     add_edge(graph, char2val('D'), char2val('F'), 1);
//     add_edge(graph, char2val('D'), char2val('E'), 2);
//     add_edge(graph, char2val('E'), char2val('F'), 2);
//     // add_edge(graph, char2val('E'), char2val('D'), 2); //wasnt provided?
//     dijkstra(graph, char2val('F'));
//     // printPath(graph, char2val('D'));
//     // printF(graph);
//     delete_graph(graph);

//     //queue testing
//     // Queue* Q = createQueue(3);
//     // Node* tmp1 = (Node*)malloc(sizeof(Node));
//     // tmp1->distToSource = 6;
//     // tmp1->isPermanent = 0;
//     // enqueue(Q, tmp1);
//     // Node* tmp2 = (Node*)malloc(sizeof(Node));
//     // tmp2->distToSource = 2;
//     // tmp2->isPermanent = 0;
//     // enqueue(Q,tmp2);
//     // Node* tmp3 = (Node*)malloc(sizeof(Node));
//     // tmp3->distToSource = 4;
//     // tmp3->isPermanent = 0;
//     // enqueue(Q, tmp3);
//     // Node* result = dequeueMIN(graph, Q);
//     // printf("%d\n", result->distToSource);

//     return 0;
// }