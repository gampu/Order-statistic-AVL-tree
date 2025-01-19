test: test/test.out
	./test/test.out
	
test/test.out: src/avl.h test/avl_test.cpp
	g++ -std=c++17 test/avl_test.cpp -o test/test.out

clean:
	rm test/test.out