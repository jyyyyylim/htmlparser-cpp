#ifndef ast

#define ast
//need to create a structure that can be fed to a traversal algorithm in some way
#include <map>
#include <string>
#include <vector>

using std::string, std::vector;

//plan: defined custom types for the various select operations
//code distinguishes them at runtime by either overloads or templating


//typedef vector<domNode*> PARSER_CHILDREN;


//input command interpretation: operation prio is.. generally... LTR
//tokens: >, +, ^, (), *
enum operation {child, sibling, ascension, group, mul};





//generalizations:
//1 type of node, many configurations
//the general used one is operation-argument-argument
//an "argument" is either just a string input or a pointer to another node

//a "selector" node never has offspring and is always rooted to an "operation" node
//an "operation" node has no limits to its placement except it cannot be rooted to a "selector" node


typedef string SELECTION_TOKEN;
typedef string SELECTION_OPERATOR;


typedef union {
    SELECTION_TOKEN token;
    syntaxNode      nestedOp;
} PARAMETER_GENERIC;

typedef struct {
    syntaxNode*     left;
    syntaxNode*     right;
    SELECTION_OPERATOR  operation;
    PARAMETER_GENERIC   data[2];
} syntaxNode;






typedef struct {string args[2];} BINARY_OPERATION;




void parseRule(string ruleInput);


#endif
