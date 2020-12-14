%{
#include <string>
#include "node.h"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%%

[ \t\n]                                 ;
#include\\s*([<\"])([^>\"]+)([>\"])     printf("include line\n");

"if"                                    return TOKEN(TIF);
"else"                                  return TOKEN(TELSE);
"for"                                   return TOKEN(TFOR);
"while"                                 return TOKEN(TWHILE);

[a-zA-Z_][a-zA-Z0-9_]*                  SAVE_TOKEN; return TIDENTIFIER;
[0-9]+\.[0-9]*                          SAVE_TOKEN; return TDOUBLE;
[0-9]+                                  SAVE_TOKEN; return TINTEGER;
['].[']                                 SAVE_TOKEN; return TCHAR;
["].*["]                                SAVE_TOKEN; return TSTRING;

"="                                     return TOKEN(TEQUAL);

"=="                                    return TOKEN(TCEQ);
"!="                                    return TOKEN(TCNE);
"<"                                     return TOKEN(TCLT);
"<="                                    return TOKEN(TCLE);
">"                                     return TOKEN(TCGT);
">="                                    return TOKEN(TCGE);

"&&"                                    return TOKEN(TAND);
"||"                                    return TOKEN(TOR);

"("                                     return TOKEN(TLPAREN);
")"                                     return TOKEN(TRPAREN);
"{"                                     return TOKEN(TLBRACE);
"}"                                     return TOKEN(TRBRACE);
"["                                     return TOKEN(TLBRACK);
"]"                                     return TOKEN(TRBRACK);

"."                                     return TOKEN(TDOT);
","                                     return TOKEN(TCOMMA);
";"                                     return TOKEN(TSEMICOLON);

"+"                                     return TOKEN(TPLUS);
"-"                                     return TOKEN(TMINUS);
"*"                                     return TOKEN(TMUL);
"/"                                     return TOKEN(TDIV);

.                                       printf("Unknown token!n"); yyterminate();

%%