
/****************************************************************************/
/* DSA node program example  D.F. ROSS                                      */
/****************************************************************************/

/****************************************************************************/
/* include files and  global data objects                                   */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************/
/* define types & constants                                                 */
/****************************************************************************/
#define MAXNOD 20

/****************************************************************************/
/* node element definition (this is hidden!)                                */
/****************************************************************************/
typedef struct nodeelem * noderef;

typedef struct nodeelem {
        char       nname;
        int        ninfo;
        noderef    edges;
        noderef    nodes;
        } nodeelem;

/****************************************************************************/
/* define graph instance                                                    */
/****************************************************************************/
static noderef G     = (noderef) NULL;          /* adjacency list           */
static int     adjmat[MAXNOD][MAXNOD];          /* adjacency matrix         */

/****************************************************************************/
/* private operations on the node - basic operationa                        */
/****************************************************************************/
static int      is_empty(noderef N)             { return N == NULL; }

static char     get_nname(noderef N)            { return N->nname; }
static int      get_ninfo(noderef N)            { return N->ninfo; }
static noderef  get_edges(noderef N)            { return N->edges; }
static noderef  get_nodes(noderef N)            { return N->nodes; }

static noderef  set_nname(noderef N, char c)    { N->nname = c; return N; }
static noderef  set_ninfo(noderef N, int v)     { N->ninfo = v; return N; }
static noderef  set_edges(noderef N, noderef E) { N->edges = E; return N; }
static noderef  set_nodes(noderef N, noderef M) { N->nodes = M; return N; }

/****************************************************************************/
/* create and initialise an element in the node                             */
/****************************************************************************/
static noderef create_n(char c, int w)
{
	return set_ninfo( set_nname((noderef)malloc(sizeof(nodeelem)),c),w);
}

/****************************************************************************/
/* private operations on the graph - basic operations                       */
/****************************************************************************/
/****************************************************************************/
/* head and tail - a RECURSIVE view of the sequence                         */
/****************************************************************************/
static noderef nhead(noderef N)     { return N; }
static noderef ntail(noderef N)     { return get_nodes(N); }

static noderef ehead(noderef E)     { return E; }
static noderef etail(noderef E)     { return get_edges(E); }

/****************************************************************************/
/* CONStruct a new node with the element at the head of the node            */
/****************************************************************************/
static noderef ncons(noderef e, noderef N) { return set_nodes(e, N); }
static noderef econs(noderef e, noderef E) { return set_edges(e, E); }

/****************************************************************************/
/* display the edges                                                        */
/****************************************************************************/
static void b_edisp(noderef E) {
	if (!is_empty(E)){ 
		printf(" %c(%2d)", get_nname(ehead(E)), get_ninfo(ehead(E)));
		b_edisp(etail(E));
	}
}

/****************************************************************************/
/* display the nodes                                                        */
/****************************************************************************/
static void b_ndisp(noderef G) {
	if (is_empty(G)) 
		printf("\n");
	else{ 
		printf(" %c : ", get_nname(nhead(G)));
		b_edisp(get_edges(G)); printf("\n");
		b_ndisp(ntail(G));
	}
}

/****************************************************************************/
/* ADD to the node in ascending order                                       */
/****************************************************************************/
static noderef b_addn(char c, noderef G)
{
	if(!is_empty(G))
		return set_nodes(G,b_addn(c,get_nodes(G)));
	else
		return create_n(c,0);
}

/****************************************************************************/
/* ADD to the edge in ascending order                                       */
/****************************************************************************/
static noderef b_adde(char c, int w, noderef E)
{
	return set_edges(create_n(c,w),E);
}

/****************************************************************************/
/* REMove a  node from the graph                                            */
/****************************************************************************/
static noderef b_remn(char c, noderef G) {

	if(!is_empty(G)){
		if(get_nname(G)==c){
			return get_nodes(G);
		}else{
			set_nodes(G,b_remn(c,get_nodes(G)));
		}
	}
	return G;			
}

/****************************************************************************/
/* REMove an edge from the graph                                            */
/****************************************************************************/
static noderef b_reme(char c, noderef E) {
	if(!is_empty(E)){
		if(get_nname(E)==c)
			return get_edges(E);
		else
			set_edges(E,b_reme(c,get_edges(E)));
	}
	return E;
}

/****************************************************************************/
/* REMove all edges for a given node from the graph                         */
/****************************************************************************/
static void b_remalle(char c, noderef G) {
	if(!is_empty(G)){
		set_edges(G,b_reme(c,get_edges(G)));
		b_remalle(c,get_nodes(G));
	}
}

