#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mymalloc.h"

// Don't change or remove these constants.
#define MINIMUM_ALLOCATION  16
#define SIZE_MULTIPLE       8

typedef struct Node
{
 struct Node* next;
 struct Node* prev;
 int used; //1 for used
 unsigned int size;
 //void* data;
} Node;

Node *head = NULL;
Node *tail = NULL;

Node* new_node(void* address, unsigned int size)
{
	Node* n = address;
	n->size = size;
	n->used = 0;
	n->next = NULL;
	n->prev = NULL;
	//n->data = address + sizeof(Node);
	return n;
}

void* data_address(Node* n)
{
	return (void*)((unsigned int)n + sizeof(Node));
}

Node* split_block(Node* nd, unsigned int needed)
{
	nd->size = nd->size - needed;
	void* newLoc = (void*)(unsigned int)nd + sizeof(Node) + nd->size;
	Node* new = new_node(newLoc, needed);
	if(tail == nd)
	{
		tail = new;
	}
	else
	{
		new->next = nd->next;
		nd->next->prev = new;
	}
	new->prev = nd;
	nd->next = new;
	return new;
}

Node* find_best_fit(unsigned int sz)
{
	Node* block;
	Node* bestFit = NULL;
	for(block = head; block != NULL; block = block->next)
	{
		if(block->size == sz && block->used == 0)
			return block;
		else if(block->size >= sz && block->used == 0)
		{
			if(bestFit == NULL) bestFit = block;
			else if(block->size < bestFit->size)
				bestFit = block;
		}
	}
	if(bestFit != NULL)
	{
		int neededSpace = sizeof(Node) + sz;
		int spaceLeft = bestFit->size - neededSpace;
		if(spaceLeft >= MINIMUM_ALLOCATION)
			bestFit = split_block(bestFit, neededSpace);
	}
	return bestFit;
}

//creates a node, puts it at the end of the list
//and returns its address
Node* insert_node(unsigned int sz)
{
	Node* n = new_node(sbrk(0), sz);
	if(tail == head)
	{
		n->prev = head;
		tail = n;
		head->next = n;
	}
	else
	{
		tail->next = n;
		n->prev = tail;
		tail = n;
	}
	n->used = 1;
	sbrk(sizeof(Node) + n->size);
	return n;
}

Node* coalesce(Node* freeNode)
{
	Node* neighborNode = freeNode->next;
	if(neighborNode != NULL && neighborNode->used == 0)
	{
		freeNode->next = neighborNode->next;
		if(neighborNode->next != NULL)
			neighborNode->next->prev = freeNode;
		freeNode->size += neighborNode->size + sizeof(Node);
		if(tail == neighborNode) tail = freeNode;
	}
	neighborNode = freeNode->prev;
	if(neighborNode != NULL && neighborNode->used == 0)
	{
		if(tail == freeNode) tail = neighborNode;
		neighborNode->next = freeNode->next;
		if(freeNode->next != NULL)
			freeNode->next->prev = neighborNode;
		neighborNode->size += freeNode->size + sizeof(Node);
		return neighborNode;
	}
	return freeNode;
}

unsigned int round_up_size(unsigned int data_size)
{
	if(data_size == 0)
		return 0;
	else if(data_size < MINIMUM_ALLOCATION)
		return MINIMUM_ALLOCATION;
	else
		return (data_size + (SIZE_MULTIPLE - 1)) & ~(SIZE_MULTIPLE - 1);
}

void* my_malloc(unsigned int size)
{
	if(size == 0)
		return NULL;

	size = round_up_size(size);

	// ------- Don't remove anything above this line. -------
	// Here's where you'll put your code!

	// AT LEAST SOME OF IT

	// USE FUNCTIONS

	// PLEASE

	if(head == NULL)
	{
		void* theBreak = sbrk(size + sizeof(Node));
		Node* first = new_node(theBreak, size);
		first->used = 1;
		head = first;
		tail = first;
		return data_address(head);
	}
	else
	{
		Node* bf = find_best_fit(size);
		if(bf != NULL)
		{
			bf->used = 1;
			return data_address(bf);
		}
		else
		{
			bf = insert_node(size);
			return data_address(bf);
		}
	}
	return NULL;
}

void my_free(void* ptr)
{
	if(ptr == NULL)
		return;

	ptr -= sizeof(Node);
	Node* block = (Node*)ptr;
	block->used = 0;
	block = coalesce(block);
	while(tail->used == 0)
	{
		void* brkpt = (void*)tail;
		if(head == tail)
		{
			head = NULL;
			tail = NULL;
			brk(brkpt);
			return;
		}
		else
		{
			tail = tail->prev;
			tail->next = NULL;
			brk(brkpt);
		}
	}
}
