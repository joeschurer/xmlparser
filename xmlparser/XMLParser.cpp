// Project 3 -- XML Parsing Project

/** XML parsing class implementation.
    @file XMLParser.cpp */

#include <string>
#include <assert.h>
#include "XMLParser.hpp"

using namespace std;

//check provided conditiions for tag names
bool XMLParser::checkTagName(string entry)
{
	//list of prohibited characters
	string prohibited = " !\"#$%&'()*+,/;<=>?@[\\]^`{|}~";
	
	//verify that string does not begin with -,., or numeric
	if((entry[0] == '.') || (entry[0] == '-')) { return false;}
	if(entry[0] < 58 && entry[0] > 47) { return false;}
	
	//verify that no prohibited characters are present if is start 
	for(size_t i=0;i<entry.size();i++){
		if(prohibited.find(entry[i]) != std::string::npos){
			return false;
		}
	}
		
	return true;
}  

//fuction to trim whitespace from a string passed as reference
void XMLParser::trim(string & entry){
	//string leading and trailing whitespace
	int posStart = entry.find_first_not_of(" \n\t");
	int posEnd = entry.find_last_not_of(" \n\t");
	entry = entry.substr(posStart,posEnd-posStart+1);
	
}


//Construtor
XMLParser::XMLParser()
{
	//dynamically allocate the Bag and stack
	elementNameBag = new Bag<std::string>;
	parseStack = new Stack<std::string>;
}  // end default constructor

//Destructor
XMLParser::~XMLParser()
{
	//free the allocated memory
	delete elementNameBag;
	elementNameBag = nullptr;
	delete parseStack;
	parseStack = nullptr;
}  // end destructor

/* Implementation of tokenizeInputString
 * Assumes that the user may use any number of declarations 
 * anywhere. Also assumes that extra whitespace at the start or the end
 * of any tag (including declarations) will be treated as an invalid tag
 * name.
 * Essentially slices the string into chunks and analyzes each one
 * individually. Will call trim to remove whitespace from the string and
 * use checkTagName to look for any invalid characters in the tag.
 */
