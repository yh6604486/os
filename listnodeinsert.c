#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"


void insert(struct ListNode heap[], int* size, int* tail,struct ListNode node){
	/*
		Args:
			heap: ListNode 堆结构
			size: int& 堆大小，插入堆结点会改变堆大小，使用引用
			tail: int& 链表结构的尾，每次插入的结点都会连在链表的尾上
			node: ListNode 待插入的结点
		Returns:
			None
		Func:
			对于一个新结点，首先根据堆的插入规则确定新结点的插入位置。同时在交换已经入堆
			的结点时要修改交换结点的前置结点的next和后置结点的prev。确定了新结点的位置
			后，将heap[tail]的next置为该位置，同时该结点的前置结点是tail。tail置为
			新的尾结点
	*/
	(*size)++;
	int index = (*size);
	while(index/2 > 0 && heap[index/2].studentId > node.studentId){
		// 确定新结点的插入位置
		memcpy(&heap[index], &heap[index/2], ListNode_SIZE);
		if(heap[index].prev != -1){
			heap[heap[index].prev].next = index;
		}
		if(heap[index].next != -1){
			heap[heap[index].next].prev = index;
		}
		index /= 2;
	}
	memcpy(&heap[index], &node, ListNode_SIZE);
	heap[index].prev = *tail;
	heap[*tail].next = index;
	*tail = index;
}

int main(){
	int shmid = shmget((key_t)KEY, sizeof(struct M), 0666 | IPC_CREAT);	// 
	if(shmid == -1){
		perror("\nListNodeinsert shmget failed: ");
		exit(EXIT_FAILURE);
	}

	void *shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1){
		perror("\nListNodeinsert shmat failed: ");
		exit(EXIT_FAILURE);
	}

	struct M *shared = (struct M *)shm;

	shared->lock = 1;
	printf("\nListNode insert ready ...");
	printf("\nEnter the -1 and end will exit insert process");

	int id;
	char name[16];
	struct ListNode node;
	while(1){
		init1(&node);
		while(shared->lock != 1) sleep(1);
		printf("\nEnter the information for the new ListNode:");
		printf("\nEnter id and name:");
		scanf("%d%s", &id, name);
		if(id == -1 && strncmp(name, "end", 3) == 0){
			break;
		}
		node.studentId = id;
		strcpy(node.name, name);
		insert(shared->heap, &shared->size, &shared->tail, node);
		shared->lock = 1;
	}
	if(shmdt(shm) == -1){
		perror("\nListNodeinsert shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
