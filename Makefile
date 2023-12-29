CXX=g++
CXXFLAGS=-Wall -g
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
REACT_LIB=/usr/local/cs/cs251/react.o
REACT_OPTIONS=-lcurl
REACT_BRIDGE=/usr/local/cs/cs251/bridge.py

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(REACT_LIB) $(REACT_OPTIONS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

bridge:
	python $(REACT_BRIDGE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
