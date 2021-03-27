#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"


bool search(struct ListNode heap[], int size, int id){
	/*
		Args:
			heap: ListNode 堆结构
			size: int 堆大小
			id: int 查找的id
		Returns:
			bool 查找成功返回true
		Func:
			根据学号沿链表查找并答应结点信息
	*/
	int index = 0;
	while(index <= size && heap[index].next != -1){
		index = heap[index].next;
		if(id == heap[index].studentId){
			if(heap[index].flag){
				return false;
			}
			printf("\nFound the node with student id %d, name: %s", id, heap[index].name);
			return true;
		}
	}
	return false;
}

int main(){
	int shmid = shmget((key_t)KEY, sizeof(struct M), 0666 | IPC_CREAT);	// 
	if(shmid == -1){
		perror("\nlistNodesearch shmget failed: ");
		exit(EXIT_FAILURE);
	}

	void *shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1){
		perror("\nlistNodesearch shmat failed: ");
		exit(EXIT_FAILURE);
	}

	struct M *shared = (struct M *)shm;

	shared->lock = 3;
	printf("\nlistNode search ready ...");
	printf("\nEnter the -1 will exit search process");

	int id;
	while(1){
		while(shared->lock != 3) sleep(1);
		printf("\nEnter the studentId for the search listNode:");
		printf("\nEnter id");
		scanf("%d", &id);
		if(id == -1){
			break;
		}
		if(search(shared->heap, shared->size, id) == false){
			printf("\nNot found or the node has been deleted");
		}
		shared->lock = 3;
	}
	if(shmdt(shm) == -1){
		perror("\nlistNodesearch shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
