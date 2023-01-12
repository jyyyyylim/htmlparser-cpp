#include <iostream>
#include <map>
#include <string>
#include <vector>
using std::string, std::vector, std::map;


//demands:
//CRUD on dom elements, both tag and content
//might also support arg 

//2 approaches: LL, pointer array
//LL: no random reads, constant time reordering
//arr: constant time access, shit reordering time



//define domtree mechanism
//domNode, doubly linked for reverse traversal
struct domNode {
    domNode*                parentNode;
    string                  tag;                    //html 標名
    //int                     occurance;              //debug
    string                  contents;         
    vector<domNode*>        children;               //vector to acommodate arbitrary amount of children
    map<string, string>     tag_attributes;         //map of tag attributes ("class", etc)
};

//might get thrown out later
struct embeddedJS {
    domNode*    parentNode;
    string      plaintextScript;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                                  DEFINE PRIMITIVES
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

domNode* init();

domNode* setTagContent(domNode* parent, string tagContent);
domNode* setTagAttributes(domNode* parent, string key);
domNode* setTagAttributes(domNode* parent, string key, string value);
domNode* pushOffspring(domNode* parent, string tagName);


domNode* ascend(domNode* leaf);




//TRAVERSAL FUNCTIONS

void tree(domNode* root);
void depthTree(domNode* root, int searchDepth);

void searchByTag(string tagName);
void searchByClass(string tagClass);