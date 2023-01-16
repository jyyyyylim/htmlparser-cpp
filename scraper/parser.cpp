#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "../dom/dom.hpp"
#include "./reader.hpp"

using std::string;

//demands:
//CRUD on dom elements, both tag and content
//might also support arg 

//plan: selective parsing 
//options: 
//search by tag
//ruleset identical to emmet expressions


//2 approaches: LL, pointer array


//initialize file handle
std::ifstream& openHandle(string inputFile){
    std::ifstream parsedFile (inputFile);
    if (!parsedFile.is_open()){
        printf("file located at %s does not exist", inputFile);
        //return nullptr;
    }
    return parsedFile;
};



//initialize buffer strings, globally scoped
string charBuffer, attrBuffer, temp;

void clrCharBuffer(){charBuffer = "";}
void clrAttrBuffer(){attrBuffer = "";}
void clrTemp(){temp = "";}



bool in_array(const string value, const vector<string> array){
    return std::find(array.begin(), array.end(), value) != array.end();
}


vector<string> tag_selfClosing = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr"};
vector<string> tag_ignore = {"script", "css", "style", "svg", "path"};
vector<string> boolattrs = {"allowfullscreen", "async", "autofocus", "autoplay", "checked", "controls", "default", "defer", "disabled", "formnovalidate", "inert", "ismap", "itemscope", "loop", "multiple", "muted", "nomodule", "novalidate", "open", "playsinline", "readonly", "required", "reversed", "selected"};

