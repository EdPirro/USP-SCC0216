#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "queue.h"
#include "heap.h"

#define INF 0xfffffff;

//typedef struct _list List;
//typedef struct _node Node;

//struct grafo
//contem um vetor de listas de adjacencia
struct _graph {
	int vert;
	List **edges;
};

//struct lista
//uma lista de adjacencai encadeada
//contem informações auxiliares como: tamanho da lista, palavra do vertice, cor do vertice, id do vertice
struct _list {
	Node *begin, *end;
	int size;	
	char word[50];
	int color;
	int id;
};

//struct nó
//um nó da lista encadeada, representa uma aresta entre dois vertices
//contem o id do vertice de destino, o peso da aresta, o tipo da areste e um ponteiro para o próximo nó da lista
struct _node {
	int id;
	int weight;
	type w;
	struct _node *next;
};


//cria um nó da lista da adjacencia
//recebe o id de destino, o tipo w e o peso
Node *createNode(int id, int w, int weight) {
	Node *n = malloc(sizeof(Node));
	n->id = id;
	n->w = w;
	n->weight = weight;
	n->next = NULL;
	return n;
}


//cria uma lista de adjacencia para um vertice com com id
//recebe: o id do vertice detentor da lista
List *createList(int id) {
	List* l;
	l = malloc(sizeof(List));
	l->begin = NULL;
	l->end = NULL;
	l->size = 0;
	l->id = id;
	l->color = -1;
	return l;
}


//cria um grafo
//recebe: o numero maximo de vertices
Graph *createGraph(int vert) {
	Graph* g = malloc(sizeof(Graph));
	if (!g) return g;

	g->vert = vert;
	g->edges = malloc(sizeof(List *) * vert);
	if (!g->edges) return g;

	for (int i = 0; i < vert; i++) {
		g->edges[i] = createList(i);
	}

	return g;
}


//retorna o numero maximo de vertices de um grafo
int getNVert(Graph* g){
	return g->vert;
}

//insere um nó na lista de adjacencia de um vertice
//recebe: a lista de adjacencia, o id do destino, o tipo da aresta, o peso da aresta
void insertNode(List* l, int id, int w, int weight) {
	if (!l->end) {									//se a lista esta vazia, inicializa ela
		l->size++;									
		l->begin = createNode(id, w, weight);
		l->end = l->begin;
		return;
	}

	Node* last = l->end;					//aumenta a lista, cria e insere um novo nó
	l->size++;
	l->end = createNode(id, w, weight);
	last->next = l->end;
	return;
}


//insere uma aresta entre dois vertices
//recebe: o grafo, o id da origem, id do destino, o tipo de aresta, o peso da aresta
void insertEdge(Graph* g, int from, int to, int w, int weight) {
	if (from >= g->vert) return;
	if (to >= g->vert) return;
	if (from < 0) return;
	if (to < 0) return;

	insertNode(g->edges[from], to, w, weight);
}

//remove um nó da lista de adjacencia de um vertice
//recebe: a lista de adjacencia, o id do nó a ser retirado
void removeNode(List* l, int id) {
	if (!l->begin) return;
	

	Node* last = NULL;
	Node* curr = l->begin;
	while (curr) {
		if (curr->id == id) {						//percorre a lista e retira o nó, se encontrar
			if (!last) l->begin = curr->next;
			else if (!(curr->next)) {
				l->end = last;
				l->end->next = NULL;
			}
			else last->next = curr->next;
			free(curr);
			l->size--;
			return;
		}
		last = curr;
		curr = curr->next;
	}
}

//remove uma aresta entre dois vertices
//recebe: o grafo, o vertice de origem e o de destino
void removeEdge(Graph* g, int from, int to) {
	if (from >= g->vert) return;
	if (to >= g->vert) return;
	if (from < 0) return;
	if (to < 0) return;

	removeNode(g->edges[from], to);
	return;
}

//destroi uma lista de adjacencia
void destroyList(List* l){
	Node* curr = l->begin;
	Node* aux;
	while(curr){					//da free nos nós da lista
		aux = curr->next;
		free(curr);
		curr = aux;
	}
	free(l);
	return;
}

