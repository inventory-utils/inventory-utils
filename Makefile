.PHONY: all test clean example_outs

all: inventory-where-backend example_outs #inventory-what-frontend

# Most rules use implicit variables, so no need to be set, check: https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
OWNCXXFLAGS=-std=c++14

obj/%.o: src/%.cpp src/%.hpp #$(DEPS)
	mkdir -p obj
	$(CXX) -c -o $@ $< $(OWNCXXFLAGS) $(CXXFLAGS)

# special rule as %-main.cpp files, does not have complimentary .hpp.
obj/%-main.o: src/%-main.cpp #$(DEPS)
	mkdir -p obj
	$(CXX) -c -o $@ $< $(OWNCXXFLAGS) $(CXXFLAGS)

# should work for both: inventory-where-backend and inventory-what-frontend
inventory-%: obj/inventory-%-main.o obj/inventory-%-datastructures.o
	$(CXX) -o $@ $^ $(OWNCXXFLAGS) $(CXXFLAGS)

test:
	true

clean:
	rm -f obj/*.o inventory-where-backend #inventory-what-frontend

example_outs: examples/where-outs/few_items_open_close_where_dump.stderr examples/where-outs/few_items_open_close_where_dump.stdout

examples/where-outs/%.stderr: examples/where-ins/%.inv inventory-where-backend
	mkdir -p examples/where-outs
	./inventory-where-backend < $< 2>$@ 1>/dev/null

examples/where-outs/%.stdout: examples/where-ins/%.inv inventory-where-backend
	mkdir -p examples/where-outs
	./inventory-where-backend < $< 1>$@ 2>/dev/null
