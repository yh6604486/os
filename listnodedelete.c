#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"


void _delete(struct ListNode heap[], int* size, int id){
	/*
		Args:
			heap: ListNode[] 堆结构
			size: int& 堆大小 删除会改变堆大小，所以使用引用
			id: int 查找删除结点的key
		Returns:
			None
		Func:
			首先沿链表查找要删除的结点，找到后把该结点的flag置为0。把堆的最后一个结点移到该位置。
			size-1，此时堆不再满足最小堆性质，所以需要调用一次堆排
	*/
	int index = 0;
	while(index <= *size && heap[index].next != -1){
		index = heap[index].next;
		if(id == heap[index].studentId){
			heap[heap[index].prev].next = heap[index].next;		// 待删除结点的前一个结点的next置为删除结点的next
			if(heap[index].next != -1){
				heap[heap[index].next].prev = heap[index].prev;		// 待删除结点的下一个结点的prev置为删除结点的prev
			}
			heap[index].flag = 1;
			memcpy(&heap[index], &heap[*size], ListNode_SIZE);
			(*size)--;
			execv("./sort", NULL);
			break;
		}
	}
}

int main(){
	int shmid = shmget((key_t)KEY, sizeof(struct M), 0666 | IPC_CREAT);	// 
	if(shmid == -1){
		perror("\nlistNodedelete shmget failed: ");
		exit(EXIT_FAILURE);
	}

	void *shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1){
		perror("\nlistNodedelete shmat failed: ");
		exit(EXIT_FAILURE);
	}

	struct M *shared = (struct M *)shm;

	shared->lock = 2;
	printf("\nlistNode delete ready ...");
	printf("\nEnter the -1 will exit delete process");

	int id;
	while(1){
		while(shared->lock != 2) sleep(1);
		printf("\nEnter the studentId for the delete listNode:");
		printf("\nEnter id");
		scanf("%d", &id);
		if(id == -1){
			break;
		}
		_delete(shared->heap, &shared->size, id);
		shared->lock = 2;
	}
	if(shmdt(shm) == -1){
		perror("\nlistNodedelete shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
