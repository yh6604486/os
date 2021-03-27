proc: main insert delete search modify print_listnode print_heap

main: main.c LISTNODE.h
	gcc -w -o main main.c

insert: listnodeinsert.c LISTNODE.h
	gcc -w -o insert listnodeinsert.c

delete: listnodedelete.c LISTNODE.h
	gcc -w -o delete listnodedelete.c

search: listnodesearch.c LISTNODE.h
	gcc -w -o search listnodesearch.c

modify: listnodemodify.c LISTNODE.h
	gcc -w -o modify listnodemodify.c

print_listnode: listnodeprint.c LISTNODE.h
	gcc -w -o print_listnode listnodeprint.c

print_heap: heapprint.c LISTNODE.h
	gcc -w -o print_heap heapprint.c -lm

clean:
	rm main insert delete search modify print_listnode print_heap
