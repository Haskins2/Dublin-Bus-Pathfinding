#include "t3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum
#include <assert.h>

#define INF 10000000
#define MAX_STRING_SIZE 40
#define maxNeighbours 20
#define maxNumVertices 7700


Graph* g = NULL;

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

Node* dequeueMIN(Graph* g, Queue* queue){
    Node* minNode = NULL;
    int minDist = 100000000;
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
        printf("\nERROR OCCURED IN DEQUEING NEGATIVE NUM %d\n", minIndex);
        int k = queue->size;
        while(queue->size != 0){
            printf("%d - dist: %d perm: %d\n", queue->array[k]->stopID, queue->array[k]->distToSource, queue->array[k]->isPermanent);
            k--;
        }
        return NULL;
    }
}

void dijkstra(Graph* g, int origin){
    Queue* Q = createQueue(g->numNodes);

    g->nodes[origin]->distToSource = 0;
    enqueue(Q, g->nodes[origin]);

    for(int i = 0; i<(g->numNodes); i++){
        if((g->nodes[i] != NULL) && (g->nodes[i] != g->nodes[origin]) && (g->nodes[i]->stopID != 0)){
            g->nodes[i]->distToSource = INF;
            g->nodes[i]->prevNode = NULL;
            enqueue(Q, g->nodes[i]);
            if(g->nodes[i]->stopID < 2){
                printf("Queued %d\n", g->nodes[i]->stopID);
            }
        }
    }

    while(Q->size != 0){
        Node* curr = dequeueMIN(g, Q);
        
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

    //free Queue memory
    free(Q->array);
    free(Q);
}

Node* createNode(int STOPID, char* NAME, float LAT, float LONG){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->stopID = STOPID;
    strcpy(newNode->name, NAME);
    newNode->latitude = LAT;
    newNode->longitude = LONG;
    newNode->isPermanent = 0;
    newNode->numNeighbours = -1;
    newNode->distToSource = INF;
    newNode->neighbours = (Node**)malloc(maxNeighbours * sizeof(Node*));
    newNode->prevNode = NULL;
    newNode->weights = (int*)malloc(maxNeighbours * sizeof(int));
    newNode->neighbours[0] = NULL;
    return newNode;
}

void insertNode(Graph* g, Node* nodeToInsert){
    g->nodes[nodeToInsert->stopID] = nodeToInsert;
}

Graph* createGraph(Node* nodeToInsert){
    Graph* newG = (Graph*)malloc(sizeof(Graph));
    newG->numNodes = maxNumVertices;
    newG->nodes = (Node**)malloc(maxNumVertices * sizeof(Node*));

    //initiate array
    for(int i=0; i < maxNumVertices; i++){
        newG->nodes[i] = NULL;
    }

    newG->nodes[nodeToInsert->stopID] = nodeToInsert;
    return newG;
}

void insertEdge(int vertex1, int vertex2, int weight){
    Node* v1 = g->nodes[vertex1];
    Node* v2 = g->nodes[vertex2];

    if(v1 == NULL){
        printf("ERROR V1 DOESNT EXIST");
    }
    if(v2 == NULL){
        printf("ERROR V2 DOESNT EXIST %d", vertex2);
    }

    //create link from and to
    int i = 0;
    while(v1->neighbours[i] != NULL){
        i++;
    }

    v1->neighbours[i] = v2;
    v1->weights[i] = weight;
    v1->numNeighbours++;
    v1->neighbours[i+1] = NULL;


    i = 0;
    while(v2->neighbours[i] != NULL){
        i++;
    }

    v2->neighbours[i] = v1;
    v2->weights[i] = weight;
    v2->numNeighbours++;
    v2->neighbours[i+1] = NULL;
}

void next_token(char *buf, FILE *f, int string_max)
{
    // start by skipping any characters we're not interested in
    buf[0] = fgetc(f);
    int quoted = 0;
    while ((!isalnum(buf[0])) && (!feof(f)) && (buf[0] != '*') && (buf[0] != '[') && (buf[0] != '{') && (buf[0] == '"'))
    {
        //move on
        if(buf[0] == '"'){ 
            quoted=!quoted; //turn on quotes
            buf[0] = fgetc(f);//get next character which will be "
            if(buf[0] == '"'){
                //then double quoted
                //making my own string here
                quoted=!quoted;
                buf[0]=' ';
                buf[1]='\0';
                return;
            }
           
        }else{
            buf[0] = fgetc(f);
        }
    }

    int i = 1;
    for (;;)
    {
        
        buf[i] = fgetc(f); // get next character from file

        if(buf[i] == '"'){ quoted=!quoted; buf[i] = fgetc(f);}
        if(buf[i] == ',' && !quoted){ break; }
        if(buf[i] == '\n'){ break; }
     

        if (feof(f))
        {
            break;
        } // file ended?
        if (i < (string_max - 1))
        {
            ++i;
        } // truncate strings that are too long
    }
    buf[i] = '\0'; // NULL terminate the string
}

int load_edges(char *fname)
{
    FILE *f;
    char buf[MAX_STRING_SIZE];
    int tmpV1;
    int tmpV2;
    int weight;

    // boiler plate code to ensure we can open the file
    f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }
    int k = 0;
    // read until the end of the file
    while (!feof(f))
    {
        next_token(buf, f, MAX_STRING_SIZE);
        if(feof(f)){
            break;
        }
        tmpV1 = atoi(buf);

        next_token(buf, f, MAX_STRING_SIZE);
        tmpV2 = atoi(buf);

        next_token(buf, f, MAX_STRING_SIZE);
        weight = atoi(buf);

 

        if(strcmp(buf, "weight") != 0){
            k++;
            insertEdge(tmpV1, tmpV2, weight);
        }
    }

    printf("Loaded %d edges\n", k);

    fclose(f);

    return 1;
}

