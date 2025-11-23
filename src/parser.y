%code requires {
    #include "ast.hpp"
    #include "ast_if_statement.hpp"
    #include "ast_update_expression.hpp"
    #include "ast_for_statement.hpp"
    #include "ast_array_access.hpp"
    #include "ast_unary_expression.hpp"
    #include "ast_address_of.hpp"
    #include "ast_dereference.hpp"
    #include "ast_enum.hpp"
    #include "ast_typedef.hpp"
    #include "ast_compound_assign.hpp"
    #include "ast_sizeof.hpp"
    #include "ast_string_literal.hpp"
    #include "ast_struct.hpp"
    #include <set>
    #include <map>
    using namespace ast;

    extern int yylineno;
    extern char* yytext;
    extern Node* g_root;
    extern FILE* yyin;
    extern std::set<std::string> g_typedef_names;
    extern std::map<std::string, TypeSpecifier> g_typedef_types;

    int yylex(void);
    void yyerror(const char*);
    int yylex_destroy(void);
}

%define parse.error detailed
%define parse.lac full

%union {
  Node* node;
  NodeList* node_list;
  int               number_int;
  double            number_float;
  std::string* string;
  TypeSpecifier     type_specifier;
  yytokentype       token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL TYPEDEF_NAME
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token UNKNOWN

%type <node> translation_unit external_declaration function_definition primary_expression postfix_expression
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression
%type <node> relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression
%type <node> logical_and_expression logical_or_expression conditional_expression assignment_expression expression
%type <node> declarator direct_declarator statement compound_statement jump_statement declaration
%type <node> init_declarator parameter_declaration selection_statement iteration_statement
%type <node> expression_statement labeled_statement enum_specifier enumerator struct_specifier

%type <node_list> statement_list init_declarator_list parameter_list argument_expression_list enumerator_list switch_body struct_declaration_list

%type <number_int> INT_CONSTANT
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER TYPEDEF_NAME STRING_LITERAL
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
    | declaration {
        auto* decl = dynamic_cast<Declaration*>($1);
        if (decl) decl->SetGlobal(true);
        $$ = $1;
    }
    | enum_specifier ';' { $$ = $1; }
    | TYPEDEF type_specifier IDENTIFIER ';' {
        g_typedef_names.insert(*$3);
        g_typedef_types[*$3] = $2;
        $$ = new TypedefDeclaration($2, std::move(*$3));
        delete $3;
    }
    | TYPEDEF type_specifier '*' IDENTIFIER ';' {
        g_typedef_names.insert(*$4);
        g_typedef_types[*$4] = $2;
        $$ = new TypedefDeclaration($2, std::move(*$4), true);
        delete $4;
    }
    | struct_specifier ';' { $$ = $1; }
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
        $$ = new Declaration($1, NodePtr($2));
    }
    | STRUCT IDENTIFIER IDENTIFIER ';' {
        $$ = new StructVariableDecl(std::move(*$2), std::move(*$3));
        delete $2;
        delete $3;
    }
    ;

declaration_specifiers
    : type_specifier { $$ = $1; }
    ;

type_specifier
    : INT { $$ = TypeSpecifier::INT; }
    | FLOAT { $$ = TypeSpecifier::FLOAT; }
    | DOUBLE { $$ = TypeSpecifier::DOUBLE; }
    | CHAR { $$ = TypeSpecifier::CHAR; }
    | UNSIGNED { $$ = TypeSpecifier::UNSIGNED; }
    | VOID { $$ = TypeSpecifier::VOID; }
    | TYPEDEF_NAME { $$ = g_typedef_types[*$1]; delete $1; }
    ;

enum_specifier
    : ENUM IDENTIFIER '{' enumerator_list '}' {
        $$ = new EnumDeclaration(std::move(*$2), NodePtr($4));
        delete $2;
    }
    | ENUM '{' enumerator_list '}' {
        $$ = new EnumDeclaration(NodePtr($3));
    }
    ;

enumerator_list
    : enumerator { $$ = new NodeList(NodePtr($1)); }
    | enumerator_list ',' enumerator { $1->PushBack(NodePtr($3)); $$ = $1; }
    ;

enumerator
    : IDENTIFIER {
        $$ = new Enumerator(std::move(*$1));
        delete $1;
    }
    | IDENTIFIER '=' INT_CONSTANT {
        $$ = new Enumerator(std::move(*$1), NodePtr(new IntConstant($3)));
        delete $1;
    }
    ;

struct_specifier
    : STRUCT IDENTIFIER '{' struct_declaration_list '}' {
        $$ = new StructDefinition(std::move(*$2), NodePtr($4));
        delete $2;
    }
    ;

