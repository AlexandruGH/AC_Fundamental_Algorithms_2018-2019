#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"


#define INFINITY INT_MAX

Profiler profiler("Merge k sorted lists");

int nrOfElements;
int nrOfLists;

typedef struct node {
	int key;
	struct node *next;
	struct node*previous;
}NodeT;

typedef struct sentinel {
	NodeT *head;
	NodeT *tail;
	int nrOfNodes;
}Sentinel;

Sentinel *sentinels;

NodeT* create_node(int key);
int peek(Sentinel*);
void delete_first(Sentinel *sentinel);
void add_last(Sentinel *sentinel, int key);
void init_sentinels(Sentinel*sentinels, int number);
void create_lists(int, int);
void print_vector(int *, int);
void print_list(Sentinel *, int);



int main()
{
	int *vector;
	nrOfElements = 100;
	nrOfLists = 2;

	create_lists(nrOfElements, nrOfLists);

	for (int i = 0; i < nrOfLists; i++)
	{
		print_list(sentinels, i);
	}

	getchar();
	return 0;
}

void print_list(Sentinel *sentinel, int i)
{
	NodeT *aux = sentinel[i].head;
	int number = sentinel[i].nrOfNodes;
	printf("List %d:\n", i);
	while (number > 0)
	{
		printf("%d -> ", aux->key);
		number--;
		aux = aux->next;
	}
	printf("NULL\n");
}

void print_vector(int *vector, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", vector[i]);
	}
	printf("\n");
}

void create_lists(int n, int k)
{
	int *vector;
	int nrOfElementsInAList = n / k; //Atentie trebuie tratate toate cazurile de dimensiune
	int currentElement = 0;

	vector = (int *)malloc(sizeof(int)*n);
	if (NULL == vector)
	{
		perror("Error at allocating memory for the lists total elements.\n");
		exit(2);
	}

	
	sentinels = (Sentinel*)malloc(sizeof(Sentinel)*k);

	if (NULL == sentinels)
	{
		perror("Error at allocating memory for the sentinel.\n");
		exit(2);
	}

	init_sentinels(sentinels, k);
	FillRandomArray(vector, n, 10, 1000, false, 1);
	print_vector(vector, n);

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < nrOfElementsInAList; j++)
		{
			add_last((sentinels + i), vector[currentElement + j]);
		}
		currentElement += nrOfElementsInAList;
	}
}

NodeT *create_node(int key)
{
	NodeT *newNode = (NodeT *)malloc(sizeof(NodeT));
	if (NULL == newNode)
	{
		perror("Error at allocating memory for a new node.\n");
		exit(2);
	}
	newNode->next = NULL;
	newNode->previous = NULL;
	newNode->key = key;
	return newNode;
}

void add_last(Sentinel *sentinel, int key)
{
	if (NULL == sentinel->head)
	{
		sentinel->head = create_node(key);
		sentinel->tail = sentinel->head;
		sentinel->nrOfNodes = 1;
	}
	else {
		NodeT *newNode = create_node(key);
		newNode->previous = sentinel->tail;
		sentinel->tail->next = newNode;
		sentinel->head->previous = newNode;
		sentinel->tail = newNode;
		sentinel->tail->next = sentinel->head;
		sentinel->nrOfNodes++;
	}
}

int peek(Sentinel *sentinel)
{
	if (0 != sentinel->nrOfNodes)
	{
		return sentinel->head->key;
	}
	else {
		return INFINITY;
	}
}

void delete_first(Sentinel *sentinel)
{
	if (0 != sentinel->nrOfNodes)
	{
		NodeT *aux = sentinel->head;
		sentinel->tail->next = sentinel->head->next;
		sentinel->head = sentinel->head->next;
		sentinel->head->previous = sentinel->tail;
		free(aux);
		sentinel->nrOfNodes--;
	}
}

void init_sentinels(Sentinel*sentinels, int number)
{
	for (int i = 0; i < number; i++)
	{
		sentinels[i].head = NULL;
		sentinels[i].tail = NULL;
		sentinels[i].nrOfNodes = 0;
	}
}