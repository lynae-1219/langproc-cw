// ast.hpp
#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "ast_node.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"
#include "ast_declaration.hpp"
#include "ast_constant.hpp"
#include "ast_float_constant.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_function_definition.hpp"
#include "ast_init_declarator.hpp"
#include "ast_jump_statement.hpp"
#include "ast_type_specifier.hpp"
#include "ast_assign.hpp"
#include "ast_compound_statement.hpp"
#include "ast_binary_op.hpp"
#include "ast_logical_op.hpp"
#include "ast_function_call.hpp"
#include "ast_while_statement.hpp"

ast::NodePtr ParseAST(std::string file_name);