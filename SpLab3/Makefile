all:
	# Використовуємо flex та bison
	flex lexer.l
	bison -d parser.y
	# Компілюємо. На Mac іноді треба явно вказувати стандарт C++17 або 11
	g++ -std=c++17 parser.tab.c lex.yy.c -o compiler

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c compiler ast_data.json huge_test.txt