/****************************************************************************/
/* FIND a  node in the graph                                                */
/****************************************************************************/
static noderef b_findn(char c, noderef G) {
	if(!is_empty(G)){
		if(get_nname(G)==c)
			return G;
		else	
			return b_findn(c,get_nodes(G));
	}else 
		return NULL;

}

/****************************************************************************/
/* FIND an edge in the graph                                                */
/****************************************************************************/
static noderef b_finde(char c, noderef E) {
	if(!is_empty(E)){
		if(get_nname(E)==c)
			return E;
		else	
			return b_finde(c,get_edges(E));
	}else 
		return NULL;

}

/****************************************************************************/
/* FIND the number of nodes in the graph (cardinality nodes)                */
/****************************************************************************/
static int b_size(noderef G) {    
	if(!is_empty(G))
		return 1+b_size(get_nodes(G));
	else 
		return 0;
}

/****************************************************************************/
/* CREATE the adjacency matrix (AM)                                         */
/****************************************************************************/
/****************************************************************************/
/* Find the position (index) of a node in the adjacency list (node list)    */
/* NB: this is used to determine the index for the edge                     */
/*     this index is in turn used to fill in the weight in the AM           */
/* e.g. for an adjacency list:                                              */
/*               (position 0)   a => b(3) -> c(2) eol                       */
/*               (position 1)   b => a(3) -> c(7) eol                       */
/*               (position 2)   c => a(2) -> b(7) eol                       */
/*                              eol                                         */
/* get_pos("b") will give 1 (and hence AM[0][1] is set to 3 i.e. a-3-b)     */
/****************************************************************************/
static int get_pos(char fname)  {
	int i=0;
	noderef T=G;
	while(get_nname(T)!=fname){
		i++;
		T = get_nodes(T);
	}
	return i;

   //printf("\n TO BE DONE "); return 0;
}

/****************************************************************************/
/* Fill in the values in the adjancy matrix from the adjacency list         */
/* this will give an adjacency matrix:     a       b       c                */
/*                                    -------------------------             */
/*                                    a |  0       3       2                */
/*                                    b |  3       0       7                */
/*                                    c |  2       7       0                */
/****************************************************************************/
static void cre_adjmat(noderef G) {
	if(!is_empty(G)){
		noderef T = G;
		while(!is_empty(T=get_edges(T))){
			adjmat[get_pos(get_nname(G))][get_pos(get_nname(T))] = get_ninfo(T);
		}	
		cre_adjmat(get_nodes(G));
	}
}

/****************************************************************************/
/* DISPLAY the adjacency matrix                                             */
/****************************************************************************/
static void b_mdisp(noderef G) {

   int i,j, nnodes;
   noderef pnode;

   cre_adjmat(G); nnodes = b_size(G);
   
   printf("\n ----"); for (i=0; i<nnodes; i++) printf("----");
   printf("\n     ");
   pnode = G;
   for (i=0; i<nnodes; i++) {
      printf("  %c ", get_nname(pnode));
      pnode = get_nodes(pnode);
      }
   printf("\n ----"); for (i=0; i<nnodes; i++) printf("----");
   pnode = G;
   for (i=0; i<nnodes; i++) {
      printf("\n "); printf(" %c: ", get_nname(pnode));
      for (j=0; j<nnodes; j++) printf(" %2d ", adjmat[i][j]);
      pnode = get_nodes(pnode);
      }
   printf("\n ----"); for (i=0; i<nnodes; i++) printf("----");
   printf("\n ");
}