struct_declaration_list
    : declaration { $$ = new NodeList(NodePtr($1)); }
    | struct_declaration_list declaration { $1->PushBack(NodePtr($2)); $$ = $1; }
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
    | '*' declarator {
        auto* dd = dynamic_cast<DirectDeclarator*>($2);
        if (dd) dd->SetIsPointer(true);
        $$ = $2;
    }
    ;

direct_declarator
    : IDENTIFIER {
        $$ = new DirectDeclarator(NodePtr(new Identifier(std::move(*$1))));
        delete $1;
    }
    | '(' declarator ')' { $$ = $2; }
    | direct_declarator '[' INT_CONSTANT ']' {
        dynamic_cast<DirectDeclarator*>($1)->SetArraySize($3);
        $$ = $1;
    }
    | direct_declarator '(' ')' {
        dynamic_cast<DirectDeclarator*>($1)->SetIsFunction(true);
        $$ = $1;
    }
    | direct_declarator '(' parameter_list ')' {
        dynamic_cast<DirectDeclarator*>($1)->SetParameters(NodePtr($3));
        dynamic_cast<DirectDeclarator*>($1)->SetIsFunction(true);
        $$ = $1;
    }
    ;

parameter_list
    : parameter_declaration { $$ = new NodeList(NodePtr($1)); }
    | parameter_list ',' parameter_declaration { $1->PushBack(NodePtr($3)); $$ = $1; }
    ;

parameter_declaration
    : declaration_specifiers declarator {
        NodePtr init_decl = NodePtr(new InitDeclarator(NodePtr($2), nullptr));
        auto init_list = new NodeList(std::move(init_decl));
        $$ = new Declaration($1, NodePtr(init_list));
    }
    ;

statement
    : compound_statement { $$ = $1; }
    | expression_statement { $$ = $1; }
    | jump_statement { $$ = $1; }
    | declaration { $$ = $1; }
    | selection_statement { $$ = $1; }
    | iteration_statement { $$ = $1; }
    | labeled_statement { $$ = $1; }
    ;

labeled_statement
    : CASE INT_CONSTANT ':' statement { $$ = new CaseStatement(NodePtr(new IntConstant($2)), NodePtr($4)); }
    | DEFAULT ':' statement { $$ = new DefaultCaseStatement(NodePtr($3)); }
    ;

expression_statement
    : ';' { $$ = nullptr; }
    | expression ';' { $$ = $1; }
    ;

iteration_statement
    : WHILE '(' expression ')' statement { $$ = new WhileStatement(NodePtr($3), NodePtr($5)); }
    | FOR '(' expression_statement expression_statement ')' statement {
        $$ = new ForStatement(NodePtr($3), NodePtr($4), nullptr, NodePtr($6));
    }
    | FOR '(' expression_statement expression_statement expression ')' statement {
        $$ = new ForStatement(NodePtr($3), NodePtr($4), NodePtr($5), NodePtr($7));
    }
    ;

selection_statement
    : IF '(' expression ')' statement { $$ = new IfStatement(NodePtr($3), NodePtr($5), nullptr); }
    | IF '(' expression ')' statement ELSE statement { $$ = new IfStatement(NodePtr($3), NodePtr($5), NodePtr($7)); }
    | SWITCH '(' expression ')' '{' switch_body '}' { $$ = new SwitchStatement(NodePtr($3), NodePtr($6)); }
    ;

switch_body
    : statement { $$ = new NodeList(NodePtr($1)); }
    | switch_body statement { $1->PushBack(NodePtr($2)); $$ = $1; }
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
    | BREAK ';' { $$ = new BreakStatement(); }
    | CONTINUE ';' { $$ = new ContinueStatement(); }
    ;

primary_expression
    : IDENTIFIER { $$ = new Identifier(std::move(*$1)); delete $1; }
    | INT_CONSTANT { $$ = new IntConstant($1); }
    | FLOAT_CONSTANT { $$ = new FloatConstant($1); }
    | STRING_LITERAL { $$ = new StringLiteral(std::move(*$1)); delete $1; }
    | '(' expression ')' { $$ = $2; }
    ;

postfix_expression
    : primary_expression { $$ = $1; }
    | postfix_expression '[' expression ']' { $$ = new ArrayAccess(NodePtr($1), NodePtr($3)); }
    | postfix_expression '(' ')' { $$ = new FunctionCall(NodePtr($1), nullptr); }
    | postfix_expression '(' argument_expression_list ')' { $$ = new FunctionCall(NodePtr($1), NodePtr($3)); }
    | postfix_expression INC_OP { $$ = new UpdateExpression(NodePtr($1), false, false); }
    | postfix_expression DEC_OP { $$ = new UpdateExpression(NodePtr($1), false, true); }
    | postfix_expression '.' IDENTIFIER { $$ = new StructMemberAccess(NodePtr($1), std::move(*$3)); delete $3; }
    ;

