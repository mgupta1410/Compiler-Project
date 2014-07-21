stage1exe: lexer.c parser.c stack.c tree.c TypeChecker.c semanticAnalyzer.c hashTable.c ast.c  
	gcc -o mycompiler driver.c lexer.c parser.c stack.c tree.c TypeChecker.c semanticAnalyzer.c hashTable.c ast.c 