//destroi um grafo
void destroyGraph(Graph* g){
	for(int i = 0; i < g->vert; i++){
		destroyList(g->edges[i]);			//da free nas listas de adjacencia
	}
	free(g->edges);
	free(g);
	return;
}


/*
Definição:
	dijkstra, but with text!
	dijkstra alterado para funcionar no grafo de texto proposto

	O algoritmo, ao contrario da abordagem tradicional, procura maximizar o custo do caminho encotrado
	Para isso, o algoritmo busca passar pelas arestas de maior peso, o que significaria palavras com maior relação.
	Isso entretanto causaria problemas de ciclos, para tal exite o vetor de antecessores secundarios, a ideia eh que 
	o algoritmo guarda ate 2 caminhos diferentes para o mesmo vertice, desse modo impedindo que ele fique preso em um ciclo infinito
	e guardando o caminho correto ate o no em questão.
Parâmetros:
	Graph *grafo: Ponteiro para o grafo sobre o qual o algoritmo executara.
	int idInicio: int identificando qual o vertice inicial do caminho buscado.
	int* antA: vetor (com tamanho = numero de vertices) onde serão guardados os antecessores primarios de cada vertice.
	int* antB: vetor (com tamanho = numero de vertices) onde serão guardados os antecessores secundarios de cada vertice. 
	int* dist: vetor (com tamanho = numero de vertices) onde serão guardados as distancias para cada vertice.
 */
void dijkstraText(Graph *grafo, int idInicio, int* antA, int* antB, int* dist){
	
	Heap* heap = createHeap();

	for(int i = 0; i < grafo->vert; i++){ //inicializa os vetores com valores inexistentes
		dist[i] = -1;
		antA[i] = -1;
		antB[i] = -1;
	}
	insertInHeap(heap, idInicio, 0);
	
	dist[idInicio] = 0;
	
	Node *aux;

	// "dijkstra", enquanto tem valores na heap encontra o de maior distancia (maximização do peso)
	while(heap->size > 0){
		int u = heap->vect[0]->id;
		
		removeFromHeap(heap);
		
		aux = grafo->edges[u]->begin;					//pega o primeiro visinho daquele nó
		
		for(int i = 0; i < grafo->edges[u]->size; i++){
			
			int v = dist[aux->id];						//custo até o visinho i
			int w = aux->weight;						//peso entre u e i
			

			// se o peso para o visinho for maior e o algoritmo puder ser chamado para o vertice em questão seguramente, ou seja,
			//se o vertice que pretende acessar não foi acessado anteriormente do mesmo lugar (loop) 
			if(dist[u] + w > dist[aux->id] && (antA[aux->id] != u || (aux->id != idInicio && antB[aux->id] == -1) || antB[aux->id] == u)){			
				antB[aux->id] = antA[aux->id];					//"tensionamento" (!relaxamento)
				antA[aux->id] = u;
				dist[aux->id] = dist[u] + w;			//custo até i = custo até u + peso
				insertInHeap(heap, aux->id, dist[aux->id]);	//insere na lista de prioridade
			}
			
			aux = aux->next;
		}
	}

	destroyHeap(heap);			
}

#include <string.h>


//cria um grafo de texto
Graph* createTextGraph(){
	Graph* g = createGraph(2);
	strcpy(g->edges[0]->word, "__BEG__");
	strcpy(g->edges[1]->word, "__END__");
	return g;
}



/*
Definição:
	Recebe uma palavra e insere-a no grafo.
Parâmetros:
	Graph *grafo: Ponteiro para o grafo no qual sera inserido a palavra.
	char* wordFrom: string do vetice origem da ligação.
	char* wordTo: string do vertice destino da ligação
	char begin: indicador de começo, sera 1 quando a palavra em questão for a primeira da frase.
	char end: indicador de final, sera 1 quando a palavra em questão for a ultima da frase.
 */
