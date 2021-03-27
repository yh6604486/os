#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"


void print_listnode(const struct ListNode heap[],const int size){
	/* 
		Args:
			heap: const ListNode 堆结构
			size: const int 堆的大小
		Returns:
			None
		Func:
			按照链表顺序打印各结点
	*/
	int index = 0, count = 0;
	printf("\nindex	student id %15s", "name");
	while(index <= size && heap[index].next != -1){
		index = heap[index].next;
		if(!heap[index].flag){
			printf("\n %4d   %8d %15s", count, heap[index].studentId, heap[index].name);
		}
		count++;
	}
}

int main(){
	int shmid = shmget((key_t)KEY, sizeof(struct M), 0666 | IPC_CREAT);	// 
	if(shmid == -1){
		perror("\nlistNodeprintf shmget failed: ");
		exit(EXIT_FAILURE);
	}

	void *shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1){
		perror("\nlistNodeprintf shmat failed: ");
		exit(EXIT_FAILURE);
	}

	struct M *shared = (struct M *)shm;

	shared->lock = 6;
	printf("\nlistNode print ready ...");


	while(shared->lock != 6) sleep(1);
	print_listnode(shared->heap, shared->size);

	if(shmdt(shm) == -1){
		perror("\nlistNodeprintf shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
