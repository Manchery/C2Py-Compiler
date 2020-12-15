%{
    #include "node.h"
    NBlock *programBlock; /* the top level root node of our final AST */

    extern int yylex();
    void yyerror(const char *s) { fprintf(stderr, "ERROR: %sn", s); }
%}

%error-verbose

/* Represents the many different ways we can access our data */
%union {
    Node *node;
    NType *type;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector<NVariableDeclaration*> *varvec;
    std::vector<NExpression*> *exprvec;
    std::string *string;
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TIDENTIFIER TINTEGER TDOUBLE TCHAR TSTRING
%token <token> TINTTYPE TDOUBLETYPE TCHARTYPE TVOIDTYPE

%token <token> TIF TELSE TFOR TWHILE

%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TOR TAND
%token <token> TPLUS TMINUS TMUL TDIV

%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TLBRACK TRBRACK
%token <token> TCOMMA TDOT TSEMICOLON

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <type> type
%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program : stmts { programBlock = $1; }
        ;
        
stmts : stmt { fprintf(stderr, "stmts->stmt\n"); $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { fprintf(stderr, "stmts->stmts stmt\n"); $1->statements.push_back($<stmt>2); }
      ;

stmt : func_decl { fprintf(stderr, "stmt->func_decl\n"); }
     | var_decl TSEMICOLON { fprintf(stderr, "stmt->var_decl TSEMICOLON\n"); }
     | expr TSEMICOLON { fprintf(stderr, "stmt->expr TSEMICOLON\n"); $$ = new NExpressionStatement(*$1); }
     ;

block : TLBRACE stmts TRBRACE { fprintf(stderr, "block->TLBRACE stmts TRBRACE"); $$ = $2; }
      | TLBRACE TRBRACE { $$ = new NBlock(); }
      ;

var_decl : type ident { fprintf(stderr, "var_decl->ident ident\n"); $$ = new NVariableDeclaration(*$1, *$2); }
         | type ident TEQUAL expr { fprintf(stderr, "var_decl->ident ident TEQUAL expr\n"); $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;
        
func_decl : type ident TLPAREN func_decl_args TRPAREN block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;
    
func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { fprintf(stderr, "func_decl_args->var_decl\n"); $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
          ;

ident : TIDENTIFIER { std::cerr<<"ident->"<<*$1<<std::endl; $$ = new NIdentifier(*$1); delete $1; }
      ;

numeric: TINTEGER     { fprintf(stderr, "numeric->TINTEGER %s\n", $1->c_str()); $$ = new NInteger(atol($1->c_str())); delete $1; }
       | TDOUBLE      { fprintf(stderr, "numeric->TDOUBLE %s\n", $1->c_str()); $$ = new NDouble(atof($1->c_str())); delete $1; }
       ;

type: TINTTYPE        { $$ = new NType(INT); }
    | TDOUBLETYPE     { $$ = new NType(DOUBLE); }
    | TCHARTYPE       { $$ = new NType(CHAR); }
    | TVOIDTYPE       { $$ = new NType(VOID); }
    ;
    
expr : ident TEQUAL expr { fprintf(stderr, "expr->ident TEQUAL expr\n"); $$ = new NAssignment(*$<ident>1, *$3); }
     | ident TLPAREN call_args TRPAREN { fprintf(stderr, "expr->ident TLPAREN call_args TRPAREN\n"); $$ = new NMethodCall(*$1, *$3); delete $3; }
     | ident { fprintf(stderr, "expr->ident\n"); $<ident>$ = $1; }
     | numeric { fprintf(stderr, "expr->numeric\n"); }
     | expr comparison expr { fprintf(stderr, "expr->expr comparison expr\n"); $$ = new NBinaryOperator(*$1, $2, *$3); }
     | TLPAREN expr TRPAREN { fprintf(stderr, "expr->TLPAREN expr TRPAREN\n"); $$ = $2; }
     ;
    
call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args TCOMMA expr  { $1->push_back($3); }
          ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE 
           | TPLUS | TMINUS | TMUL | TDIV
           ;

%%