void insertTextGraph(Graph* g, char* wordFrom, char* wordTo, char begin, char end){
	char containsFrom = 0;
	char containsTo = 0;
	for(int i = 2; i < g->vert; i++){
		if(!strcmp(g->edges[i]->word, wordFrom)) containsFrom = i;			//checa se a palavra de origem ou destino ja existem
		if(!strcmp(g->edges[i]->word, wordTo)) containsTo = i;				//no grafo, e pega seu id
	}

	if(!containsTo){													//se não contem a palavra de destino
		g->edges = realloc(g->edges, ++g->vert * sizeof(List*));		//insere um novo nó no grafo, com a palavra
		containsTo = g->vert - 1;
		g->edges[containsTo] = createList(containsTo);				
		strcpy(g->edges[containsTo]->word, wordTo);
	}

	if(!containsFrom){												//se não comtem a palavra de origem
		g->edges = realloc(g->edges, ++g->vert * sizeof(List*));	//insere um novo nó no grafo com a palavra
		containsFrom = g->vert - 1;
		g->edges[containsFrom] = createList(containsFrom);
		strcpy(g->edges[containsFrom]->word, wordFrom);
	}

	Node* aux = g->edges[containsFrom]->begin;
	int i;
	for(i = 0; i < g->edges[containsFrom]->size; i++){				//checa os adjacentes da palavra de origem
		if(aux->id == containsTo){									//se existe uma aresta com a palavra de destino
			aux->weight++;											//aumenta seu peso
			break;
		}
		aux = aux->next;
	}																							//se não
	if(i == g->edges[containsFrom]->size) insertEdge(g, containsFrom, containsTo, 1, 1);		//cria uma aresta entre as palavras
	

	if(begin){												//se for a primeira palavra da farse
		Node* aux = g->edges[0]->begin;						//verifica se a palavra ja esta conectada no inicio.
		for(i = 0; i < g->edges[0]->size; i++){
			if(aux->id == containsFrom){					//caso sim, aumenta o peso da ligação
				aux->weight++;
				break;
			}
			aux = aux->next;
		}																	//caso nao, insere uma aresta entre begin e a palvra em questão
		if(i == g->edges[0]->size) insertEdge(g, 0, containsFrom, 1, 1);
	}

	if(end){												//se for a ultima palavra da farse
		Node* aux = g->edges[containsTo]->begin;			//verifica se a palavra ja esta conectada no inicio.
		int i;												
		for(i = 0; i < g->edges[containsTo]->size; i++){	//caso sim, aumenta o peso da ligação
			if(aux->id == 1) break;
			aux = aux->next;
		}
		if(i == g->edges[containsTo]->size){				//caso nao, insere uma aresta entre begin e a palvra em questão
			insertEdge(g, containsTo, 1, 1, 1);
		}
	}

	return;
}


//retorna a palavra de um nó, dado o id do nó
char* getWordById(Graph* g, int id){
	return g->edges[id]->word;
}


//retorna o id de um nó, dada a palavra do nó
int getIdByWord(Graph* g, char* word){
	for(int i = 0; i < g->vert; i++){
		if(!strcmp(getWordById(g, i), word)) return i;
	}
	return -1;
}


//imprime um grafo de texto
void printTextGraph(Graph* g){
	for(int i = 0; i < g->vert; i++){								//para cada vertice
		if (!g->edges[i]->size) {									//imprime vazio, se não houver adjacencias
			printf("Vertice Atual: <%s>, nao se conecta a nenhum vertice\n", getWordById(g, i));
			continue;
		}
		printf("Vertice Atual: <%s>, se conecta a:\n", getWordById(g, i));
		Node* aux = g->edges[i]->begin;
		for(int j = 0; j < g->edges[i]->size; j++){					//imprime os adjacentes daquele nó
			printf("\t<%s> %d vez(es)\n", getWordById(g, aux->id), aux->weight);
			aux = aux->next;
		}
	}
}


