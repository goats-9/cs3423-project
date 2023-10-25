lex_source = lex.l
parser_source = parser.y
C_File = lex.yy.c parser.tab.cc
objs = parser.tab.o lex.yy.o driver.o main.o
generated_header_file = parser.tab.hh location.hh
driver_header_file = driver.hh
header_File = ${generated_header_file} ${driver_header_file}
driver_C_file = driver.cc

tabulate: ${objs}
	g++ ${objs} -o ../Tabulate
	rm ${objs} ${C_File} ${generated_header_file}

parser.tab.o: ${parser_source} ${driver_header_file}
	bison ${parser_source}
	g++ parser.tab.cc -c

lex.yy.o: ${lex_source} ${header_File}
	lex ${lex_source}
	g++ lex.yy.c -c 

driver.o: ${driver_C_file} ${header_File}
	g++ ${driver_C_file} -c

main.o: main.cc ${driver_header_file}
	g++ main.cc -c