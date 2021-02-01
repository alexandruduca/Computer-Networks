
s1:
	gcc server.c -o server
	./server
	
c1:
	gcc client.c -o client
	./client 127.0.0.1 3333