.PHONY: all clean

all: inventory-where-backend #inventory-what-frontend

CPP=g++
#CPP=clang++
CFLAGS= #e.g. -I.
LIBS= #e.g. -lm
#DEPS = future_commmon_dependency.h
#OBJDIR=obj
#LIBDIR =../lib

obj/%.o: src/%.cpp src/%.hpp obj #$(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

# special rule as %-main.cpp files, does not have complimentary .hpp.
obj/%-main.o: src/%-main.cpp obj #$(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

# should work for both: inventory-where-backend and inventory-what-frontend
inventory-%: obj/inventory-%-main.o obj/inventory-%-datastructures.o
	$(CPP) -o $@ $^ $(CFLAGS)

obj:
	mkdir obj

clean:
	rm -f obj/*.o inventory-where-backend #inventory-what-frontend
