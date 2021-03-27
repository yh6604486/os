#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "LISTNODE.h"

int main(){
	pid_t pid;
	int count = 0;
	char command[15];
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

	struct M* shared = (struct M*)shm;
	init2(shared);

	while(1){
		printf("command: insert, delete, search, modify\nprint_listnode, print_heap\n");
		printf("Enter the command\n");
		scanf("%s", command);
		pid = vfork();
		if(pid < 0){
			printf("pid error\n");
			exit(EXIT_FAILURE);
		}
		else if(pid == 0){
			if(strcmp(command, "insert") == 0){
				execv("./insert", NULL);
			}
			else if(strcmp(command, "delete") == 0){
				execv("./delete", NULL);
			}
			else if(strcmp(command, "search") == 0){
				execv("./search", NULL);
			}
			else if(strcmp(command, "modify") == 0){
				execv("./modify", NULL);
			}
			else if(strcmp(command, "print_heap") == 0){
				execv("./print_heap", NULL);
			}
			else if(strcmp(command, "print_listnode") == 0){
				execv("./print_listnode", NULL);
			}
			else if(strcmp(command, "end") == 0){
				exit(EXIT_SUCCESS);
			}
			else{
				printf("command error\n");
			}
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
			if(strcmp(command, "end") == 0){
				exit(EXIT_FAILURE);
			}
			sleep(2);
			system("clear");
		}
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1) {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