/****************************************************************************/
/* GRAPH ALGORITHMS                                                         */
/****************************************************************************/
void b_Dijkstra() {
	
	if(is_empty(G))
		return;
	noderef Curr = G;
	noderef T = Curr;
	
	int weightto = 0,i,condition, least = INT_MAX,temp=0;

	int * visited = (int *)malloc(sizeof(int)*b_size(G));

	int * D = (int *)malloc(sizeof(char)*b_size(G));
	char * E = (char *)malloc(sizeof(char)*b_size(G));
	int * L = (int *)malloc(sizeof(char)*b_size(G));

	for(i=0; i<b_size(G);i++){
		D[i]=INT_MAX;
		E[i]=get_nname(G);
		L[i]=INT_MAX;
	}

	do{
		least = INT_MAX;
		T=Curr;
		printf("Hejhej\n\n");
		while(!is_empty(T=get_edges(T))){
			if((D[get_pos(get_nname(T))]==INT_MAX || (get_ninfo(T)+weightto)<D[get_pos(get_nname(T))]) && visited[get_pos(get_nname(T))]!=1){
				D[get_pos(get_nname(T))] = get_ninfo(T)+weightto;
				L[get_pos(get_nname(T))] = get_ninfo(T);
				E[get_pos(get_nname(T))] = get_nname(Curr);
			}
		}

	printf("Första loopen klar\n\n");

	visited[get_pos(get_nname(Curr))]=1;
	T=G;
	condition = 0;
	for(i=0;i<b_size(G);i++){
		if( D[i]!='-' && (D[i]+weightto)<least && visited[i]!=1){
			least = D[i]+weightto;
			condition = 1;
			Curr = T;
			temp = i;
		}
		T = get_nodes(T);
	}

	visited[temp] = 1;
	weightto = least;
	
	}while(condition);
	

	for(i=0;i<b_size(G);i++){
		if(D[i]==INT_MAX)
			printf("   -   ");
		else
			printf("  %d  ",D[i]);
	}

	printf("\n");

	for(i=0;i<b_size(G);i++)
		printf("  %c  ",E[i]);

	printf("\n");

	for(i=0;i<b_size(G);i++){
		if(L[i]==INT_MAX)
			printf("   -   ");
		else
			printf("  %d  ",L[i]);
	}
}
void b_Floyd()    { /* TO DO */ }
void b_Warshall() { /* TO DO */ }
void b_Prim()     { 
	
	noderef Vn = create_n(get_nname(G),0);
	set_edges(Vn,get_edges(G));
	noderef En = (noderef)NULL, T, S, U, N;
	int least;
	
	while(b_size(G) > b_size(Vn)){
		least = INT_MAX;
		S = Vn;

		while(!is_empty(S)){
			T = get_edges(S);
			while(!is_empty(T)){
				if(get_ninfo(T)<least && is_empty(b_findn(get_nname(T),Vn))){
					U=T;
					N=S;
					least = get_ninfo(T);
				}
				T = get_edges(T);
			}
			S = get_nodes(S);
		}
		printf("\nbara namn på noderna U = %c och N = %c\n",get_nname(U),get_nname(N));
	
		Vn = b_addn(get_nname(U),Vn);
		set_edges(b_findn(get_nname(U),Vn),get_edges(b_findn(get_nname(U),G)));

		En = b_addn(get_nname(U),En);
		set_edges(b_findn(get_nname(U),En),create_n(get_nname(N),least));

		printf("\n");
		T=En;
		while(!is_empty(T)){
			printf(" %c ",get_nname(T));
			T = get_nodes(T);
		}

		printf("\n");

		T = En;
		printf("\n");
		while(!is_empty(T)){
			printf("   %c och edgen är %d", get_nname(T), get_ninfo(get_edges(T)));
			T = get_nodes(T);
		}
		printf("\n");

	}

	printf("\n\nResultat:\n");
	while(!is_empty(Vn)){
		printf(" %c ",get_nname(Vn));
		Vn = get_nodes(Vn);
	}
	printf("\n\n");


	T = En;
	while(!is_empty(T)){
		printf(" %c - %d - %c  \n",get_nname(T),get_ninfo(get_edges(T)),get_nname(get_edges(T)));
		T = get_nodes(T);
	}

}

void b_dispSPT()  { /* TO DO */ }
void b_dispFm()   { /* TO DO */ }
void b_dispTC()   { /* TO DO */ }
void b_dispMST()  { /* TO DO */ }

/****************************************************************************/
/* public operations on the node                                            */
/****************************************************************************/
void be_display_adjlist()      { b_ndisp(G); }
void be_display_adjmatrix()    { b_mdisp(G); }

void be_addnode(char c)  { G = b_addn(c, G); }
void be_remnode(char c)  { G = b_remn(c, G); b_remalle(c, G); }

void be_addedge(char cs, char cd, int v) {
   set_edges(b_findn(cs, G), b_adde(cd, v, get_edges(b_findn(cs, G))));
   }

void be_remedge(char cs, char cd) {
   set_edges(b_findn(cs, G), b_reme(cd, get_edges(b_findn(cs, G))));
   }

int be_is_nmember(char c) { return !is_empty(b_findn(c, G)); }

int be_is_emember(char cs, char cd) {
   return be_is_nmember(cs) &&
          !is_empty(b_finde(cd, get_edges(b_findn(cs, G))));
   }

int be_size() { return b_size(G); }

/****************************************************************************/
/* GRAPH ALGORITHMS                                                         */
/****************************************************************************/
void be_Dijkstra()  { b_Dijkstra(); b_dispSPT(); }
void be_Floyd()     { b_Floyd();    b_dispFm();  }
void be_Warshall()  { b_Warshall(); b_dispTC();  }
void be_Prim()      { b_Prim();     b_dispMST(); }
/****************************************************************************/
/* end of basic functions                                                   */
/****************************************************************************/



