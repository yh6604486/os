#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"


void print_heap(const struct ListNode heap[],const int size){
	/*
		Args:
			heap: const ListNode 堆结构，打印函数不改变堆结构使用const
			size: const int 堆大小
		Returns:
			None
		Func:
			打印堆，按照二元小顶堆的结构分层打印。即第一层一个结点，第二层两个结点
			...
	*/
	int index = 0, count = 0;
	printf("\n#### Heap ####");
	for(int i = 1;i <= size;i++){
		if(pow(2, count) == i){
			count++;
			printf("\n");
		}
		printf("%d: %s\t", heap[i].studentId, heap[i].name);
	}
	printf("\n#############");
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

	shared->lock = 7;
	printf("\nlistNode print ready ...");


	while(shared->lock != 7) sleep(1);
	print_heap(shared->heap, shared->size);

	if(shmdt(shm) == -1){
		perror("\nlistNodeprintf shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
