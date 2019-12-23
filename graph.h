typedef int type;
typedef struct _graph Graph;
typedef struct _list List;
typedef struct _node Node;

Graph *createGraph(int vert);
//void insertEdge(Graph* g, int from, int to, int w);
void insertEdge(Graph* g, int from, int to, int w, int weight);
void removeEdge(Graph* g, int from, int to);
void destroyGraph(Graph* g);
//void printGraph(Graph* g);
//Graph* copyGraph(Graph* g);
//int *findEulerianCycle(Graph *g);
//int maxColors(Graph* g);
//int getAdjId(Graph* g, int baseId, int adjPos);
//int* bfs(Graph* g);

//void dijkstra(Graph *grafo, int idInicio);
void dijkstraText(Graph *grafo, int idInicio, int* antA, int* antB, int* dist);
int getNVert(Graph* g);
Graph* createTextGraph();
void insertTextGraph(Graph* g, char* wordFrom, char* wordTo, char begin, char end);
void printTextGraph(Graph* g);
char* getWordById(Graph* g, int id);
int getIdByWord(Graph* g, char* word);
int* getFirstTextPath(Graph* g, int to, int* size);
int* getLastTextPath(Graph* g, int from, int* size);
int* getTextPath(Graph* g, int from, int to, int* size);
int getTopThree(Graph* g, int base, char** topWords);