bool XMLParser::tokenizeInputString(const std::string &inputString)
{	
	//setup variables to parse the string
	std::string copy = inputString;
	std::string temp;
	int posStart,posEnd;
	
	//remove any leading and trailing whitespace
	trim(copy);
	
	
	//verify that the first character is a <
	if(copy[0] != '<') { return false;}
	
	while(copy.size() != 0){
		//trim leading and trailing whitespace
		trim(copy);
	
		//Item is content
		if(copy[0] != '<') {
			//extract content
			posStart = copy.find('<');
			if(posStart == std::string::npos){
				clear();
				return false;
			}
			temp = copy.substr(0,posStart);
			
			//generate new token
			TokenStruct newToken;
			newToken.tokenType = CONTENT;
			newToken.tokenString = temp;
			tokenizedInputVector.push_back(newToken);
			
			//remove content from string
			copy = copy.substr(posStart,std::string::npos);		
			
		}
		
		//else item must be a tag
		else{
			posStart = copy.find('<');
			posEnd = copy.find('>');
			
			//get what is in between the tags
			//temp contains string of whatever is between <  and >
			temp = copy.substr(posStart+1,posEnd-1);
			std::string toCheck;
			toCheck.push_back(temp[0]);
			
			//identify the type of tag and take proper action
			//check for a declaration
			if(temp[0] == '?'){
				posStart = copy.find('<');
				posEnd = copy.find('>');
				temp = copy.substr(posStart+1,posEnd-posStart-1);
				
				//temp string to check for whitespace
				std::string toCheck = temp.substr(1,temp.size()-2);
				std::string checkTrim = toCheck;
				trim(checkTrim);
			
				//if it is not a valid declaration exit
				if(temp.back() != '?'){
					return false;
				}
				//check if there is leading or trailing spaces
				else if(toCheck.size() != checkTrim.size()){
					tokenizedInputVector.clear();
					elementNameBag->clear();
					parseStack->clear();
					return false;
				}
				else{
					//otherwise generate token and place it onto the stack
					TokenStruct newToken;
					newToken.tokenType = DECLARATION;
					newToken.tokenString = temp.substr(1,temp.size()-2);
					tokenizedInputVector.push_back(newToken);
				
					//parseStack->push(newToken);		
				
					//remove the declaration from the string
					copy = copy.substr(posEnd+1,std::string::npos);
					posStart = copy.find_first_not_of(" \n");
					copy = copy.substr(posStart,string::npos);
				}
			}
			
			else if(temp[0] == '/'){//tag is an end tag
				temp = temp.substr(1,std::string::npos);
				
				//if the tag name is invalid return fals
				if(checkTagName(temp) == 0) {
					tokenizedInputVector.clear();
					elementNameBag->clear();
					parseStack->clear();
					return false;
				}
				
				//generate new token and place in stack
				TokenStruct newToken;
				newToken.tokenType = END_TAG;
				newToken.tokenString = temp;
				tokenizedInputVector.push_back(newToken);
				
				//remove the tag from the string
				copy = copy.substr(posEnd+1,std::string::npos);
				
			}
			else if(temp.back()== '/'){ //item is an empty tag
				//remove the end /
				size_t pos = temp.find('/');
				std::string ncopy = temp;
				ncopy.pop_back();
				
				//empty tag may have attributes so look for them
				pos= ncopy.find_first_of(" \n\t");
				if(pos == std::string::npos){//no attributes
					//if the tag name is invalid return false
					if(checkTagName(ncopy) == 0) {
						tokenizedInputVector.clear();
						elementNameBag->clear();
						parseStack->clear();
						return false;
					}
					//generate new token and place in stack
					TokenStruct newToken;
					newToken.tokenType = EMPTY_TAG;
					newToken.tokenString = ncopy;
					tokenizedInputVector.push_back(newToken);
				
					//place tag name in bag 
					//elementNameBag->add(ncopy);
				
				
					//remove the tag from the string
					copy = copy.substr(posEnd+1,std::string::npos);
				}
				
				else{//there may be an attribute
					
					//check if there is leading or trailing spaces
					std::string checkTrim = ncopy;
					trim(checkTrim);
					if(ncopy.size() != checkTrim.size()){
						tokenizedInputVector.clear();
						elementNameBag->clear();
						parseStack->clear();
						return false;
					}
					
					
					//extract and validate tag
					string tag =temp.substr(0,pos);
					string attribute = temp.substr(pos+1,string::npos);
					
					//if the tag name is invalid return fals
					if(checkTagName(tag) == 0) {
						tokenizedInputVector.clear();
						elementNameBag->clear();
						parseStack->clear();
						return false;
					}
				
					//generate new token and place in stack
					TokenStruct newToken;
					newToken.tokenType = EMPTY_TAG;
					newToken.tokenString = tag;
					tokenizedInputVector.push_back(newToken);
				
					//place tag name in bag 
					//elementNameBag->add(tag);
				
				
					//remove the tag from the string
					copy = copy.substr(posEnd+1,std::string::npos);
				}
			}
			
			else if(checkTagName(toCheck)){//item is a start tag
				//cout << temp << endl;
				//empty tag may have attributes so look for them
				size_t pos = temp.find_first_of(" \n");
				if(pos == std::string::npos){//no attributes
					//if the tag name is invalid return fals
					if(checkTagName(temp) == 0) {
						tokenizedInputVector.clear();
						elementNameBag->clear();
						parseStack->clear();
						return false;
					}
				
					//generate new token and place in stack
					TokenStruct newToken;
					newToken.tokenType = START_TAG;
					newToken.tokenString = temp;
					tokenizedInputVector.push_back(newToken);		
				
					//place tag name in bag 
					//elementNameBag->add(temp);
				
				
					//remove the tag from the string
					copy = copy.substr(posEnd+1,std::string::npos);
				}
				
				else{//there may be an attribute
					//check if there is leading or trailing spaces
					std::string checkTrim = temp;
					trim(checkTrim);
					if(temp.size() != checkTrim.size()){
						tokenizedInputVector.clear();
						elementNameBag->clear();
						parseStack->clear();
						return false;
					}
					
					//extract and validate tag
					string tag =temp.substr(0,pos);
					string attribute = temp.substr(pos+1,string::npos);
					
					//if the tag name is invalid return fals
					if(checkTagName(tag) == 0) {
						tokenizedInputVector.clear();
						elementNameBag->clear();
						parseStack->clear();
						return false;
					}
				
					//generate new token and place in stack
					TokenStruct newToken;
					newToken.tokenType = START_TAG;
					newToken.tokenString = tag;
					tokenizedInputVector.push_back(newToken);	
					
					//place tag name in bag 
					//elementNameBag->add(tag);
				
				
					//remove the tag from the string
					copy = copy.substr(posEnd+1,std::string::npos);
				}
			}
			
			//invalid character
			else{
				tokenizedInputVector.clear();
				elementNameBag->clear();
				parseStack->clear();
				return false;
			}
		}		
	}
	
	return true;
}  // end

