CXX := g++
CXXFLAGS := -Wall
SRC := Tester.cpp Network.cpp TextField.cpp Button.cpp pugixml.cpp
OBJ := Network.o TextField.o Button.o pugixml.o

runme:  $(OBJ)
	$(CXX) $(CXXFLAGS) -o FuzzyTester.exe Tester.cpp $(OBJ)

$(OBJ): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

debug: CXXFLAGS += -g
debug: $(OBJ)
	$(CXX) $(CXXFLAGS) -o dbgFuzzyTester Tester.cpp $(OBJ)
	gdb	dbgprog02

clean:
	rm $(OBJ)

depend: $(SRC)
	$(CXX) -MM $^ > $@

include depend