int load_vertices(char *fname)
{
    FILE *f;
    char buf[MAX_STRING_SIZE];
    int tmpStopID;
    char tmpName[MAX_STRING_SIZE];
    float tmpLatitude;
    float tmpLongitude;
    Node* tmpNode;


    // boiler plate code to ensure we can open the file
    f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }
    int k = 0;
    // read until the end of the file
    while (!feof(f))
    {
        next_token(buf, f, MAX_STRING_SIZE);
        if(feof(f)){
            break;
        }
        tmpStopID = atoi(buf);

        next_token(buf, f, MAX_STRING_SIZE);
        strcpy(tmpName, buf);

        next_token(buf, f, MAX_STRING_SIZE);
        tmpLatitude = atof(buf);

        next_token(buf, f, MAX_STRING_SIZE);
        tmpLongitude = atof(buf);



        if(strcmp(buf, "Longitude") != 0){
            k++;

            tmpNode = createNode(tmpStopID, tmpName, tmpLatitude, tmpLongitude);
            if(k == 1){
                //then first node
                tmpNode = createNode(tmpStopID, tmpName, tmpLatitude, tmpLongitude);
                g = createGraph(tmpNode);
            }else{
                insertNode(g, tmpNode);
            }
        }
    }

    printf("Loaded %d vertices\n", k);
    fclose(f);

    return 1;
}



void shortest_path(int startNode, int endNode){
    dijkstra(g, endNode);
    Node* curr = g->nodes[startNode];
    while(curr->prevNode != NULL){
        printf("%d %s\n", curr->stopID, curr->name);
        curr = curr->prevNode;
    }

    printf("%d %s\n", curr->stopID, curr->name);

}

void free_memory (void) {
    for (int i = 2; i < g->numNodes; i++) {
        Node* node = g->nodes[i];
        if(node != NULL){
            free(node->neighbours);
            free(node->weights);
            free(node);
        }
     
    }
    free(g->nodes);
    free(g);
}



// int main(){

//     load_vertices("vertices.csv");
//     load_edges("edges.csv");

//     // dijkstra(g, 2);
//     shortest_path(300, 253);
//     // shortest_path(1194, 7469);

//     // printG(g);
//     free_memory();


//     return 0;
// }

//rather than realloacting the entire array, leave gaps. this way we dont have to linearly search when
//looking for a specific vertex
//it is assumed there are no duplicate stopIDs
