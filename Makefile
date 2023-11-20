EXEFILE = raytracer
CXXFLAGS = -c -Wall -std=c++11 
SOURCES = $(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

$(EXEFILE): $(OBJECTS)
	g++ $^ -o $@

%.o: %.cpp
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm src/*.o $(EXEFILE)

.PHONY: clean