/*
Definição:
	Função que, a partir dos resultados do dijkstraText(...) cria um vetor de inteiros formado pelos ids dos vertices que estão no caminho encontrado.
	Sua função eh retornar um vetor que mostre qual o caminho proposto por dijkstraText(...) para ser tomado entre o vertice begin e o to (parametro).
	para tal ela vai ate o vertice de destino e contoi uma "pilha" usando os vetores de antecessores, essa pilha eh o caminho a ser tomado, que,
	no caso significa a frase sugerida entre duas palavras.
	Essa função deve ser usada quando a palavra inserida for a primeira da frase, pois, caso o inicio esteja ligada diretamente a essa palavra,
	não havera caminho entre o inicio e ela, pois ela ja eh um possivel inicio. Caso contrario esse caminho eh encontrado.
Parâmetros:
	Graph *grafo: Ponteiro para o grafo que contem as informações do dicionario.
	int to: inteiro representando o id do vertice que deseja-se acançar.
	int* size: ponteiro para um inteiro que guardara o tamanho final do vetor retornado (para ser processado corretamente onde foi chamado).
Retorno:
	int* - um vetor com ideia de "pilha" que guarda o id dos vertices que estão no caminho (gerado pela função dijkstraText(...)) entre begin e to.
 */
int* getFirstTextPath(Graph* g, int to, int* size){
	if(to == -1) return NULL;
	Node* aux = g->edges[0]->begin;
	for(int i = 0; i < g->edges[0]->size; i++){		// itera sobre os adjacentes de begin. 
		if(aux->id == to){							// Caso a palavra em questão seja um possivel inicio, não havera caminho entre begin e ela
			int* ret = malloc(1 * sizeof(int));		// O algoritmo entende que ela sera a primeira palavra presente na frase pois eh capaz de se-la
			*size = 0;
			return ret;
		}
	}

	// Caso a palavra não seja um possivel inicio, um caminho entre inicio e ela sera gerado.
	int antA[g->vert];
	int antB[g->vert];
	int dist[g->vert];
	dijkstraText(g, 0, antA, antB, dist); // chama dijkstraText(...) para criar as distancias e os caminhos a partir vertice begin (para todos os outros)

	// inicia o vetor de retorno.
	int* ret = malloc(sizeof(int));
	int tempSize = 0;
	int next = antA[to];
	while(1){
		if(next == -1){ // se next = -1 quer dizer que eh impossivel chegar em to a partir de begin.
			free(ret);
			*size = 0;
			return NULL;
		}
		if(next == 0){ // quando next for igual ao id do inicio, no caso 0, a fução retorna (chegou no lugar desejado como inicio);
			*size = tempSize;
			return ret;
		}
		int i;
		for(i = 0; i < tempSize; i++){ // verifica se o vertice a ser inserido ja esta no vetor de caminho.
			if(ret[i] == next){

				// caso ja esteja, então ele eh inserido e next vira o antecessor secundario dele,
				// desse modo, o algoritmo so passa por um ciclo completo uma unica vez, tomando um caminho diferente na segunda.
				ret = realloc(ret, (tempSize + 1) * sizeof(int));
				ret[tempSize++] = next;
				next = antB[next];
				break;
			}
		}
		if(i == tempSize){

			// caso não esteja no caminho ainda, ele eh inserido e next vira seu antecessor primario.
			ret = realloc(ret, (tempSize + 1) *  sizeof(int));
			ret[tempSize++] = next;
			next = antA[next];
		}	
	}
}


/*
Definição:
	Função que, a partir dos resultados do dijkstraText(...) cria um vetor de inteiros formado pelos ids dos vertices que estão no caminho encontrado.
	Sua função eh retornar um vetor que mostre qual o caminho proposto por dijkstraText(...) para ser tomado entre o vertice from (parametro) e o end.
	para tal ela vai ate o vertice de destino e contoi uma "pilha" usando os vetores de antecessores, essa pilha eh o caminho a ser tomado, que,
	no caso significa a frase sugerida entre duas palavras.
	Essa função deve ser usada quando a palavra inserida for a ultima da frase, pois, caso essa palavra esteja ligada diretamente ao final,
	não havera caminho entre o ela e final, pois ela ja eh um possivel fim. Caso contrario esse caminho eh encontrado.
Parâmetros:
	Graph *grafo: Ponteiro para o grafo que contem as informações do dicionario.
	int from: inteiro representando o id do vertice que deseja-se acançar.
	int* size: ponteiro para um inteiro que guardara o tamanho final do vetor retornado (para ser processado corretamente onde foi chamado).
Retorno:
	int* - um vetor com ideia de "pilha" que guarda o id dos vertices que estão no caminho (gerado pela função dijkstraText(...)) entre from e end.
 */
