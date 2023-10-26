%{
    #include <fstream>
    #include <stdio.h>
    #include "driver.hh"
    #include "parser.tab.hh"
    #include "auxilary.hh"
    using namespace std;

    // Code run each time a pattern is matched.
    # define YY_USER_ACTION  loc.columns (yyleng);
%}

%option noyywrap nounput noinput batch debug

%{
    // Declare token defination as follow
%}

DIGIT [0-9]+
COMMENTS #[^#]*#
SPACE [ \t]
IGNORE ({SPACE}|{COMMENTS})
char_id_start [_a-zA-Z]
char_id [_a-zA-Z0-9]
ID {char_id_start}{char_id}*
DATATYPE1 "int"|"double"|"string"|"bool"|"date"|"time"
DATATYPE2 "cell"|"range"|"array"|"table"|"formula"
DATATYPE {DATATYPE1}|{DATATYPE2}
ARITHMETIC "ADD"|"SUB"|"MUL"|"DIV"|"MOD"|"POW"
bi_OP "=="|"!="|">"|"<"|">="|"<="|"AND"|"OR"|"XOR"
uni_OP "NOT"|"-"|"~"|"TYPEOF"
EQUAL "="
open_square_brac "["
close_square_brac "]"
colon ":"
dot "."

%%
%{
    // location
    yy::location& loc = drv.location;
    // Code run each time yylex is called.
    loc.step ();
    // Add token defination below
    // Some tokens are added for reference
%}

"struct" {
    auto token = yy::parser::make_STRUCT(loc);
    helper::handleToken(token);
    return token;
}
{DATATYPE} {
    auto token = yy::parser::make_DATATYPE(yytext,loc);
    helper::handleToken(token);
    return token;
}
{ARITHMETIC} {
    auto token = yy::parser::make_ARITHMETIC(yytext,loc);
    helper::handleToken(token);
    return token;
}
{bi_OP} {
    auto token = yy::parser::make_BIOP(yytext,loc);
    helper::handleToken(token);
    return token;
}
{uni_OP} {
    auto token = yy::parser::make_UNIOP(yytext,loc);
    helper::handleToken(token);
    return token;
}
{EQUAL} {
    auto token = yy::parser::make_EQUAL(yytext,loc);
    helper::handleToken(token);
    return token;
}
{open_square_brac} {
    auto token = yy::parser::make_OPEN_SQUARE_BRAC(yytext,loc);
    helper::handleToken(token);
    return token;
}
{close_square_brac} {
    auto token = yy::parser::make_CLOSE_SQUARE_BRAC(yytext,loc);
    helper::handleToken(token);
    return token;
}
{colon} {
    auto token = yy::parser::make_COLON(yytext,loc);
    helper::handleToken(token);
    return token;
}
{dot} {
    auto token = yy::parser::make_DOT(yytext,loc);
    helper::handleToken(token);
    return token;
}
{DIGIT} {
    auto token = yy::parser::make_DIGIT(stoi(yytext), loc);
    helper::handleToken(token);
    return token;
}
{ID} {
    auto token = yy::parser::make_ID(yytext, loc);
    helper::handleToken(token);
    return token;
}
{IGNORE} {
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


