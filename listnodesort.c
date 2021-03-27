#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"

void heap_sort(struct ListNode heap[], int i, int size){
	/*
		Args:
			heap: ListNode 堆结构
			i: int 排序结点
			size: int 堆大小
		Returns:
			None
		Func:
			堆排序，使每个结点保持小顶堆性质
	*/
	int min_index = i;
	int left = 2*i, right = 2*i + 1;
	while(i < size){
		if(left <= size && heap[left].studentId < heap[min_index].studentId){
			min_index = left;
		}
		if(right <= size && heap[right].studentId < heap[min_index].studentId){
			min_index = right;
		}
		if(min_index == i){
			break;
		}

		ListNode temp;
		memcpy(&temp, &heap[i], ListNode_SIZE);
		memcpy(&heap[i], &heap[min_index], ListNode_SIZE);
		memcpy(&heap[min_index], &temp, ListNode_SIZE);
		heap[heap[i].prev].next = i;
		if(heap[i].next != -1){
			heap[heap[i].next].prev = i;
		}
		heap[heap[min_index].prev].next = min_index;
		if(heap[min_index].next != -1){
			heap[heap[min_index].next].prev = min_index;
		}
		i = min_index;
	}
}

void sort(ListNode heap[], int size){
	/*
		Args:
			heap: ListNode 堆结构
			size: int 堆大小
		Returns:
			None
		Func:
			堆排序
	*/
	for(int i = size/2;i > 0;i--){
		heap_sort(heap, i, size);
	}
}

int main(){
	int shmid = shmget((key_t)KEY, sizeof(struct M), 0666 | IPC_CREAT);
	if(shmid == -1){
		perror("\nlistNodesort shmget failed: ");
		exit(EXIT_FAILURE);
	}

	void *shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1){
		perror("\nlistNodesort shmat failed: ");
		exit(EXIT_FAILURE);
	}

	struct M *shared = (struct M *)shm;

	shared->lock = 5;
	printf("\nlistNode sort ready ...");


	while(shared->lock != 5) sleep(1);
	sort(shared->heap, shared->size);
	
	if(shmdt(shm) == -1){
		perror("\nlistNodesort shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
