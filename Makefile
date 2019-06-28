compiler = gcc
flags = -g -fms-extensions
headers_path = -I headers/ -I headers/interfaces -I headers/presenter/  -I headers/sec_index -I headers/storage
security_flags = -Wall -Wextra -Werror
run:
	$(compiler) src/main.c src/pizza.c src/presenter.c $(flags) $(headers_path) $(security_flags) -o bin/main
	./bin/main
clean:
	rm bin/main
