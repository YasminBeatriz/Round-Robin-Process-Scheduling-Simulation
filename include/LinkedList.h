#define TRUE	1
#define FALSE	0

#define PROCESS	2
#define EVENT	1

#ifndef LIST_H
#define LIST_H

	typedef double element;

	typedef struct no
	{
		element priority;
		int type;
		void *info;
		struct no* prox;
	} Lista;

#endif

Lista* initLista();

int isEmpty(Lista *l);

long int size(Lista *l);

void insertEnd(Lista *l, void *i, int t, element pr);

void insertBeginning(Lista *l, void *i, int t, element pr);

void insertByPriority(Lista *l, void *i, int t, element pr);

void *removeBeginning(Lista *l);

void *removeEnd(Lista *l);

void printList(Lista *l);

void releaseList(Lista *l);