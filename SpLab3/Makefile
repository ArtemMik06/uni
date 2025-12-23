all:
	flex lexer.l
	bison -d parser.y
	g++ -std=c++17 parser.tab.c lex.yy.c -o compiler

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c compiler ast_data.json huge_test.txt