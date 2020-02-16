huffman: bin/main.o bin/huffman.o
	g++ bin/main.o bin/huffman.o -O2 -std=c++11 -pedantic -Wall -Wextra -Werror -o huffman

bin/main.o: src/main.cpp src/huffman.h | bin/
	g++ src/main.cpp -c -std=c++11 -O2 -pedantic -Wall -Wextra -Werror -o bin/main.o

test: bin/test.o bin/autotest.o bin/huffman.o
	g++ bin/test.o bin/autotest.o bin/huffman.o -I src/ -O2 -std=c++11 -pedantic -Wall -Wextra -Werror -o huffman_test	

bin/test.o: test/test.cpp test/test.h src/huffman.h | bin/
	g++ test/test.cpp -c -std=c++11 -O2 -pedantic -I src/ -Wall -Wextra -Werror -o bin/test.o

bin/huffman.o: src/huffman.cpp src/huffman.h | bin/
	g++ src/huffman.cpp -c -std=c++11 -O2 -pedantic -Wall -Wextra -Werror -o bin/huffman.o

bin/autotest.o: test/autotest.cpp test/autotest.h | bin/
	g++ test/autotest.cpp -c -std=c++11 -O2 -pedantic -Wall -Wextra -Werror -o bin/autotest.o

bin/:
	mkdir bin/

clean:
	rm -rf bin/ huffman huffman_test