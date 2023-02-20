#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListaNod{
	char* data; 
	struct ListaNod *next; 
}ListaNod, *Lista;

typedef struct{
	int nr; 
	Lista *adl; 
	char **numeNod;
}TGraph;


TGraph* creareMatriceAdiacenta(int numberOfNodes) {
	TGraph* matrice_adiacenta = calloc(1,sizeof(TGraph));
	matrice_adiacenta->nr = numberOfNodes;
	matrice_adiacenta->adl = calloc(numberOfNodes, sizeof(ListaNod));

	matrice_adiacenta->numeNod = calloc(numberOfNodes , sizeof(char*));

	for(int i = 0; i < numberOfNodes; i++) {
		matrice_adiacenta->adl[i] = NULL;
		matrice_adiacenta->numeNod[i] = calloc(10 ,sizeof(char));
	}
		
	return matrice_adiacenta;
}

void creareLegatura(TGraph* graph, char *init, char *final) {
	ListaNod* nod = calloc(1,sizeof(ListaNod));
	nod->data = calloc(10 , sizeof(char));
	for(int i = 0; i< graph->nr; i++) {
		if(strcmp(graph->numeNod[i], init) == 0) {
			nod->next = NULL;
			memcpy(nod->data, final, strlen(final));
			ListaNod* parcurgere = graph->adl[i];
			if(graph->adl[i] == NULL) 
				graph->adl[i] = nod;
			else {
				while(parcurgere->next != NULL)
					parcurgere = parcurgere->next;
				parcurgere->next = nod;
			}
			
		}
	}
	
	
}

void next_recursiv(TGraph* graph, char* nod, int *visited, char **sorted, int *index) { 
	for(int i = 0; i < graph->nr; i++) {
		if(strcmp(nod,graph->numeNod[i]) == 0) {
			ListaNod* aux = graph->adl[i];
			while(aux != NULL) {
				for(int j = 0; j < graph->nr; j++) {
					if(strcmp(graph->numeNod[j], aux->data) == 0 && visited[j] == 0){
						visited[j] = 1;
						
						(*index)++;
						memcpy(sorted[*index], graph->numeNod[j], strlen(graph->numeNod[j]));
						next_recursiv(graph, graph->numeNod[j], visited, sorted, index);
					}
				}
				aux = aux->next;
			}
		}
	}
}

void next(TGraph* graph, char* nod, char **final,int *index) {
	int  index_final = 0;
	char **sorted = calloc(graph->nr, sizeof(char *));
	for(int i = 0; i < graph->nr; i++) {
		sorted[i] = calloc(10, sizeof(char));
	}
	int *visited = calloc(graph->nr , sizeof(int));
	next_recursiv(graph, nod, visited, sorted, index);
	(*index)++;

	for(int i = 0; i < graph->nr; i++) {
		for(int j = 0 ; j < (*index); j ++) {
			if(strcmp(sorted[j], graph->numeNod[i]) == 0) {
				memcpy(final[index_final], sorted[j], strlen(sorted[j]));
				index_final++;
			}
		}
	}
	for(int i = 0; i < graph->nr; i++) {
		free(sorted[i]);
	}
	free(sorted);
	free(visited);
}

void future(TGraph* graph, ListaNod* nod, char **out, int *index_out) {
	int index = -1;
	char **final = calloc(graph->nr, sizeof(char *));
	for(int i = 0; i < graph->nr; i++) 
		final[i] = calloc(10, sizeof(char));
	for(int i = 0; i < graph->nr; i++) {
		index = -1;
		next(graph, graph->numeNod[i], final, &index);
		for(int j = 0; j < index ; j++) {
			if(strcmp(final[j], nod->data) == 0) {
				memcpy(out[*index_out], graph->numeNod[i], strlen(graph->numeNod[i]));
				(*index_out)++;
			}			
		}
	}
	for(int i = 0; i < graph->nr; i++) 
		free(final[i]);
	free(final);
}

void anticone(TGraph* graph, ListaNod* nod, int *visited) {
	int index = -1;
	char **final = calloc(graph->nr, sizeof(char *));
	for(int i = 0; i < graph->nr; i++) 
		final[i] = calloc(10, sizeof(char));
	for(int i = 0; i < graph->nr; i++) {
		index = -1;
		next(graph, graph->numeNod[i], final, &index);
		for(int j = 0; j < index ; j++) {
			if(strcmp(final[j], nod->data) == 0) 
				visited[i] = 1;
					
		}
	}
	index = -1;
	next(graph, nod->data, final, &index);
	for(int i = 0; i < graph->nr ; i++) {
		for(int j = 0; j < index; j++) {
			if(strcmp(graph->numeNod[i], final[j]) == 0 || strcmp(graph->numeNod[i], nod->data) == 0)
				visited[i] = 1;
		}
	}
	for(int i = 0; i < graph->nr; i++) 
		free(final[i]);
	free(final);
}

void tips(TGraph* graph, int *visited) {
	ListaNod* nod = calloc(1, sizeof(ListaNod));
	nod->data = calloc(10 ,sizeof(char));
	nod->next = NULL;
	for(int i = 0; i < graph->nr; i++) {
		memcpy(nod->data, graph->numeNod[i], strlen(graph->numeNod[i]));
		int index = -1;
		char **final = calloc(graph->nr, sizeof(char *));
		for(int j = 0; j < graph->nr; j++) 
			final[j] = calloc(10, sizeof(char));
		next(graph, nod->data, final, &index);
	
		for(int j = 0; j < index ; j++){
			for(int k = 0; k < graph->nr; k ++) {
				if(strcmp(graph->numeNod[k], final[j]) == 0)
					visited[k] = 1;
			}
		}
		for(int j = 0; j < graph->nr; j++) 
			free(final[j]);
		free(final);
		free(nod->data);
		nod->data = calloc(10 ,sizeof(char));
	}
	free(nod->data);
	free(nod);
}

