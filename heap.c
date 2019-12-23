//heap.c
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//cria uma heap
Heap* createHeap() {
	Heap* h = calloc(1, sizeof(Heap));
	h->size = 0;
	h->vect = NULL;

	return h;
}


//cria um elemento par
//recebe: um id (id do vertice de destino), um peso (peso da aresta)
Pair* createPair(int id, int weight) {
	Pair* pair = calloc(1, sizeof(Pair));
	pair->weight = weight;
	pair->id = id;
	//pair->data = data;

	return pair;
}

//compara dois elementos pares, retorna a diferença de peso entre eles
int comparePair(Pair* a, Pair* b) {
	return a->weight - b->weight;
}


//insere um novo elemento par na heap e, em seguida, recalcula a heap
void maxHeapifyOnInsert(Heap* heap) {
	int child = heap->size - 1;
	int parent = child % 2 ? (child / 2) : (child / 2) - 1;
	
	while (parent >= 0 && comparePair(heap->vect[child], heap->vect[parent]) > 0) {
		Pair* tmp = heap->vect[parent];
		heap->vect[parent] = heap->vect[child];
		heap->vect[child] = tmp;

		child = parent;
		parent = child % 2 ? (child / 2) : (child / 2) - 1;
	}
}

//ordena uma heap
//recebe: a heap a ordenar, o nó pai
void maxHeapify(Heap* heap, int parent) {
	
	int child = (parent * 2) + 1;

	if (child >= heap->size) return;

	if (comparePair(heap->vect[child], heap->vect[parent]) > 0 || 
		(child + 1 < heap->size && comparePair(heap->vect[child + 1], heap->vect[parent]) > 0)) {

		if (child + 1 < heap->size && comparePair(heap->vect[child], heap->vect[child + 1]) < 0)
			child += 1;

		Pair* tmp = heap->vect[child];
		heap->vect[child] = heap->vect[parent];
		heap->vect[parent] = tmp;

		maxHeapify(heap, child);
	}
}


//insere na heap um par
//recebe: a heap, o id do par e o peso do par
void insertInHeap(Heap* heap, int id, int weight) {

	heap->vect = realloc(heap->vect, ++heap->size * sizeof(Pair**));
	
	heap->vect[heap->size - 1] = createPair(id, weight);

	maxHeapifyOnInsert(heap);
}

//remove o elemento no começo da heap
//recebe: a heap de onde remover
//retorna: o elemento removido
int removeFromHeap(Heap* heap) {
	
	if (!heap->size) return -1;

	Pair* p = heap->vect[0];										//remove o elemento
	heap->vect[0] = heap->vect[heap->size - 1];
	heap->vect = realloc(heap->vect, --heap->size * sizeof(char*));	//redimenciona a heap

	int tempId = p->id;		
	free(p);

	maxHeapify(heap, 0);						//recalcula o estado de heap

	return tempId;
}

// verifica se a heap esta vazia
int isHeapEmpty(Heap* heap) {
	return !heap->size;
}


// destroi e desaloca uma heap.
void destroyHeap(Heap* heap) {
	for (int i = 0; i < heap->size; i++) {
		free(heap->vect[i]);
	}

	free(heap->vect);
	free(heap);
}

//imprime o conteudo da heap
void printHeap(Heap* heap) {
	for (int i = 0; i < heap->size; i++) {
		printf("%d %d, ", heap->vect[i]->id, heap->vect[i]->weight);
	}
	printf("\n");
}
