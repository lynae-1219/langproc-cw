// parser.y
// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

%code requires {
	#include "ast.hpp"
	using namespace ast;

	extern int yylineno;
	extern char* yytext;
	extern Node* g_root;
	extern FILE* yyin;

	int yylex(void);
	void yyerror(const char*);
	int yylex_destroy(void);
}

%define parse.error detailed
%define parse.lac full

%union {
  Node*				    node;
  NodeList*			  node_list;
  int          		number_int;
  double       		number_float;
  std::string*		string;
  TypeSpecifier 	type_specifier;
  yytokentype  		token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token UNKNOWN

%type <node> translation_unit external_declaration function_definition primary_expression postfix_expression
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression declarator direct_declarator statement compound_statement jump_statement
%type <node> declaration init_declarator

%type <node_list> statement_list init_declarator_list parameter_list

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER
%type <type_specifier> type_specifier declaration_specifiers


%start ROOT
%%

ROOT
	: translation_unit { g_root = $1; }
	;

translation_unit
	: external_declaration { $$ = new NodeList(NodePtr($1)); }
    | translation_unit external_declaration { dynamic_cast<NodeList*>($1)->PushBack(NodePtr($2)); $$ = $1; }
	;

external_declaration
	: function_definition { $$ = $1; }
    | declaration         { $$ = $1; }
	;

function_definition
	: declaration_specifiers declarator compound_statement {
        auto dd = dynamic_cast<DirectDeclarator*>($2);
        auto params = dd ? dd->TakeParameters() : nullptr;
		$$ = new FunctionDefinition($1, NodePtr($2), std::move(params), NodePtr($3));
	}
	;

declaration
    : declaration_specifiers init_declarator_list ';' {
        $$ = new Declaration(NodePtr($2));
    }
    ;

declaration_specifiers
	: type_specifier { $$ = $1; }
	;

type_specifier
	: INT { $$ = TypeSpecifier::INT; }
	;

init_declarator_list
    : init_declarator { $$ = new NodeList(NodePtr($1)); }
    | init_declarator_list ',' init_declarator { $1->PushBack(NodePtr($3)); $$ = $1; }
    ;

init_declarator
    : declarator { $$ = new InitDeclarator(NodePtr($1), nullptr); }
    | declarator '=' assignment_expression { $$ = new InitDeclarator(NodePtr($1), NodePtr($3)); }
    ;

declarator
	: direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER {
		$$ = new DirectDeclarator(NodePtr(new Identifier(std::move(*$1))));
		delete $1;
	}
    | '(' declarator ')' { $$ = $2; }
	| direct_declarator '(' ')' {
        $$ = $1; // No parameters
	}
    | direct_declarator '(' parameter_list ')' {
        dynamic_cast<DirectDeclarator*>($1)->SetParameters(NodePtr($3));
        $$ = $1;
    }
	;

parameter_list
    : declaration { $$ = new NodeList(NodePtr($1)); } // Simplified parameter declaration
    | parameter_list ',' declaration { $1->PushBack(NodePtr($3)); $$ = $1; }
    ;

statement
    : compound_statement { $$ = $1; }
	| expression ';'     { $$ = $1; }
    | jump_statement     { $$ = $1; }
    | declaration        { $$ = $1; }
	;

compound_statement
	: '{' '}' { $$ = new CompoundStatement(nullptr); }
    | '{' statement_list '}' { $$ = new CompoundStatement(NodePtr($2)); }
	;

statement_list
	: statement { $$ = new NodeList(NodePtr($1)); }
	| statement_list statement { $1->PushBack(NodePtr($2)); $$=$1; }
	;

jump_statement
	: RETURN ';' { $$ = new ReturnStatement(nullptr); }
	| RETURN expression ';' { $$ = new ReturnStatement(NodePtr($2)); }
	;

primary_expression
	: IDENTIFIER { $$ = new Identifier(std::move(*$1)); delete $1; }
    | INT_CONSTANT { $$ = new IntConstant($1); }
    | '(' expression ')' { $$ = $2; }
	;

postfix_expression
	: primary_expression { $$ = $1; }
	;

unary_expression
	: postfix_expression { $$ = $1; }
	;

cast_expression
	: unary_expression { $$ = $1; }
	;

multiplicative_expression
	: cast_expression { $$ = $1; }
	;

additive_expression
	: multiplicative_expression { $$ = $1; }
    | additive_expression '+' multiplicative_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "+"); }
	;

shift_expression
	: additive_expression { $$ = $1; }
	;

relational_expression
	: shift_expression { $$ = $1; }
	;

equality_expression
	: relational_expression { $$ = $1; }
	;

and_expression
	: equality_expression { $$ = $1; }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	;

conditional_expression
	: logical_or_expression { $$ = $1; }
	;

assignment_expression
	: conditional_expression { $$ = $1; }
    | unary_expression '=' assignment_expression { $$ = new Assign(NodePtr($1), NodePtr($3)); }
	;

expression
	: assignment_expression { $$ = $1; }
	;
%%

void yyerror (const char *s)
{
  std::cerr << "Error: " << s << " at line " << yylineno;
  std::cerr << " near '" << yytext << "'" << std::endl;
  std::exit(1);
}

Node* g_root;
NodePtr ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if (yyin == nullptr) {
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    std::exit(1);
  }

  g_root = nullptr;
  yyparse();

  fclose(yyin);
  yylex_destroy();

  return NodePtr(g_root);
}