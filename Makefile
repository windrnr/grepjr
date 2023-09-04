all: linux windows 

linux: 
	gcc -Wall -pedantic -o build/grepjr src/grepjr.c 
	
windows:
	x86_64-w64-mingw32-gcc -Wall -pedantic -o build/grepjr src/grepjr.c
