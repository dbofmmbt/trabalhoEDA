compiler = gcc
flags = -g -fms-extensions
headers_path = -I headers/

run:
	$(compiler) src/main.c src/pizza.c $(flags) $(headers_path) -o bin/main