int* getLastTextPath(Graph* g, int from, int* size){
	if(from == -1) return NULL;
	Node* aux = g->edges[from]->begin;
	for(int i = 0; i < g->edges[from]->size; i++){ // itera sobre os adjacentes de from
		if(aux->id == 1){							// Caso a palavra em questão seja um possivel fim, não havera caminho entre ela e end
			int* ret = malloc(1 * sizeof(int));		// O algoritmo entende que ela sera a ultima palavra presente na frase pois eh capaz de se-la
			ret[0] = from;
			*size = 1;
			return ret;
		}
	}

	// Caso a palavra não seja um possivel fim, um caminho entre ela e end sera gerado.
	int antA[g->vert];
	int antB[g->vert];
	int dist[g->vert];
	dijkstraText(g, from, antA, antB, dist); // chama dijkstraText(...) para criar as distancias e os caminhos a partir vertice from (para todos os outros)

	// inicia o vetor de retorno.
	int* ret = malloc(sizeof(int));
	int tempSize = 0;
	int next = antA[1];
	while(1){
		if(next == -1){ // se next = -1 quer dizer que eh impossivel chegar em end a partir de from.
			free(ret);
			*size = 0;
			return NULL;
		}
		if(next == from){ // quando next for igual ao id do vertice inicial (from), a fução retorna (chegou no lugar desejado como fim);
			ret = realloc(ret, (tempSize + 1) * sizeof(int));
			ret[tempSize++] = from;
			*size = tempSize;
			return ret;
		}
		int i;
		for(i = 0; i < tempSize; i++){ // verifica se o vertice a ser inserido ja esta no vetor de caminho.
			if(ret[i] == next){

				// caso ja esteja, então ele eh inserido e next vira o antecessor secundario dele,
				// desse modo, o algoritmo so passa por um ciclo completo uma unica vez, tomando um caminho diferente na segunda.
				ret = realloc(ret, (tempSize + 1) * sizeof(int));
				ret[tempSize++] = next;
				next = antB[next];
				break;
			}
		}
		if(i == tempSize){

			// caso não esteja no caminho ainda, ele eh inserido e next vira seu antecessor primario.
			ret = realloc(ret, (tempSize + 1) *  sizeof(int));
			ret[tempSize++] = next;
			next = antA[next];
		}	
	}
}

/*
Definição:
	Função que, a partir dos resultados do dijkstraText(...) cria um vetor de inteiros formado pelos ids dos vertices que estão no caminho encontrado.
	Sua função eh retornar um vetor que mostre qual o caminho proposto por dijkstraText(...) para ser tomado entre o vertice from (parametro) e o to (parametro).
	para tal ela vai ate o vertice de destino e contoi uma "pilha" usando os vetores de antecessores, essa pilha eh o caminho a ser tomado, que,
	no caso significa a frase sugerida entre duas palavras.
	Essa função deve ser usada quando a palavra inserida for uma do meio da frase, neste caso o algoritmo sempre bucara o "melhor" caminho 
	(proposto pela função dijkstraText(...)) entre from e to.
Parâmetros:
	Graph *grafo: Ponteiro para o grafo que contem as informações do dicionario.
	int from: inteiro representando o id do vertice que deseja-se acançar.
	int to: inteiro representando o id do vertice inicial.
	int* size: ponteiro para um inteiro que guardara o tamanho final do vetor retornado (para ser processado corretamente onde foi chamado).
Retorno:
	int* - um vetor com ideia de "pilha" que guarda o id dos vertices que estão no caminho (gerado pela função dijkstraText(...)) entre from e to.
 */
