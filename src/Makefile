BISON = bison
FLEX = flex
CXX = g++
lex_source = lex
parser_source = parser
helping_file = types.o driver.o
parsing_objs = parser.tab.o lex.yy.o
main_objs = tabulate.o main.o
objs = ${parsing_objs} ${helping_file} ${main_objs}
generated_file = lex.yy.c ${parser_source}.tab.cc ${parser_source}.tab.hh location.hh
editable_helping_file = driver.cc driver.hh symtab.cc symtab.hh types.cc types.hh
editable_file = ${parser_source}.y ${lex_source}.l main.cc tabulate.cc tabulate.hh \
 $(addprefix tabulate/,${editable_helping_file})

all: ../Tabulate

../Tabulate: ${objs}
	$(CXX) ${objs} -o ../Tabulate

parser.tab.o:
	$(BISON) ${parser_source}.y
	$(CXX) parser.tab.cc -c

lex.yy.o:
	$(FLEX) ${lex_source}.l
	$(CXX) lex.yy.c -c 

${main_objs}:
	$(CXX) $*.cc -c

${helping_file}:
	$(CXX) tabulate/$*.cc -c

clean:
	rm ${generated_file} ${objs} 
