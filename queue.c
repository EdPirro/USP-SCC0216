#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

/*void createQueue(Queue* q) {
	q->begin = NULL;
	q->end = NULL;
	return;
}

int isQueueEmpty(Queue q) {
	return (q.begin == NULL);
}

int pushToQueue(Queue* q, elem x) {
	NodeQ* n = (NodeQ *) malloc(sizeof(NodeQ));
	if (n == NULL) 
		return 0;

	n->info = x;
	n->next = NULL;

	if (isQueueEmpty(*q))
		q->begin = n;
	else 
		q->end->next = n;
	
	q->end = n;

	return 1;
}

int popFromQueue(Queue* q, elem* x) {
	if (isQueueEmpty(*q))
		return 0;

	*x = q->begin->info;

	NodeQ* n = q->begin;

	q->begin = q->begin->next;

	if (q->begin == NULL)
		q->end == NULL;

	free(n);
	return 1;
}

int prinfFila(Queue q) {
	while (q.begin != NULL) {
		printf("%d ", q.begin->info);
		q.begin = q.begin->next;
	}
}*/
