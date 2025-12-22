%{
#include <cstdio>
#include <iostream>
#include <fstream>
#include "ast.h"

// Оголошення функцій
int yylex();
void yyerror(const char* s);

// Глобальна змінна для кореня дерева
Node* root = nullptr;
std::map<std::string, int> symbolTable;
%}

%union {
    int ival;
    std::string* sval;
    Node* node;
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token INT PLUS MINUS MULT DIV ASSIGN SEMI LPAREN RPAREN

%type <node> expression term factor assignment

/* Пріоритет операцій (від найнижчого до найвищого) */
%left PLUS MINUS
%left MULT DIV

%%

program:
    program statement
    | statement
    ;

statement:
    assignment SEMI {
        root = $1; // Зберігаємо останній стейтмент як корінь для демо
    }
    ;

assignment:
    INT IDENTIFIER ASSIGN expression {
        symbolTable[*$2] = $4->evaluate(); // Обчислюємо і зберігаємо
        $$ = new OpNode('=', new IdNode(*$2), $4);
        delete $2;
    }
    | IDENTIFIER ASSIGN expression {
         symbolTable[*$1] = $3->evaluate();
         $$ = new OpNode('=', new IdNode(*$1), $3);
         delete $1;
    }
    ;

expression:
    expression PLUS term { $$ = new OpNode('+', $1, $3); }
    | expression MINUS term { $$ = new OpNode('-', $1, $3); }
    | term { $$ = $1; }
    ;

term:
    term MULT factor { $$ = new OpNode('*', $1, $3); }
    | term DIV factor { $$ = new OpNode('/', $1, $3); }
    | factor { $$ = $1; }
    ;

factor:
    NUMBER { $$ = new NumberNode($1); }
    | IDENTIFIER { $$ = new IdNode(*$1); delete $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char* s) {
    std::cerr << "Error: " << s << std::endl;
}

int main() {
    std::cout << "Enter code (e.g., int x = 2 + 3 * 4;):" << std::endl;
    yyparse();

    if (root) {
        std::cout << "\nComputed Result: " << root->evaluate() << std::endl;

        // Генерація JSON для візуалізації
        std::ofstream jsonFile("ast_data.json");
        root->printJSON(jsonFile);
        jsonFile.close();
        std::cout << "AST exported to ast_data.json" << std::endl;
    }
    return 0;
}