#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "heap.h"


// Função que le diversas frases e insere cada uma das palavras no grafo indicado, apontando quando eh inicio e fim.
// argumentos: Graph* g - grafo que contem informações das palavras.
void insertSentence(Graph* g){
	while(1){
		printf("Entre a quantidade de palavras que deseja inserir no grafo seguido da frase (insira -1 para sair):\n");
		int nWords = 0;
		scanf("%d", &nWords);
		if(nWords == -1) break;
		char from[50];
		char to[50];
		scanf(" %s", from);
		for(int i = 1; i < nWords; i++){							//recebe as palavras e insere-as no grafo
			scanf(" %s", to);
			insertTextGraph(g, from, to, i == 1, i == (nWords - 1));
			strcpy(from, to);
		}
	}
	return;
}


// Função que manipula corretamente os valores de from e to para chamar a variação correta de Graph::getTextPath(...) a fim de imprimir o caminho entre duas palavras entradas
// argumentos: 
// Graph* g - grafo que contem informações das palavras.
// char* from - string com a palavra origem.
// char* to - string com a palavra destino
// retorna um int como booleano dizndo se foi possivel ou não achar o caminho.
int printSubSentence(Graph* g, char* from, char* to){
	int* path;
	int size;
	if(!from) path = getFirstTextPath(g, getIdByWord(g, to), &size);
	else if(!to) path = getLastTextPath(g, getIdByWord(g, from), &size);
	else path = getTextPath(g, getIdByWord(g, from), getIdByWord(g, to), &size);
	if(!path) return 0;
	for(int i = size - 1; i >= 0; i--){
		if(to && path[i] == getIdByWord(g, to)) break;
		printf("%s ", getWordById(g, path[i]));
	}
	free(path);
	return 1;
}	


// Função que le palavras, as manipula corretamente chamando printSubSentence(...) para elas e imprimindo seu resultado
// argumentos: Graph* g - grafo que contem informações das palavras.
void completeSentence(Graph* g){
	printf("Entre a quantidade de palavras que deseja na frase seguido da frase a ser completada:\n");
	int nWord = 0;
	scanf("%d ", &nWord);
	char from[50];
	char to[50];
	scanf("%s", to);
	printf("\nFrase completa: ");
	if(!printSubSentence(g, NULL, to)){
		printf("\nArgumentos Invalidos, primeira palavra nao pertence ao dicionario ou eh impossivel liga-la ao inicio.\n");
		return;
	}
	strcpy(from, to);
	for(int i = 1; i < nWord; i++){
		scanf("%s", to);
		if(!printSubSentence(g, from, to)){
			printf("\nArgumentos Invalidos, palavra \"%s\" nao pertence ao dicionario ou eh impossivel liga-a a \"%s\".\n", to, from);
			return;
		}
		strcpy(from, to);
	}
	if(!printSubSentence(g, from, NULL)){
		printf("\nArgumentos Invalidos, ultima palavra nao pertence ao dicionario ou eh impossivel liga-la ao final.\n");
		return;
	}
	printf("\n\n");
}

// Função que le diversas palavras, e para cada uma delas, chama a função Graph::getTopThree(...) e imprime o resultado obtido por ela.
// argumentos: Graph* g - grafo que contem informações das palavras.
void suggestWord(Graph* g){
	char word[50];
	printf("Insira a palavra e as 3 melhores sugestões serão exibidas(Entre \"<\" para sair):\n");
	scanf("%s", word);
	while(strcmp(word, "<")){
		char** topThree = malloc(3 * sizeof(char*));
		int nSug = getTopThree(g, getIdByWord(g, word), topThree);
		if(!nSug) printf("Não tem nenhuma sugestão para essa palavra :(\n");
		else{
			printf("%d sugestão(ões) encontradas:\n", nSug);
			for(int i = 0; i < nSug; i++){
				printf("%s ", topThree[i]);
			}
		}
		printf("\n");
		free(topThree);
		printf("Insira a palavra e as 3 melhores sugestões serão exibidas(Entre \"<\" para sair):\n");
		scanf("%s", word);
	}
}


//Metodo main, simplesmente encontra a operação desejada e chama a função responsavel pela sua resolução.
int main(void) {
	
	
	Graph *g = createTextGraph();

	int op = -1;

	while(op != 0){
		printf("Qual funcionalidade deseja realizar\n1-Inserir Frase\n2-Imprimir o Grafo\n3-Completar Frase\n4-Sugerir Palavra\n0-Sair:\n");
		scanf("%d", &op);
		switch (op){
		case 1:
			insertSentence(g);
			break;
		case 2:
			printTextGraph(g);
			break;
		case 3:
			completeSentence(g);
			break;
		case 4:
			suggestWord(g);
			break;
		}
	}


	/*
	int antA[getNVert(g)];
	int antB[getNVert(g)];
	int dist[getNVert(g)];
	dijkstraText(g, 2, antA, antB, dist);

	for(int i = 0; i < getNVert(g); i++)
		printf("Vertice atual: %s\nDist: %d\nAntA: %s\nAntB: %s\n", getWordById(g, i), dist[i], antA[i] == -1 ? "none": getWordById(g, antA[i]), antB[i] == -1 ? "none": getWordById(g, antB[i]));
	
	*/
	destroyGraph(g);
}
