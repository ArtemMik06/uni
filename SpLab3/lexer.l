%{
#include "ast.h"
#include "parser.tab.h"
#include <string>
%}

%option noyywrap

%%

[ \t\n]         ; // Ігноруємо пробіли
"int"           { return INT; }
[0-9]+          { yylval.ival = std::stoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = new std::string(yytext); return IDENTIFIER; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"="             { return ASSIGN; }
";"             { return SEMI; }
"("             { return LPAREN; }
")"             { return RPAREN; }
.               { printf("Unknown character: %s\n", yytext); }

%%