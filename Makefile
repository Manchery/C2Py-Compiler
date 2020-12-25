all: parser

clean:
	rm parser.cpp parser.hpp parser tokens.cpp

parser.cpp: parser.y
	bison -d -o $@ $^ --debug
    
parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	lex -d -o $@ $^

parser: parser.cpp codegen.cpp main.cpp tokens.cpp
	g++ -g -o $@ $^ -DYYDEBUG=1 -std=c++11