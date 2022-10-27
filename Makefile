src = $(wildcard ./src/*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))

CFLAGS = -Wall -c -I ./include 
main = tetris



$(main): $(obj)
	$(CXX) $^ -pthread -o $@

$(obj): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@



.PHONY: clean
clean:
	rm -rf $(main) $(obj)
