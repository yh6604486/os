#ifndef LISTNODE_H
#define LISTNODE_H

#include<string.h>

#define ListNode_SIZE 32
#define MAX_SIZE 1024
#define KEY 6666
#define true 1
#define false 0
#define bool int

struct ListNode{
	int flag;
	int studentId;
	char name[16];
	int prev;
	int next;
};

struct M{
	int lock;
	struct ListNode heap[MAX_SIZE];
	int size;
	int tail;
};

void init1(struct ListNode* n){
	n->flag = 0;
	n->studentId = 0;
	memset(n->name, 0, sizeof(n->name));
	n->prev = -1;
	n->next = -1;
}

void init2(struct M* shared_memory){
	shared_memory->lock = 0;
	for(int i = 0;i < MAX_SIZE;i++){
		init1(&(shared_memory->heap[i]));
	}
	shared_memory->size = 0;
	shared_memory->tail = 0;
}

#endif
