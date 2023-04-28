compile:
	gcc -c lexer.c
	gcc -c chash.c
	gcc -c parser.c
	gcc -c driver.c
	gcc -c parseTree.c
	gcc -c symbolTable.c
	gcc -c ast.c
	gcc -c semanticAnalyzer.c
	gcc -c intermediateCodegen.c
	gcc -c codegen.c
	gcc -o compiler lexer.o chash.o parser.o driver.o parseTree.o symbolTable.o ast.o semanticAnalyzer.o intermediateCodegen.o codegen.o

debug:
	gcc -g -c lexer.c
	gcc -g -c chash.c
	gcc -g -c parser.c
	gcc -g -c driver.c
	gcc -g -c parseTree.c
	gcc -g -c symbolTable.c
	gcc -g -c ast.c
	gcc -g -c semanticAnalyzer.c
	gcc -g -c intermediateCodegen.c
	gcc -g -c codegen.c
	gcc -g -o compiler lexer.o chash.o parser.o driver.o parseTree.o symbolTable.o ast.o semanticAnalyzer.o intermediateCodegen.o codegen.o