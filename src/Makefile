lex_source = lex
parser_source = parser
helping_file = types.o driver.o
parsing_objs = parser.tab.o lex.yy.o
main_objs = tabulate.o main.o
objs = ${parsing_objs} ${helping_file} ${main_objs}
generated_file = lex.yy.c ${parser_source}.tab.cc ${parser_source}.tab.hh location.hh

all: ${objs}
	g++ ${objs} -o ../Tabulate

parser.tab.o: 
	bison ${parser_source}.y
	g++ parser.tab.cc -c

lex.yy.o: 
	lex ${lex_source}.l
	g++ lex.yy.c -c 

${main_objs}:
	g++ $*.cc -c

${helping_file}: 
	g++ tabulate/$*.cc -c

clean: all
	rm  ${generated_file} ${objs} 