// TODO: Implement a helper function to delete attributes from a START_TAG
// or EMPTY_TAG string (you can change this...)
static std::string deleteAttributes(std::string input)
{
	return input;
}

/* Function to parse the TokenizedInput
 * Will update the stack and bag of element names
 * Assume that content is acceptable anywhere, as long as it is after
 * the first tag and before the last tag.
 * Also assuming that an empty tag can occur anywhere
 */
bool XMLParser::parseTokenizedInput()
{
	//imeadiatley fail the test if the vector is empty
	if(tokenizedInputVector.size() == 0){
		clear();
		return false;
	}
	 
	 int startTag=0;
	//loop thru token vector and will push open tags onto stack
	for(int i=0;i<tokenizedInputVector.size();i++){
		//if there is an empty tag the stack must not be empty
		if(tokenizedInputVector[i].tokenType == EMPTY_TAG){
			//if the stack is empty we cannot have an empty tag
			if(parseStack->isEmpty()){
				clear();
				return false;
			}
			
			//place empty tag in bag
			elementNameBag->add(tokenizedInputVector[i].tokenString);
		}
		
		//if there is a declaration just check it is before start tag
		if(tokenizedInputVector[i].tokenType == DECLARATION){
			//if the stack is empty the first tag MUST be the rootTag
			if(startTag==1){
				clear();
				return false;
			}
		}
		
		//if the token is a content tag
		//essentially ignore unless the stack is empty
		if(tokenizedInputVector[i].tokenType == CONTENT){
			/* If the stack is empty that means that this item of
			 * content is occuring outside of any tags
			 * i.e. before the first tag or after the last one
			 */
			if(parseStack->isEmpty()) {
				clear();
				return false;
			}
		}
		
		//if the token is a start tag push it onto the stack
		if(tokenizedInputVector[i].tokenType == START_TAG){
			//if the stack is empty the first tag MUST be the rootTag
			if(parseStack->isEmpty() && startTag==1){
				clear();
				return false;
			}
			
			//else it is a nested start tag so just push it on there
			parseStack->push(tokenizedInputVector[i].tokenString);
			startTag=1;
			
			
			//place start tag in bag
			elementNameBag->add(tokenizedInputVector[i].tokenString);
		}
		
		//if the token is an end tag
		if(tokenizedInputVector[i].tokenType == END_TAG){
			//if the stack is empty there is an unmatached end tag
			if(parseStack->isEmpty()){
				clear();
				return false;
			}
			
			//it must be the end tag for the start tag at top of stack
			if(tokenizedInputVector[i].tokenString != parseStack->peek()){
				clear();
				return false;
			}

			//otherwise pop the matched start tag off the parseStack
			parseStack->pop();
			
		}
		
		//if not at the start or end of vector verify that the stack is not empty
	}
	
	//if stack not empty fail
	if(parseStack->isEmpty()==0){
		clear();
		return false;
	}
	
	return true;
}

//Clear method resets internal data strcutrues
void XMLParser::clear()
{
	//free the allocated memory
	delete elementNameBag;
	delete parseStack;
	
	//dynamically allocate the Bag and stack
	elementNameBag = new Bag<std::string>;
	parseStack = new Stack<string>;
	
	//reset the internal vector
	tokenizedInputVector.clear();
}

/*returns the value of the internal tokenizedInputVector
 * this not being empty has not meaning as to whether the XML is
 * actually correct, only that the tokenizeInputString function was
 * successful.
 */
vector<TokenStruct> XMLParser::returnTokenizedInput() const
{
	return tokenizedInputVector;
}

//containsElementName method
bool XMLParser::containsElementName(const std::string &inputString) const
{
	//run the parse function
	//parseTokenizedInput();
	
	//check that the token vector is not empty
	if(elementNameBag->isEmpty()){
		return false;
	}
	
	//return true if the bag contins a specific element name
	if(elementNameBag->contains(inputString)==1){	return true;}
	
	return false;
}

//frequencyElementName method
int XMLParser::frequencyElementName(const std::string &inputString) const
{
	//run the parse function 
	//parseTokenizedInput();
	
	//re tokenize and parse the string and verify they both returned true
	//bool test = parseTokenizedInput();
	if(elementNameBag->isEmpty()){
		return -1;
	}
	
	//return the frequency of the inputString in the bag
	return elementNameBag->getFrequencyOf(inputString);
}

