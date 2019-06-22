compiler = gcc
flags = -Wall -g -fms-extensions
headers_path = -I headers/

run:
	$(compiler) src/* $(flags) $(headers_path) -o bin/main
