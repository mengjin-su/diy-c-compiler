%{
#include <stdio.h>
#include <stdarg.h> 

extern int  yyerror(char *);
extern int  yylex(), yylineno;
extern FILE *yyin;

#define YYDEBUG 1

%}

%token CHAR INT VOID CONSTANT IDENTIFIER

%start program

%%

program
    : external_definitions		{ printf("parsing finished.\n"); }
    ;
external_definitions
    : function_definition
    | external_definitions function_definition
    ;
function_definition
    : type_specifier
	  function_declarator
      compound_statement        { printf("#%d: a function defined\n", yylineno);   }
	;
identifier
	: IDENTIFIER
	;
primary_expr
    : identifier
    | CONSTANT 
    | '(' primary_expr ')'
    ;
assignment_statement
    : identifier
      '=' primary_expr ';'		{ printf("#%d: an assignment statement\n", yylineno);  }
    ;
func_data_declaration
    : type_specifier declarator_list ';' 
    ;{ printf("#%d: define variable\n", yylineno); }
declarator_list
    : identifier
    | declarator_list ',' identifier
    ;
type_specifier
    : CHAR
    | INT
	| VOID
    ;
function_declarator
    : identifier '(' ')'
	;
statement
    : compound_statement
    | assignment_statement
    ;
compound_statement
    : '{' '}'
    | '{' statement_list '}'
    ;
general_statement
	: statement
	| func_data_declaration
	;
statement_list
    : general_statement
	| statement_list general_statement
    ;
%%