int* getTextPath(Graph* g, int from, int to, int* size){
	if(from == -1 || to == -1) return NULL;

	// Caso a palavra não seja um possivel fim, um caminho entre ela e end sera gerado.
	int antA[g->vert];
	int antB[g->vert];
	int dist[g->vert];
	dijkstraText(g, from, antA, antB, dist); // chama dijkstraText(...) para criar as distancias e os caminhos a partir vertice from (para todos os outros)

	// inicia o vetor de retorno.
	int* ret = malloc(sizeof(int));
	int tempSize = 1;
	ret[0] = to;
	int next = antA[to];
	while(1){	
		if(next == from){ // quando next for igual ao id do vertice inicial (from), a fução retorna (chegou no lugar desejado como fim);
			ret = realloc(ret, (tempSize + 1) * sizeof(int));
			ret[tempSize++] = from;
			*size = tempSize;
			return ret;
		}
		if(next == -1){ // se next = -1 quer dizer que eh impossivel chegar em to a partir de from.
			free(ret);
			*size = 0;
			return NULL;
		}
		int i;
		for(i = 0; i < tempSize; i++){ 
			if(ret[i] == next){ 		// verifica se o vertice a ser inserido ja esta no vetor de caminho.

				// caso ja esteja, então ele eh inserido e next vira o antecessor secundario dele,
				// desse modo, o algoritmo so passa por um ciclo completo uma unica vez, tomando um caminho diferente na segunda.
				ret = realloc(ret, (tempSize + 1) * sizeof(int));
				ret[tempSize++] = next;
				next = antB[next];
				break;
			}
		}
		if(i == tempSize){

			// caso não esteja no caminho ainda, ele eh inserido e next vira seu antecessor primario.
			ret = realloc(ret, (tempSize + 1) *  sizeof(int));
			ret[tempSize++] = next;
			next = antA[next];
		}	
	}
}

//dado um vertice, encontra as 3 palavras mais provaveis para o completamento automatico
//recebe: o grafo, o id do vertice com a l=palavra de origem, um vetor de palavras onde serão carregadas as 3 sujestões
int getTopThree(Graph* g, int base, char** topWords){
	int topWeight[3] = {-1};
	int ret = 0;
	if(base < 2) return 0;
	Node* aux = g->edges[base]->begin;
	for(int i = 0; i < g->edges[base]->size; i++){				//analisa todas as arestas do vertice,
		if(aux->id == 1){										//salvando as 3 arestas de maior peso
			aux = aux->next;									//em ordem decrescente
			continue;
		}
		if(aux->weight > topWeight[0]){
			topWeight[2] = topWeight[1];
			topWords[2] = topWords[1];
			topWeight[1] = topWeight[0];
			topWords[1] = topWords[0];
			topWeight[0] = aux->weight;
			topWords[0] = getWordById(g, aux->id);
			ret++;
		}
		else if(aux->weight > topWeight[1]){
			topWeight[2] = topWeight[1];
			topWords[2] = topWords[1];
			topWeight[1] = aux->weight;
			topWords[1] = getWordById(g, aux->id);
			ret++;
		}
		else if(aux->weight > topWeight[2]){
			topWeight[2] = aux->weight;
			topWords[2] = getWordById(g, aux->id);
			ret++;
		}
		aux = aux->next;
	}
	return ret >= 3 ? 3 : ret;
}









//---------------------FIM DA LINHA
//---------------------------------
//---------------------------------
//---------------------------------
//---------------------------------
//---------------------------------
//----------NADA PRA VER LA EMBAIXO
//---------------------------------
//---------------------------------
//---------------------------------
//---------------------------------
//---------------------------------
//------------------------EU AVISEI

/*void printGraph(Graph* g) {
	printf("   to peso |to peso |to peso |to peso |to peso\n");
	for (int i = 0; i < g->vert; i++) {
		Node *curr = g->edges[i]->begin;
		printf("%d: ", g->edges[i]->id);
		while(curr) {
			printf("(%d: \"%d\")|", curr->id, curr->weight);
			curr = curr->next;
		} 
		printf("\n");
	}
}*/

//retorna o grau de um vertice
/*int degree(Graph *g, int id) {
	return g->edges[id]->size;
} 
int degreeList(List* vert) {
	return vert->size;
}*/

