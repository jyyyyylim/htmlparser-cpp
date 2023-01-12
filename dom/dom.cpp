#include "dom.hpp"

//arbitrarily creates a new node
//only used to initialize the "head" of a document object
domNode* init(){
    domNode *root= new domNode;
    //push(*root, );
    return root;
};


//sets the content of the given tag
domNode* setTagContent(domNode* target, string tagContent){
    //if (!isspace(tagContent)){}
    target->contents += tagContent;
    return target;
};


//set the map representing the attributes of the tag, without the value
domNode* setTagAttributes(domNode* target, string key){  
    target->tag_attributes[key];
    return target;
};

//overload: set the map representing the tag attributes
domNode* setTagAttributes(domNode* target, string key, string value){
    target->tag_attributes[key]= value;
    return target;
};


//adds a "leaf" to the given node
domNode* pushOffspring(domNode* parent, string tagName){

    domNode* childNode= new domNode;
    parent->children.push_back(childNode);    
    
    childNode->parentNode=  parent;
    childNode->tag=         tagName;
    //std::cout << "created leaf with tag name: " << tagName << "\n";
    return childNode;
};



//returns the ptr of the parent node
domNode* ascend(domNode* subject){
    if (subject->parentNode != NULL){
        return subject->parentNode;
    } else {
        return subject;
    }
};




void tree(domNode* root){


};


void depthTree(domNode* root, int searchDepth){


};



void searchByTag(string tagName){};
void searchByClass(string tagClass){};