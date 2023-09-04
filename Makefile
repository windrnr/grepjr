all: linux windows 

linux: 
	gcc -Wall -Wextra -Wdouble-promotion -pedantic -o build/grepjr src/grepjr.c 
	
windows:
	x86_64-w64-mingw32-gcc -Wall -Wextra -Wdouble-promotion -pedantic -o build/grepjr src/grepjr.c
