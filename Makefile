build: 
	gcc *.c -o blockdag -g 

test: build
	valgrind --leak-check=full ./blockdag

run: build
	./blockdag

clean:
	rm -rf blockdag 
