%{
    #include <fstream>
    #include <stdio.h>
    #include "driver.hh"
    #include "parser.tab.hh"
    using namespace std;

    // Code run each time a pattern is matched.
    # define YY_USER_ACTION  loc.columns (yyleng);
%}

%option noyywrap nounput noinput batch debug

%{
    // Declare token defination as follow
%}

DIGIT [0-9]+

%%
%{
    // location
    yy::location& loc = drv.location;
    // Code run each time yylex is called.
    loc.step ();
%}

%{
    // Add token defination below
    // Some tokens are added for reference
%}

{DIGIT} {
    return yy::parser::make_DIGIT(stoi(yytext), loc);
}
[ \t] {
    loc.step();
}
\n {loc.lines (yyleng); loc.step();}
. {throw yy::parser::syntax_error(loc, "invalid character: " + string(yytext));}
<<EOF>>    return yy::parser::make_YYEOF (loc);
%%

void driver::scan_begin ()
{
    yy_flex_debug = trace_scanning;
    if (file.empty ())
        yyin = stdin;
    else if (!(yyin = fopen (file.c_str (), "r")))
    {
        std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
        exit (EXIT_FAILURE);
    }
}


void driver::scan_end ()
{
    fclose (yyin);
}


