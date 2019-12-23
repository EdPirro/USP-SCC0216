
typedef struct _heap Heap;
typedef struct _pair Pair;

typedef struct _pair {
	int weight;
	int id;
	//char* data;
} Pair;

struct _heap {
	int size;
	Pair** vect;
};

Heap* createHeap();

Pair* createPair(int id, int weight);

void maxHeapifyOnInsert(Heap* heap);

void maxHeapify(Heap* heap, int parent);

void insertInHeap(Heap* heap, int id, int weight);

int removeFromHeap(Heap* heap);

int isHeapEmpty(Heap* heap) ;

void destroyHeap(Heap* heap) ;

void printHeap(Heap* heap);
