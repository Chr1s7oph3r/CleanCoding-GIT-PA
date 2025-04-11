#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Structura pentru noduri
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structura pentru graf
typedef struct Graph {
    int vertices;
    int* visited;
    Node** adjacencyLists;
} Graph;

// Structura pentru coadă
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Funcție pentru a crea un nod nou
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru nod\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// Funcție pentru a crea un graf nou
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru graf\n");
        exit(EXIT_FAILURE);
    }

    graph->vertices = vertices;
    graph->adjacencyLists = (Node**)malloc(vertices * sizeof(Node*));
    graph->visited = (int*)malloc(vertices * sizeof(int));

    if (graph->adjacencyLists == NULL || graph->visited == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < vertices; i++) {
        graph->adjacencyLists[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

// Funcție pentru a adăuga o muchie în graf
void addEdge(Graph* graph, int source, int destination) {
    // Adaugă muchia de la sursă la destinație
    Node* newNode = createNode(destination);
    newNode->next = graph->adjacencyLists[source];
    graph->adjacencyLists[source] = newNode;

    // Adaugă muchia de la destinație la sursă
    newNode = createNode(source);
    newNode->next = graph->adjacencyLists[destination];
    graph->adjacencyLists[destination] = newNode;
}

// Funcție pentru a adăuga muchii în graf
void addEdges(Graph* graph, int numberOfEdges) {
    printf("Introduceți %d muchii (ex: 0 1):\n", numberOfEdges);
    for (int i = 0; i < numberOfEdges; i++) {
        int source, destination;
        printf("Muchie %d: ", i + 1);
        scanf("%d %d", &source, &destination);
        addEdge(graph, source, destination);
    }
}

// Funcție pentru a crea o coadă nouă
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// Funcție pentru a verifica dacă coada este goală
bool isEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Funcție pentru a adăuga un element în coadă
void enqueue(Queue* queue, int value) {
    Node* newNode = createNode(value);
    if (isEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Funcție pentru a scoate un element din coadă
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Coada este goală\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = queue->front;
    int value = temp->data;
    queue->front = queue->front->next;
    free(temp);
    return value;
}

// Funcție pentru parcurgerea DFS
void DFS(Graph* graph, int vertex) {
    graph->visited[vertex] = 1;
    printf("%d ", vertex);

    Node* adj_list = graph->adjacencyLists[vertex];
    while (adj_list != NULL) {
        int connected_vertex = adj_list->data;
        if (!graph->visited[connected_vertex]) {
            DFS(graph, connected_vertex);
        }
        adj_list = adj_list->next;
    }
}

// Funcție pentru parcurgerea BFS
void BFS(Graph* graph, int start) {
    Queue* queue = createQueue();
    graph->visited[start] = 1;
    enqueue(queue, start);

    while (!isEmpty(queue)) {
        int current = dequeue(queue);
        printf("%d ", current);

        Node* temp = graph->adjacencyLists[current];
        while (temp != NULL) {
            int adj_vertex = temp->data;
            if (!graph->visited[adj_vertex]) {
                graph->visited[adj_vertex] = 1;
                enqueue(queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
    free(queue);
}

// Funcție pentru a reseta lista de vizitare
void resetVisited(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

// Funcție pentru a afișa lista de adiacență
void printAdjacencyLists(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        printf("Lista de adiacență pentru nodul %d: ", i);
        Node* temp = graph->adjacencyLists[i];
        while (temp != NULL) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Funcție pentru a sorta lista de adiacență
void sortAdjacencyList(Node** head) {
    Node* current = *head;
    while (current != NULL) {
        Node* nextNode = current->next;
        while (nextNode != NULL) {
            if (current->data > nextNode->data) {
                int temp = current->data;
                current->data = nextNode->data;
                nextNode->data = temp;
            }
            nextNode = nextNode->next;
        }
        current = current->next;
    }
}

// Funcție pentru a sorta toate listele de adiacență
void sortAdjacencyLists(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        sortAdjacencyList(&graph->adjacencyLists[i]);
    }
}

int main() {
    int numberOfVertices, numberOfEdges, startVertex;

    printf("Introduceți numărul de noduri: ");
    scanf("%d", &numberOfVertices);

    printf("Introduceți numărul de muchii: ");
    scanf("%d", &numberOfEdges);

    Graph* graph = createGraph(numberOfVertices);
    addEdges(graph, numberOfEdges);

    sortAdjacencyLists(graph); // Sortează listele de adiacență
    
    printf("Listele de adiacență:\n");
    printAdjacencyLists(graph);

    printf("Introduceți nodul de start pentru DFS: ");
    scanf("%d", &startVertex);
    printf("Parcurgere DFS: ");
    DFS(graph, startVertex);
    resetVisited(graph);
    printf("\n");

    printf("Introduceți nodul de start pentru BFS: ");
    scanf("%d", &startVertex);
    printf("Parcurgere BFS: ");
    BFS(graph, startVertex);

    // Eliberare memorie (opțional)
    for (int i = 0; i < graph->vertices; i++) {
        Node* temp = graph->adjacencyLists[i];
        while (temp != NULL) {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjacencyLists);
    free(graph->visited);
    free(graph);

    return 0;
}
