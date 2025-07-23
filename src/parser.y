%code requires {
	#include "ast.hpp"
	using namespace ast;

	
    extern int yylineno;
    extern char* yytext;
    extern ast::Node* g_root;
    extern FILE* yyin;

    int yylex(void);
    void yyerror(const char*);
    int yylex_destroy(void);
}

%define parse.error detailed
%define parse.lac full

%union {
    ast::Node* node;
    ast::NodeList* node_list;
    int number_int;
    double number_float;
    std::string* string;
    ast::Context::Type type_specifier;
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

%type <node_list> statement_list

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
    : external_declaration { $$ = $1; }
    ;

external_declaration
    : function_definition { $$ = $1; }
    ;

function_definition
    : declaration_specifiers declarator compound_statement {
        auto list = new ast::NodeList();
        list->PushBack(std::unique_ptr<ast::Node>($3));
        $$ = list;
    }
    ;

declaration_specifiers
    : type_specifier { $$ = $1; }
    ;

type_specifier
    : INT {
        $$ = ast::Context::Type::INT;
    }
    | VOID {
        $$ = ast::Context::Type::VOID;
    }
    ;

declarator
    : direct_declarator { $$ = $1; }
    ;

direct_declarator
    : IDENTIFIER {
        // Create a basic node for the identifier
        $$ = new ast::NodeList(); // Placeholder - should be Identifier node
        delete $1;
    }
    | direct_declarator '(' ')' {
        // Function declarator - just pass through for now
        $$ = $1;
    }
    ;

statement
    : compound_statement { $$ = $1; }
    | jump_statement { $$ = $1; }
    ;

compound_statement
    : '{' '}' {
        $$ = new ast::NodeList();
    }
    | '{' statement_list '}' { 
        $$ = $2; 
    }
    ;

statement_list
    : statement { 
        auto list = new ast::NodeList();
        list->PushBack(std::unique_ptr<ast::Node>($1));
        $$ = list;
    }
    | statement_list statement { 
        $1->PushBack(std::unique_ptr<ast::Node>($2)); 
        $$ = $1; 
    }
    ;

jump_statement
    : RETURN ';' {
        $$ = new ast::ReturnStatement(nullptr);
    }
    | RETURN expression ';' {
        $$ = new ast::ReturnStatement(std::unique_ptr<ast::Node>($2));
    }
    | BREAK ';' {
        $$ = new ast::BreakStatement();
    }
    | CONTINUE ';' {
        $$ = new ast::ContinueStatement();
    }
    | GOTO IDENTIFIER ';' {
        $$ = new ast::GotoStatement(*$2);
        delete $2;
    }
    ;

primary_expression
    : IDENTIFIER {
        // Placeholder - should create Identifier node
        $$ = new ast::NodeList();
        delete $1;
    }
    | INT_CONSTANT {
        // Placeholder - should create IntConstant node
        $$ = new ast::NodeList();
    }
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
    ;

expression
    : assignment_expression { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    std::cerr << "Error: " << s << " at line " << yylineno;
    std::cerr << " near '" << yytext << "'" << std::endl;
    std::exit(1);
}

ast::Node* g_root = nullptr;

std::unique_ptr<ast::Node> ParseAST(std::string file_name) {
    yyin = fopen(file_name.c_str(), "r");
    if (yyin == nullptr) {
        std::cerr << "Couldn't open input file: " << file_name << std::endl;
        std::exit(1);
    }

    g_root = nullptr;
    yyparse();

    fclose(yyin);
    yylex_destroy();

    return std::unique_ptr<ast::Node>(g_root);
}