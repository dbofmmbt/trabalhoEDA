compiler = gcc
flags = -g -fms-extensions
headers_path = -I headers/ -I headers/interfaces -I headers/presenter/  -I headers/sec_index -I headers/storage
security_flags = -Wall -Wextra -Werror
source = src/main.c src/pizza.c src/presenter.c src/interface_init.c src/storage/storage.c src/storage/metadata.c src/storage/leaf_node.c src/storage/internal_node.c src/storage/rotations.c
run: build
	./bin/main
clean:
	rm bin/main
	rm data/*
build:
	$(compiler) $(source) $(flags) $(headers_path) $(security_flags) -o bin/main