/*int isEulerianCiclyc(Graph* g) {
	for (int i = 0; i < g->vert; i++) {
		if (degree(g, i) % 2 || !degree(g, i))
			return 0;
	}
	return 1;
}*/

/*void dfs(Graph *g, int* visited, int id) {
	visited[id] = 1;
	Node *curr = g->edges[id]->begin;
	while (curr) {
		if (!visited[curr->id]) 
			dfs(g, visited, curr->id);
		curr = curr->next;
	}
}*/

/*int isBridge(Graph *g, int from, int to) {
	int* visited = calloc(g->vert, sizeof(int));
	
	//removeEdge(temp, curr, to);
	//removeEdge(temp, to, curr);

	dfs(g, visited, to);

	for (int i = 0; i < g->vert; i++) {
		if (!visited[i]) {
			free(visited);
			return 1; 
		}
	}

	//insertEdge(temp, curr, to);
	//insertEdge(temp, to, curr);

	free(visited);
	return 0;
}*/

/*Graph* copyGraph(Graph* g) {
	Graph* temp = createGraph((g->vert));
	for (int i = 0; i < temp->vert; i++) {
		Node* curr = g->edges[i]->begin;
		while(curr) {
			insertEdge(temp, i, curr->id, curr->w, curr->weight);
			curr = curr->next;
		}
	}

	return temp;
}*/

/*int *findEulerianCycle(Graph *g) {
	if (!isEulerianCiclyc(g))
		return NULL;

	Graph *temp = copyGraph(g);
	int totalEdges = 0;
	for (int i = 0; i < g->vert; i++) {
		totalEdges += degree(temp, i);
	}

	int* path = malloc(sizeof(int) * ((totalEdges / 2) + 2));
	int curr = 0;
	int count = 0;
	path[count++] = curr; 
	while(temp->edges[0]->size) {
		Node *nodeTo = temp->edges[curr]->begin;
		while(nodeTo) {
			int to = nodeTo->id;
			if (!isBridge(temp, curr, to) || (isBridge(temp, curr, to) && temp->edges[curr]->size == 1)) {
				removeEdge(temp, curr, to);
				removeEdge(temp, to, curr);
				curr = to;
				path[count++] = curr;
				break;
			}

			nodeTo = nodeTo->next;
		}
	}

	destroyGraph(temp);

	path[(totalEdges / 2) + 1] = -1;

	return path;
}*/


/*int getAdjId(Graph* g, int baseId, int adjPos){  
	if(!g) return -1;
	Node* curr = g->edges[baseId]->begin;
	if(!curr) return -1;
	if(adjPos >= g->edges[baseId]->size) return -1; 
	for(int i = 0; i < adjPos; i++)	curr = curr->next;
		return curr->id;
}

int getAdjWeight(Graph* g, int baseId, int adjPos){  
	if(!g) return -1;
	Node* curr = g->edges[baseId]->begin;
	if(!curr) return -1;
	if(adjPos >= g->edges[baseId]->size) return -1; 
	for(int i = 0; i < adjPos; i++)	curr = curr->next;
		return curr->w;
}*/

/*int colorfy(Graph* g, List** edgeList){
	int color = 0;
	int noColorCount = -1;
	while(noColorCount){
		noColorCount = 0;
		for(int i = 0; i < g->vert; i++){
			List* curr = g->edges[edgeList[i]->id];
			if(curr->color == -1){
				noColorCount++;
				int canPaint = 1;
				for(int j = 0; j < curr->size; j++){
					if(g->edges[getAdjId(g, curr->id, j)]->color == color){
						canPaint = 0;
						break;
					}
				}
				if(canPaint) g->edges[curr->id]->color = color;
			}

		}
		if(noColorCount) color++;
	}
	return color;
}*/

/*int compareDegree(const void* edgeA, const void* edgeB){
	return degreeList(*((List**)edgeB)) - degreeList(*((List**)edgeA));
}*/

/*int maxColors(Graph* g){
	int i = 0;
	Graph* a = copyGraph(g);
	List** edgeList = a->edges;
	qsort(edgeList, g->vert, sizeof(List*), compareDegree);
	int ret = colorfy(g, edgeList);
	destroyGraph(a);
	return ret;
}*/

