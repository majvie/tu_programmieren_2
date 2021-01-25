// Einfache Implementierung einer Schlange
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "iueduma/duma.h"

// Struktur für die Daten der Schlange
typedef struct Queue_s {
    long size;
    node_t **queue;
} Queue;

// Eine Struktur für die Daten des Stapels
typedef struct Stack_s{
    long size;
    node_t **stack; // Adresse für dynamischen Speicher
} Stack;

// Speicher initialisieren
void initialise_queue(Queue *queue, long size) {
    queue->size = 0;
    queue->queue = malloc(size * sizeof(node_t));
}

// dynamischen Speicher wieder löschen <-- WICHTIG!!!!
void delete_queue(Queue *queue) {
    if(queue->queue) {
        free(queue->queue);
        queue->queue = 0;
    } else {
        printf("ERROR: Queue was already 0!");
    }
    queue->size = 0;
}

// Element am Ende einfügen
void put(Queue *queue, node_t *element) {
    queue->queue[queue->size] = element;
    ++(queue->size);
}

// Wenn das erste Element entfernt wird, müssen alle anderen nach Vorne geschoben werden
node_t* get(Queue *queue) {
    long i = 0;
    // Element von Anfang entfernen
    node_t* element = queue->queue[0];
    --(queue->size);
    // Alle Elemente danach nach links schieben
    for(i = 0; i < queue->size; ++i) {
        queue->queue[i] = queue->queue[i+1];
    }
    return element;
}

// Speicher initialiseren
void initialise_stack(Stack *stack, long size) {
    stack->stack = malloc(size * sizeof(long));
    stack->size = 0;
}

// dynamischen Speicher wieder löschen <-- WICHTIG!!!!
void delete_stack(Stack *stack) {
    if(stack->stack) {
        free(stack->stack);
        stack->stack = 0;
    } else {
        printf("ERROR: Stack was already 0!");
    }
}

void push(Stack *stack, node_t *element) {
    stack->stack[stack->size] = element; // Fügt ein neues Element oben auf dem Stapel hinzu
    ++(stack->size);                     // Erhöht die Stapelgröße um eins
}

node_t *pop(Stack *stack) {
    return stack->stack[--(stack->size)]; // Liefert das oberste Element und reduziert die Stapelgröße um eins
}