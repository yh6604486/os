#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "LISTNODE.h"


bool modify(struct ListNode heap[], int size, int id, char newname[]){
	/*
		Args:
			heap: ListNode 堆结构
			size: int 堆大小
			id: int 需要修改的结点的查找key
			newname: char[] 修改的信息
		Returns:
			bool 查找并修改成功返回true
	*/
	int index = 0;
	while(index <= size && heap[index].next != -1){
		index = heap[index].next;
		if(id == heap[index].studentId){
			if(heap[index].flag){
				return false;
			}
			else{
				strcpy(heap[index].name, newname);
				return true;
			}
		}
	}
	return false;
}

int main(){
	int shmid = shmget((key_t)KEY, sizeof(struct M), 0666 | IPC_CREAT);	// 
	if(shmid == -1){
		perror("\nlistNodemodify shmget failed: ");
		exit(EXIT_FAILURE);
	}

	void *shm = shmat(shmid, 0, 0);
	if(shm == (void*)-1){
		perror("\nlistNodemodify shmat failed: ");
		exit(EXIT_FAILURE);
	}

	struct M *shared = (struct M *)shm;

	shared->lock = 4;
	printf("\nlistNode modify ready ...");
	printf("\nEnter the -1 and end will exit modify process");

	int id;
	char name[15];
	while(1){
		while(shared->lock != 4) sleep(1);
		printf("\nEnter the studentId of the modified listNode:");
		printf("\nEnter id and the new name");
		scanf("%d%s", &id, name);
		if(id == -1 && strncmp(name, "end", 3) == 0){
			break;
		}
		if(modify(shared->heap, shared->size, id, name) == false){
			printf("\nModify Error!");
		}
		shared->lock = 4;
	}
	if(shmdt(shm) == -1){
		perror("\nlistNodemodify shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	sleep(2);
	exit(EXIT_SUCCESS);
}