int aciclic(TGraph* graph) {
	int ok = 1, index = -1;
	char **final = calloc(graph->nr, sizeof(char *));
	for(int i = 0; i < graph->nr; i++) 
		final[i] = calloc(10, sizeof(char));
	for(int i = 0; i < graph->nr; i++) {
		index = -1;
		next(graph, graph->numeNod[i], final, &index);
		for(int j = 0; j < index ; j++) {
			if(strcmp(final[j], graph->numeNod[i]) == 0) 
				ok = 0;
		}
	}
	for(int i = 0; i < graph->nr; i++) 
		free(final[i]);
	free(final);
	return ok;
}

void afisare(TGraph* graph) {
	for(int i = 0 ; i < graph->nr; i++) {
		printf("%s:: ", graph->numeNod[i]);
		ListaNod* nod = graph->adl[i];
		while(nod != NULL) {
			printf("%s ", nod->data);
			nod = nod->next;
		}
		printf("\n");
	}
}



void memorie(TGraph* graph) {
	for(int i = 0 ; i < graph->nr; i++) {
		ListaNod* nod = graph->adl[i];
		ListaNod* prev = nod;
		while(nod != NULL) {
			nod = nod->next;
			free(prev->data);
			free(prev);
			prev = nod;
		}
		free(graph->numeNod[i]);
	}

	free(graph->adl);
	free(graph->numeNod);
	free(graph);
}

int main(int argc, char **argv) {
	FILE *f = fopen("blockdag.in", "rw");
    if (f == NULL) {
        printf("Error opening file f!\n");
        return 0;
    }
	FILE *g = fopen("blockdag.out", "w");
    if (g == NULL) {
        printf("Error opening file g!\n");
        return 0;
    }
	int n;
	char terminator;
	fscanf(f, "%d", &n);
	TGraph* matrice = creareMatriceAdiacenta(n);
	for(int i = 0; i < matrice->nr; i++){
		fscanf(f, "%s", matrice->numeNod[i]);
	}
	fscanf(f, "%c", &terminator);

	char *nod_init = calloc(10, sizeof(char));
	size_t maxim = 9999;
	char *line = malloc(sizeof(char) * maxim);
	for(int i = 0; i < matrice->nr; i++) {
		getline(&line, &maxim, f);
		line[strlen(line)-1] = '\0';
		char *p = strtok(line,": ");
		memcpy(nod_init,p, sizeof(p));
		
		while(p != NULL){
			 p = strtok(NULL, " :");
			 if(p != NULL && nod_init != NULL)
			 	creareLegatura(matrice, nod_init, p);
		}
		
		
	}
	
	///Cerinta 1 -----------------------------
	if(argc == 2 && strcmp(argv[1], "-c1") == 0 ) {
		if (aciclic(matrice) == 1) 
			fprintf(g, "correct\n");
		else
			fprintf(g, "impossible\n");
	}
	else if (argc == 3 && strcmp(argv[1], "-c2") == 0 ) {
	int ok = 0;
	for(int i = 0; i < matrice->nr; i++) {
		if(strcmp(argv[2],matrice->numeNod[i]) == 0)
			ok = 1;
	}
	if(ok == 1) {
	///Cerinta 2 ------------------------------
	ListaNod* nod = malloc(sizeof(ListaNod));
	nod->data = calloc(10, sizeof(char));
	memcpy(nod->data, argv[2], strlen(argv[2]));

	///past
	int index = -1;
	char **final = calloc(matrice->nr, sizeof(char *));
	for(int i = 0; i < matrice->nr; i++) 
		final[i] = calloc(10, sizeof(char));
	next(matrice, nod->data, final, &index);
	fprintf(g, "past(%s) : ", nod->data);
	for(int i = 0; i < index ; i++) {
		fprintf(g, "%s ", final[i]);
	}
	fprintf(g, "\n");
	
	///future
	index = 0;
	for(int i = 0; i < matrice->nr; i++) {
		free(final[i]);
		final[i] = calloc(10, sizeof(char));
	}
	future(matrice, nod, final, &index);
	fprintf(g, "future(%s) : ", nod->data);
	for(int i = 0; i < index; i++){
		fprintf(g, "%s ", final[i]);
		
	}
	for(int i = 0; i < matrice->nr; i++){
		
		free(final[i]);
	}
	free(final);
	fprintf(g, "\n");

	///anticone
	fprintf(g, "anticone(%s) : ", nod->data);
	int *visited = calloc(matrice->nr, sizeof(int));
	anticone(matrice, nod,visited);
	for(int i = 0 ; i < matrice->nr; i++) {
		if(visited[i] == 0)
			fprintf(g, "%s ", matrice->numeNod[i]);
		visited[i] = 0;
	}
	fprintf(g, "\n");
	///tips
	fprintf(g, "tips(G) : ");
	tips(matrice, visited);
	for(int i = 0 ; i < matrice->nr; i++) {
		if(visited[i] == 0)
			fprintf(g, "%s ", matrice->numeNod[i]);
	}
	fprintf(g, "\n");
	free(visited);
	free(nod->data);
	free(nod);
	}}
	///Cerinta 3 ---------------

	
	fclose(g);
	fclose(f);
	free(line);
	free(nod_init);
	memorie(matrice);
	return 0;
}