/*int *bfsTest(Graph* g, int id) {
	int *dist = malloc(sizeof(int) * g->vert);
	memset(dist, -1, g->vert);

	Queue q;
	createQueue(&q);

	pushToQueue(&q, g->edges[id]->id);

	dist[id] = 0;

	while(!isQueueEmpty(q)) {
		popFromQueue(&q, &id);
		for (int i = 0; i < g->edges[id]->size; i++) {
			int curr = getAdjId(g, id, i);
			if (!(dist[curr] + 1)) {
				pushToQueue(&q, curr);
				dist[curr] = dist[id] + 1;
			}
		}
	}

	return dist;
}*/


/*int* bfs(Graph* g) {
	int size = g->vert;

	int *dist = malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++) {
		dist[i] = -1;
	}

	Queue q;
	createQueue(&q);

	int v;
	for (int k = 0; k < g->vert; k++) {
		if (dist[k] != -1) continue;
		dist[k] = 0;
		pushToQueue(&q, k);
		
		while(!isQueueEmpty(q)) {
			popFromQueue(&q, &v);
			for (int i = 0; i < g->edges[v]->size; i++) {
				int curr = getAdjId(g, v, i);
				if (dist[curr] == -1) {
					dist[curr] = dist[v] + 1;
					pushToQueue(&q, curr);
				}
			}
		}
	}

	return dist;
}*/



/*void dijkstra(Graph *grafo, int idInicio) {
	
	Heap *heap = createHeap();
	
	int dist[grafo->vert];
	int ant[grafo->vert];
	
	for(int i = 0; i < grafo->vert; i++){
		dist[i] = INF;
		ant[i] = -1;
	}

	insertInHeap(heap, idInicio, 0);
	
	dist[idInicio] = 0;
	
	Node *aux;

	while(heap->size > 0){
		
		int u = heap->vect[0]->id;
		
		removeFromHeap(heap);
		
		aux = grafo->edges[u]->begin;					//pega o primeiro visinho daquele nó
		
		for(int i = 0; i < grafo->edges[u]->size; i++){
			
			int v = dist[aux->id];						//custo até o visinho i
			int w = aux->weight;						//peso entre u e i
			
			if(dist[u] + w < dist[aux->id]){			//se (custo até u + peso) < (custo até i)
				ant[aux->id] = u;
				dist[aux->id] = dist[u] + w;			//custo até i = custo até u + peso
				insertInHeap(heap, aux->id, -dist[aux->id]);	//insere na heap
			}
			
			aux = aux->next;
		}
	}
	
	for(int i = 0; i < grafo->vert; i++){
		printf("dist from %d to %d = %d\n", idInicio, i, dist[i]);
		printf("path: ");
		int cur = i;
		while(1){
			printf("%d ", cur);
			if(cur == idInicio || ant[cur] == -1)
				break;
			printf("<- ");
			cur = ant[cur];
		}
		printf("\n\n");
	}
			
}*/




// Graph* prim(Graph* g) {
// 	Graph* tree = createGraph(g->vert);
// 	char* isInMst = calloc(g->vert, sizeof(char));

// 	Heap* h = createHeap(); 

// 	insertInHeap(h, 0, 0, -1); // insere na heap 'h', peso '0', from '0', to '-1'
// 	// indica que eh raiz

// 	while (!isHeapEmpty(h)) {
// 		int u, v, w; // from, to & weight
// 		removeFromHeap(h, &w, &v, &u); //
// 		if (isInMst[v]) continue;
		
// 		isInMst[v] = 1;

// 		if (u != -1) insertEdge(tree, u, v, w);

// 		for (int i = 0; i < g->edges[v]->size; i++) {
// 			if (!isInMst[i]) {
// 				insertInHeap(h, getAdjWeight(g, v, i), getAdjId(g, v, i), v)
// 			}
// 		}

// 	}

// 	destroyHeap(h);
// 	free(isInMst);

// 	return tree;	
// }
