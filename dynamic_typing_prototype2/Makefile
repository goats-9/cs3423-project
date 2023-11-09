OBJS = types.o any.o inbuilt.o helper.o
CXX = g++

all: run

run: translated.o
	$(CXX) $(OBJS) translated.o -o run

runtime: $(OBJS)

types.o: types.hh any.hh

any.o: any.hh types.hh

inbuilt.o: inbuilt.hh any.hh types.hh

helper.o: any.hh helper.hh

clean: runtime all
	rm $(OBJS) translated.o run