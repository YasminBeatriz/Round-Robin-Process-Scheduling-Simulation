#include "LinkedList.h"
#include <stdlib.h>
#include <stdio.h>

Lista* initLista()
{
	Lista *l = (Lista*) malloc (sizeof(Lista));
	l->prox = NULL;

	return l;
}

int isEmpty(Lista *l)
{
	if(l->prox == NULL)
		return TRUE;

	return FALSE;
}

long int size(Lista *l)
{
	long int count = 0;
	Lista *p = l;

	while(p->prox != NULL)
	{
		p = p->prox;
		count++;
	}

	return count;
}

void insertEnd(Lista *l, void *i, int t, element pr)
{
	Lista *novo;
	Lista *p = l;

	novo = (Lista*) malloc (sizeof(Lista));
	novo->info = i;
	novo->priority = pr;

	while(p->prox != NULL)
		p = p->prox;

	novo->prox = p->prox;
	p->prox = novo;
}

void insertBeginning(Lista *l, void *i, int t, element pr)
{
	Lista *novo;

	novo = (Lista*) malloc (sizeof(Lista));
	novo->info = i;
	novo->priority = pr;
	novo->prox = l->prox;
	l->prox = novo;
}

void insertByPriority(Lista *l, void *i, int t, element pr)
{
	Lista *novo;
	Lista *p = l;
	int count = 0;

	novo = (Lista*) malloc (sizeof(Lista));
	novo->info = i;
	novo->priority = pr;

	while(p->prox != NULL && novo->priority > p->prox->priority)
	{
		printf("%d ", count++);
		p = p->prox;
	}

	novo->prox = p->prox;
	p->prox = novo;
}

void *removeBeginning(Lista *l)
{
	void *removed;
	Lista *r;

	if(!isEmpty(l))
	{
		r = l->prox;
		removed = l->prox->info;
		l->prox = l->prox->prox;
		free(r);
		return removed;
	}
	else
		return NULL;
}

void *removeEnd(Lista *l)
{
	void *removed;
	Lista *r, *p = l;

	if(!isEmpty(l))
	{
		if(size(l) > 1)
		{
			while(p->prox->prox != NULL)
				p = p->prox;

		}

		r = p->prox;
		p->prox = NULL;
		removed = r->info;
		free(r);
		return removed;
	}
	else
		return NULL;
}

void releaseList(Lista *l)
{
	void *objectReturned;

	if(!isEmpty(l))
	{
		Lista *next, *current;

		current = l->prox;
		while(current != NULL)
		{
			next = current->prox;
			objectReturned = current->info;
			free(objectReturned);
			free(current);
			current = next;
		}
	}
}

void printList(Lista *l)
{
	/*Lista *p = l;

	while(p->prox != NULL)
	{
		p = p->prox;

		if(p->priority != -1)
		{
			printf("Process %d\n", p->info->id);
		}
	}	
	printf("\n");*/
	
}