//function used in conjunction with openHandle(), addr to ifstream object is used
readReturn read(std::ifstream& handle){
    readReturn returnParams;

    state parserState = STATE_ctnt;
    string ln, endtag;

    //initialize parser flags
    bool rootPopulated, utfBOM, charIsEnclosed, tagSelfCloses, unsanitizedIgnore, readcursorEnclosed, isBoolAttr;
    rootPopulated = utfBOM = charIsEnclosed = tagSelfCloses = unsanitizedIgnore = readcursorEnclosed = isBoolAttr = false;

    //create the root node
    domNode* rootNode = new domNode;
    rootNode->parentNode = nullptr;
    
    domNode* currentContext = rootNode;

    while(getline(handle, ln)){

        for (char c : ln){
            //ignore odd bytes on BOM encoded files
            if (static_cast<int>(c) < 1){continue;}

            //discard state: skips checks regardless
            if (parserState==STATE_discard){

                //issue: without reader "foresight" any closing brackets including the embeds count 
                //hence start reading from the occurance of a tag, building a string
                //on detecting termination of the tag, compare it to the opening

                //drawbacks of this approach: an matching closing tag to the current context will return a false positive
                //no other way im aware of to approach this though
                if (unsanitizedIgnore){

                    if (c=='<'){
                        clrTemp();
                        if (readcursorEnclosed){
                            temp+=c; continue;
                        } else {
                            readcursorEnclosed = true;
                            temp+=c; continue;
                        }
                        
                    } else {
                        temp+=c;

                        //define a "timeout" to prevent indefinite waiting for a closing tag
                        if (temp.length()==endtag.length()){
                            if (temp!=endtag){
                                clrTemp();
                                readcursorEnclosed = false;
                                continue;
                            } else if (c=='>' && temp==endtag){
                                
                                clrTemp();

                                currentContext = ascend(currentContext);
                                unsanitizedIgnore = readcursorEnclosed = false;
                                parserState = STATE_ctnt;

                                continue;                                
                            }
                        }
                    } 

                } else {
                    
                    //assume the closing angle found implies the end of the read ignore
                    if (c=='>' && !readcursorEnclosed){
                        charIsEnclosed = false;
                        parserState = STATE_ctnt;
                        //std::cout << "state neutralized\n";
                        continue;
                    } else {
                        continue;
                    }

                }
            } 


            //character is free-floating, only check for new tags and content
            //check context for ignores
            if (parserState==STATE_ctnt){

                if (c == '<'){
                    //if "cursor" hits opening tag

                    if (!empty(charBuffer)){
                        setTagContent(currentContext, charBuffer);
                    }
                    
                    
                    clrCharBuffer();
                    
                    parserState = STATE_within_tag;
                    //std::cout << "entered into a tag\n";
                    continue;

                } else {
                    //all other scenarios, record the content as-is
                    charBuffer+=c;
                    continue;
                
                }
                
            
            } else if (parserState==STATE_within_tag){
                //character is not free floating, do the in-tag routines instead
                //attr parsing, check opening tag termination

                //termination checks
                if (c=='>'){

                    //tag is "self closing"
                    if (in_array(charBuffer, tag_selfClosing)){    
                        parserState = STATE_ctnt;
                        continue;

                    //as the only aim is to show the existence of ignored tags yet ignore its contents
                    //hand over to the discard routine
                    } else if (in_array(charBuffer, tag_ignore)){
                        
                        currentContext = pushOffspring(currentContext, charBuffer);
                        //currentContext->occurance = lnnr;

                        endtag = "</" + charBuffer + ">"; 
                        clrCharBuffer();


                        unsanitizedIgnore = true;
                        parserState = STATE_discard;
                        
                        continue;
                    }
                    
                    if (!rootPopulated){
                        currentContext->tag = charBuffer;
                        clrCharBuffer();
                        rootPopulated = true;
                        continue;
                    } else {
                        currentContext = pushOffspring(currentContext, charBuffer);
                        //currentContext->occurance = lnnr;

                        clrCharBuffer();
                        parserState = STATE_ctnt;
                        continue;
                    }
                
                } else if (c==' '){
                //even if it was an arbitrary space i can only assume it as an implied key
                //its possible to cancel it out later anyway

                    if (!rootPopulated){
                        currentContext->tag=charBuffer;
                        rootPopulated = true;
                    } else {
                        currentContext = pushOffspring(currentContext, charBuffer);
                        //currentContext->occurance = lnnr;
                    }
                    
                    //tag is "self closing"
                    if (in_array(charBuffer, tag_selfClosing)){    
                        tagSelfCloses = true;
                    } 
                
                    clrCharBuffer();

                    parserState = STATE_tag_attribute_key;
                    continue;

                } else if (c=='/'){
                    //closing tag found, ascend to parent of Context
                    parserState = STATE_discard;
                    currentContext = ascend(currentContext);
                    clrCharBuffer();
                    continue;

                } else if (c=='!'){
                    //ignores comments as well as doctype decl
                    parserState = STATE_discard;
                    continue;

                } else {
                    charBuffer+=c;
                    continue;
                } 
                
                
                
            //character is presumed flagged as an attr key
            } else if (parserState==STATE_tag_attribute_key){

                if (c=='>'){
                    //tag closes, so cancel the key read
                    parserState = STATE_ctnt;
                    if (tagSelfCloses){
                        currentContext = ascend(currentContext);
                        tagSelfCloses = false;
                    } 
                    
                    if (in_array(currentContext->tag, tag_ignore)){
                        
                        endtag = "</" + currentContext->tag + ">"; 
                        unsanitizedIgnore = true;
                        parserState = STATE_discard;
                        
                        continue;
                    }


                    continue;

                } else if (c==' '){
                    
                    if (in_array(attrBuffer, boolattrs)){
                        isBoolAttr = true;
                        parserState = STATE_tag_attribute_value;
                        continue;
                    }
                    
                    

                    continue;

                } else if (c=='='){
                    if (in_array(attrBuffer, boolattrs)){isBoolAttr = true;}
                    parserState = STATE_tag_attribute_value;
                    continue;

                } else if (c=='/'){
                    continue;

                } else {
                    attrBuffer+=c;
                    continue;
                }

            //character is flagged as an attr value
            } else if (parserState==STATE_tag_attribute_value){

                if (!isBoolAttr){
                    if (!charIsEnclosed && (c=='"' || c=='\'')){
                        //toggle the charIsEnclosed flag on a quote that indicates an attr value
                        charIsEnclosed = true;
                        continue;   
                    } else {
                        
                        if (c=='"' || c=='\''){
                            charIsEnclosed = false;
                            parserState = STATE_tag_attribute_key;
                            setTagAttributes(currentContext, attrBuffer, charBuffer);

                            clrAttrBuffer();
                            clrCharBuffer();
                            
                            continue;
                        } else {
                            charBuffer+=c;
                            continue;
                        }
                    }
                } else {
                    //bool attr is passed in by itself, implying an unambigous TRUE
                    if (!charIsEnclosed && c!=' '){
                        parserState = STATE_tag_attribute_key;
                        setTagAttributes(currentContext, attrBuffer, "true");
                        isBoolAttr = false;
                        clrAttrBuffer();
                        attrBuffer+=c;
                        clrCharBuffer();
                        continue;
                    }
                }
                

                
                continue;



            }

            //charBuffer+=c;
            //continue;
        }

    }
    //std::cout << "done";


    returnParams.documentRoot= rootNode;
    returnParams.terminatingContext= currentContext;

    return returnParams;
};

