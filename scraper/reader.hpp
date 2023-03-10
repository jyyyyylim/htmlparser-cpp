#ifndef reader
#define reader

#include <string>
#include <vector>

//#include "../dom/node.hpp"
#include "../dom/dom.hpp"

using std::string, std::vector;






//init: start state
//opening: bracket portions
//contents
//closing: bracket portions of endtags
//discard: read skip entire tag- inclusion inconsequential to the final structure


//all possible states for the state machine read
enum state {STATE_ctnt, STATE_discard, STATE_within_tag, STATE_tag_attribute_key, STATE_tag_attribute_value};

typedef struct {
    string                  tag;                    //html 標名
    vector<string, string>  tag_attributes;         //map of tag attributes ("class", etc)
    string                  contents;         
} currentNode;

typedef struct {
    domNode*                documentRoot;
    domNode*                terminatingContext;
} readReturn;


//std::ifstream& openHandle(string inputFile);
//readReturn read(std::ifstream& handle);
readReturn read(string inputFile);

#endif