argument_expression_list
    : assignment_expression { $$ = new NodeList(NodePtr($1)); }
    | argument_expression_list ',' assignment_expression { $1->PushBack(NodePtr($3)); $$ = $1; }
    ;

unary_expression
    : postfix_expression { $$ = $1; }
    | INC_OP unary_expression { $$ = new UpdateExpression(NodePtr($2), true, false); }
    | DEC_OP unary_expression { $$ = new UpdateExpression(NodePtr($2), true, true); }
    | '-' cast_expression { $$ = new UnaryExpression(NodePtr($2), "-"); }
    | '+' cast_expression { $$ = new UnaryExpression(NodePtr($2), "+"); }
    | '!' cast_expression { $$ = new UnaryExpression(NodePtr($2), "!"); }
    | '~' cast_expression { $$ = new UnaryExpression(NodePtr($2), "~"); }
    | '&' cast_expression { $$ = new AddressOf(NodePtr($2)); }
    | '*' cast_expression { $$ = new Dereference(NodePtr($2)); }
    | SIZEOF '(' type_specifier ')' { $$ = new SizeofExpr($3); }
    | SIZEOF '(' unary_expression ')' { $$ = new SizeofExpr(NodePtr($3)); }
    ;

cast_expression
    : unary_expression { $$ = $1; }
    ;

multiplicative_expression
    : cast_expression { $$ = $1; }
    | multiplicative_expression '*' cast_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "*"); }
    | multiplicative_expression '/' cast_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "/"); }
    | multiplicative_expression '%' cast_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "%"); }
    ;

additive_expression
    : multiplicative_expression { $$ = $1; }
    | additive_expression '+' multiplicative_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "+"); }
    | additive_expression '-' multiplicative_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "-"); }
    ;

shift_expression
    : additive_expression { $$ = $1; }
    | shift_expression LEFT_OP additive_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "<<"); }
    | shift_expression RIGHT_OP additive_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), ">>"); }
    ;

relational_expression
    : shift_expression { $$ = $1; }
    | relational_expression '<' shift_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "<"); }
    | relational_expression '>' shift_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), ">"); }
    | relational_expression LE_OP shift_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "<="); }
    | relational_expression GE_OP shift_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), ">="); }
    ;

equality_expression
    : relational_expression { $$ = $1; }
    | equality_expression EQ_OP relational_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "=="); }
    | equality_expression NE_OP relational_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "!="); }
    ;

and_expression
    : equality_expression { $$ = $1; }
    | and_expression '&' equality_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "&"); }
    ;

exclusive_or_expression
    : and_expression { $$ = $1; }
    | exclusive_or_expression '^' and_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "^"); }
    ;

inclusive_or_expression
    : exclusive_or_expression { $$ = $1; }
    | inclusive_or_expression '|' exclusive_or_expression { $$ = new BinaryOp(NodePtr($1), NodePtr($3), "|"); }
    ;

logical_and_expression
    : inclusive_or_expression { $$ = $1; }
    | logical_and_expression AND_OP inclusive_or_expression { $$ = new LogicalOp(NodePtr($1), NodePtr($3), "&&"); }
    ;

logical_or_expression
    : logical_and_expression { $$ = $1; }
    | logical_or_expression OR_OP logical_and_expression { $$ = new LogicalOp(NodePtr($1), NodePtr($3), "||"); }
    ;

conditional_expression
    : logical_or_expression { $$ = $1; }
    ;

assignment_expression
    : conditional_expression { $$ = $1; }
    | unary_expression '=' assignment_expression { $$ = new Assign(NodePtr($1), NodePtr($3)); }
    | unary_expression ADD_ASSIGN assignment_expression { $$ = new CompoundAssign(NodePtr($1), NodePtr($3), "+="); }
    | unary_expression SUB_ASSIGN assignment_expression { $$ = new CompoundAssign(NodePtr($1), NodePtr($3), "-="); }
    | unary_expression MUL_ASSIGN assignment_expression { $$ = new CompoundAssign(NodePtr($1), NodePtr($3), "*="); }
    | unary_expression DIV_ASSIGN assignment_expression { $$ = new CompoundAssign(NodePtr($1), NodePtr($3), "/="); }
    | unary_expression MOD_ASSIGN assignment_expression { $$ = new CompoundAssign(NodePtr($1), NodePtr($3), "%="); }
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
std::set<std::string> g_typedef_names;
std::map<std::string, TypeSpecifier> g_typedef_types;

NodePtr ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if (yyin == nullptr) {
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    std::exit(1);
  }

  g_root = nullptr;
  g_typedef_names.clear();
  g_typedef_types.clear();
  yyparse();

  fclose(yyin);
  yylex_destroy();

  return NodePtr(g_root);
}
