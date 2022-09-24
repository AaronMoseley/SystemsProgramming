#include "pq.h"

pq_t* pq_create()
{
	pq_t* newPQ = (pq_t*)malloc(PQSIZE);
	newPQ->head = NULL;

	pthread_mutex_t mx;
	pthread_cond_t cd;

	newPQ->mutex = mx;
	newPQ->condition = cd;

	int error = 0;
	error = pthread_mutex_init(&(newPQ->mutex), NULL);
 
	if(error == -1)
		return NULL;
 
	error = pthread_cond_init(&(newPQ->condition), NULL);
 
	if(error == -1)
		return NULL;

	return newPQ;
}

void pq_insert(pq_t* q, void* item, short prio)
{
	node* newNode = (node*)malloc(NODESIZE);
	newNode->currItem = item;
	newNode->priority = prio;
	newNode->next = NULL;
	newNode->prev = NULL;

	//printf("created new node\n");

	if(q->head == NULL)
	{
		//printf("empty list\n");
		q->head = newNode;
		pthread_cond_signal(&q->condition);
		return;
	}

	node* head = q->head;
	if(prio > head->priority)
	{
		//printf("new head\n");
		head->prev = newNode;
		newNode->next = head;
		q->head = newNode;
		
		pthread_cond_signal(&(q->condition));
		return;
	}

	node* currNode = head;
	while(currNode->next != NULL)
	{
		if(prio > currNode->priority)
		{
			//printf("middle of list\n");
			node* prevNode = (node*)currNode->prev;
			
			newNode->next = (void*)currNode;
			newNode->prev = (void*)prevNode;

			prevNode->next = (void*)newNode;
			currNode->prev = (void*)newNode;

			pthread_cond_signal(&(q->condition));
			return;
		}

		currNode = (node*)currNode->next;
	}

	if(prio > currNode->priority)
	{
		//printf("second to last\n");
		node* prevNode = (node*)currNode->prev;

		newNode->next = (void*)currNode;
		newNode->prev = (void*)prevNode;

		prevNode->next = (void*)newNode;
		currNode->prev = (void*)newNode;

		pthread_cond_signal(&(q->condition));
		return;
	}

	//printf("new tail\n");

	newNode->prev = (void*)currNode;
	currNode->next = (void*)newNode;

	pthread_cond_signal(&(q->condition));
}

void* pq_next(pq_t* q)
{
	pthread_mutex_lock(&(q->mutex));

	if(q->head == NULL)
	{
		int error = pthread_cond_wait(&(q->condition), &(q->mutex));
		if(error != 0)
		{
			printf("Error: pthread_cond_wait, %d\n", error);
			return NULL;
		}
	}

	node* head = q->head;

	void* returnVal = head->currItem;
	
	if(head->next != NULL)
	{
		node* nextNode = (node*)head->next;
		
		nextNode->prev = NULL;
		q->head = nextNode;
		returnVal = head->currItem;
		free(head);
	}
	else {
		q->head = NULL;
		free(head);
	}

	pthread_mutex_unlock(&(q->mutex));

	return returnVal;
}






