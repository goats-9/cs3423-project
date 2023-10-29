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
	g++ ${objs} -o ../Tabulate

parser.tab.o: ${editable_file}
	bison ${parser_source}.y
	g++ parser.tab.cc -c

lex.yy.o: ${editable_file}
	lex ${lex_source}.l
	g++ lex.yy.c -c 

${main_objs}: ${editable_file}
	g++ $*.cc -c

${helping_file}: ${editable_file}
	g++ tabulate/$*.cc -c

clean: all
	rm  ${generated_file} ${objs} 
