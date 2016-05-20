.PHONY: all test clean

all: inventory-where-backend #inventory-what-frontend

# Most of below are implicit variables, so no need to be set, check: https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
#CXXFLAGS= #e.g. -I.
#LIBS= #e.g. -lm
#DEPS = future_commmon_dependency.h
#OBJDIR=obj
#LIBDIR =../lib

obj/%.o: src/%.cpp src/%.hpp obj #$(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# special rule as %-main.cpp files, does not have complimentary .hpp.
obj/%-main.o: src/%-main.cpp obj #$(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# should work for both: inventory-where-backend and inventory-what-frontend
inventory-%: obj/inventory-%-main.o obj/inventory-%-datastructures.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

obj:
	mkdir obj

test:
	true

clean:
	rm -f obj/*.o inventory-where-backend #inventory-what-frontend
