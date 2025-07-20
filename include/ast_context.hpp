#pragma once

namespace ast {
// An object of class Context is passed between ast nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).
class Context
{
    UNSIGNED_CHAR,
    UNSIGNED_SHORT,
    UNSIGNED_LONG,
    UNSIGNED_INT,
    LONG_DOUBLE,
    CHAR,
    FLOAT,
    DOUBLE,
    INT,
    VOID,
    LONG,
    STRUCT,
    SHORT,
};

} // namespace ast
