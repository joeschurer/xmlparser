#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include <iostream>
#include "catch.hpp"
#include "XMLParser.cpp"

/* Joseph Schurer
 * Tests of stack and XMLParser Class
 */

using namespace std;

//Provided tests

TEST_CASE( "Test Bag add", "[XMLParser]" )
{
	   INFO("Hint: testing Bag add()");
		// Create a Bag to hold ints
		Bag<int> intBag;
		int testSize = 2;
		int bagSize;
		bool success;
		for (int i=0; i<testSize; i++) {
			success = intBag.add(i);
			REQUIRE(success);
			bagSize = intBag.size();
			success = (bagSize == (i+1));
			REQUIRE(success);
		}
}

TEST_CASE( "Test Stack push", "[XMLParser]" )
{
	   INFO("Hint: testing Stack push()");
		// Create a Stack to hold ints
		Stack<int> intStack;
		int testSize = 3;
		int stackSize;
		bool success;
		for (int i=0; i<testSize; i++) {
			success = intStack.push(i);
			REQUIRE(success);
			stackSize = intStack.size();
			success = (stackSize == (i+1));
			REQUIRE(success);
		}
}

TEST_CASE( "Test XMLParser tokenizeInputString", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test>stuff</test>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
}

//tests from the "XMLParser_more_test.xpp" file
TEST_CASE( "Test Stack handout-0", "[XMLParser]" )
{
		// Create a Stack to hold ints
		Stack<int> intStack;
		int testSize = 10;
		int stackSize;
		bool success = false;
		for (int i=0; i<testSize; i++) {
			intStack.push(i);
			stackSize = intStack.size();
			success = (stackSize == (i+1));
			REQUIRE(success);
		}
		for (int i = testSize-1; i>=0; i--) {
			REQUIRE(intStack.isEmpty() == false);
            int elem = intStack.peek();
			REQUIRE(elem == i);
            REQUIRE(intStack.isEmpty() == false);
			intStack.pop();
		}
        REQUIRE(intStack.isEmpty() == true);
}

TEST_CASE( "Test Stack handout-1", "[XMLParser]" )
{
	   Stack<char> charStack;
       std::string s("Hello world");
       int i = 1;
       for (auto c:s) {
           charStack.push(c);
           REQUIRE(charStack.size() == i);
           i++;
       }
       for (i = 0; i < 5; i++) {
           REQUIRE(charStack.isEmpty() == false);
           char c = charStack.peek();
           REQUIRE(c == s[s.length()-1-i]);
           REQUIRE(charStack.isEmpty() == false);
           charStack.pop();
       }
       REQUIRE(charStack.size() == 6);
       charStack.clear();
       REQUIRE(charStack.isEmpty() == true);      
}

// You can assume that the beginning and the end of CONTENT will not be filled with whitespace
TEST_CASE( "Test XMLParser tokenizeInputString Handout-0", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test>stuff</test>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		std::vector<TokenStruct> result = {TokenStruct{StringTokenType::START_TAG, std::string("test")},
											TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
											TokenStruct{StringTokenType::END_TAG, std::string("test")}};
		std::vector<TokenStruct> output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
		}
}

// You can assume that the beginning and the end of CONTENT will not be filled with whitespace
TEST_CASE( "Test XMLParser tokenizeInputString Handout-1", "[XMLParser]" )
{
	   INFO("Hint: tokenize multiple elements test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Note src='gmail'>  <From>Tom</From> <To>Alice</To> </Note>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		std::vector<TokenStruct> result = { TokenStruct{StringTokenType::DECLARATION, std::string("xml version=\"1.0\" encoding=\"UTF-8\"")},
									TokenStruct{StringTokenType::START_TAG, std::string("Note")},
									TokenStruct{StringTokenType::START_TAG, std::string("From")},
									TokenStruct{StringTokenType::CONTENT, std::string("Tom")},
									TokenStruct{StringTokenType::END_TAG, std::string("From")},
									TokenStruct{StringTokenType::START_TAG, std::string("To")},
									TokenStruct{StringTokenType::CONTENT, std::string("Alice")},
									TokenStruct{StringTokenType::END_TAG, std::string("To")},
									TokenStruct{StringTokenType::END_TAG, std::string("Note")}};
		std::vector<TokenStruct> output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
		}
}

TEST_CASE( "Test XMLParser parseTokenizedInput Handout-0", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test myattr='abcdef'>stuff<this_is_empty_tag/></test>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		std::vector<TokenStruct> result = {TokenStruct{StringTokenType::START_TAG, std::string("test")},
											TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
											TokenStruct{StringTokenType::EMPTY_TAG, std::string("this_is_empty_tag")},
											TokenStruct{StringTokenType::END_TAG, std::string("test")}};
		std::vector<TokenStruct> output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		success = myXMLParser.parseTokenizedInput();
		REQUIRE(success);
		output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
		}
}

TEST_CASE( "Test XMLParser Final Handout-0", "[XMLParser]" )
{
	   INFO("Hint: Product");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		ifstream myfile ("./test2.txt");
		std::string inputString((std::istreambuf_iterator<char>(myfile) ), (std::istreambuf_iterator<char>()) );

		bool success;
		success = myXMLParser.tokenizeInputString(inputString);
		REQUIRE(success);
		success = myXMLParser.parseTokenizedInput();
		REQUIRE(success);
		REQUIRE(myXMLParser.containsElementName("catalog"));
		REQUIRE(myXMLParser.frequencyElementName("catalog") == 1);
		REQUIRE(myXMLParser.containsElementName("product"));
		REQUIRE(myXMLParser.frequencyElementName("product") == 1);
		REQUIRE(myXMLParser.containsElementName("catalog_item"));
		REQUIRE(myXMLParser.frequencyElementName("catalog_item") == 2);
		REQUIRE(myXMLParser.containsElementName("item_number"));
		REQUIRE(myXMLParser.frequencyElementName("item_number") == 2);
		REQUIRE(myXMLParser.containsElementName("size"));
		REQUIRE(myXMLParser.frequencyElementName("size") == 6);
		REQUIRE(myXMLParser.containsElementName("color_swatch"));
		REQUIRE(myXMLParser.frequencyElementName("color_swatch") == 15);
}

//My implemented tests

//Tests for the stack class
//test stack default construtor
TEST_CASE("Stack - Default constructor", "[XMLParser]"){
	//init stack
	Stack<int> a;
	
	//stack must be empty
	REQUIRE(a.size()==0);
	REQUIRE(a.isEmpty());
}

//test pushing item onto stack
TEST_CASE("Stack - Push item", "[XMLParser]"){
	//init stack
	Stack<int> a;
	
	//push item onto the stack
	REQUIRE(a.push(1));
	REQUIRE(a.size()==1);
	REQUIRE(a.push(2));
	
	//stack must be empty
	REQUIRE(a.size()==2);
	REQUIRE(!a.isEmpty());
}

//test popping item off stack
TEST_CASE("Stack - Pop item", "[XMLParser]"){
	//init stack
	Stack<int> a;
	
	//push item onto the stack
	REQUIRE(a.push(1));
	REQUIRE(a.push(2));
	
	//pop and recheck stack
	REQUIRE(a.pop());
	REQUIRE(a.size()==1);
	REQUIRE(a.peek() == 1);
}

//test the size and isEmpty
TEST_CASE("Stack - size and isEmpty", "[XMLParser]"){
	//init stack
	Stack<int> a;
	//verify the stack is empty
	REQUIRE(a.isEmpty());
	REQUIRE(a.size()==0);
	
	//push item onto the stack
	REQUIRE(a.push(1));
	REQUIRE(a.push(2));
	
	//verify that size and isEmpty updated properly
	REQUIRE(a.size()==2);
	REQUIRE(!a.isEmpty());
	
	//stack must be empty
	REQUIRE(a.pop());
	REQUIRE(a.size()==1);
	REQUIRE(!a.isEmpty());
	
	//stack must be empty
	REQUIRE(a.pop());
	REQUIRE(a.size()==0);
	REQUIRE(a.isEmpty());
}

//test peeking the stack
TEST_CASE("Stack - peek stack", "[XMLParser]"){
	//init stack
	Stack<int> a;
	
	//test peeking empty stack
	int i=0;
	try{
		a.peek();
	} catch(logic_error){
		i++;
	}
	
	//make sure that exception occured
	REQUIRE(i==1);
	
	//push an item onto the stack and then peek it
	a.push(1);
	REQUIRE(a.peek()==1);
}

//test clearing the stack
TEST_CASE("Stack - clear stack", "[XMLParser]"){
	//init stack
	Stack<int> a;
	
	//put stuff in stack
	for(int i=0;i<50;i++){
		REQUIRE(a.push(i));
	}
	
	//check that the size is correct
	REQUIRE(a.size()==50);
	
	//clear and then check that the stack is empty
	a.clear();
	REQUIRE(a.size()==0);
	REQUIRE(a.isEmpty());
	
}

//Tests for the XMLParser
//test the tokenizing function
TEST_CASE("XMLParser - tokenize", "[XMLParser]"){
	XMLParser a;
	string testString = "<test>stuff</test>";
	bool success;
	success = a.tokenizeInputString(testString);
	REQUIRE(success);
	
	a.clear();
	//all below are invalid
	string fail1 = "< test>stuff</test>";
	string fail2 = "< test >stuff</test>";
	string fail3 = "<test >stuff</test>";
	string fail4 = "<test>stuff< /test>";
	string fail5 = "<test>stuff</ test>";
	string fail6 = "<test>stuff</test >";
	
	//this should pass
	string test2 = "<test>stuff<test/></test>";
	string test3 = "<test some sort of attribute>stuff<test/></test>";
	string test4 = "<test>stuff<test valid input/></test>";
	//valid/invalid is treated as an attributes to a start tag
	string test5 = "<test>stuff<test valid/invalid></test>";
	
	//all below invalid
	string fail7 = "<test>stuff< test/></test>";
	string fail8 = "<test>stuff<test/ ></test>";
	string fail9 = "<test>stuff<test /></test>";
	string fail10 = "<test invalid attribute >stuff<test/></test>";
	string fail11 = "< test invalid attribute >stuff<test/></test>";
	string fail12 = "<test>stuff<test invalid/ ></test>";
	string fail13 = "<test>stuff<test/></test cant have attribute here>";
	string fail14 = "<test>stuff<test invalid input /></test>";\
	string fail16 = "<test>stuff< test invlaid/></test>";
	
	
	REQUIRE(!a.tokenizeInputString(fail1));
	REQUIRE(!a.tokenizeInputString(fail2));
	REQUIRE(!a.tokenizeInputString(fail3));
	REQUIRE(!a.tokenizeInputString(fail4));
	REQUIRE(!a.tokenizeInputString(fail5));
	REQUIRE(!a.tokenizeInputString(fail6));
	REQUIRE(!a.tokenizeInputString(fail7));
	REQUIRE(!a.tokenizeInputString(fail8));
	REQUIRE(!a.tokenizeInputString(fail9));
	REQUIRE(!a.tokenizeInputString(fail10));
	REQUIRE(!a.tokenizeInputString(fail12));
	REQUIRE(!a.tokenizeInputString(fail11));
	REQUIRE(!a.tokenizeInputString(fail13));
	REQUIRE(!a.tokenizeInputString(fail14));
	REQUIRE(!a.tokenizeInputString(fail16));
	REQUIRE(a.tokenizeInputString(test2));
	a.clear();
	REQUIRE(a.tokenizeInputString(test3));
	a.clear();
	REQUIRE(a.tokenizeInputString(test4));
	a.clear();
	REQUIRE(a.tokenizeInputString(test5));
	
}

//Parse tokenized input
TEST_CASE("XMLParser - parse", "[XMLParser]"){
	XMLParser a;
	string testString = "<test>stuff</test>";
	bool success;
	success = a.tokenizeInputString(testString);
	REQUIRE(success);
	REQUIRE(a.parseTokenizedInput());
	
	a.clear();
	//all below are invalid
	string fail1 = "< test>stuff</test>";
	
	//this should pass
	string test2 = "<test>stuff<test/></test>";
	string test3 = "<test some sort of attribute>stuff<test/></test>";
	string test4 = "<test>stuff<test valid input/></test>";
	//valid/invalid is treated as an attributes to a start tag
	string test5 = "<test>stuff<test valid/invalid></test>";
	
	//all below invalid
	string fail2 = "<test>stuff< test/></test>";
	string fail3 = "<test>stuff<test/ ></test>";
	string fail4 = "<test>stuff<test /></test>";
	string fail5 = "<test invalid attribute >stuff<test/></test>";
	string fail6 = "< test invalid attribute >stuff<test/></test>";
	string fail7 = "<test>stuff<test invalid/ ></test>";
	string fail8 = "<test>stuff<test/></test cant have attribute here>";
	string fail9 = "<test>stuff<test invalid input /></test>";\
	string fail10 = "<test>stuff< test invlaid/></test>";
	
	
	REQUIRE(!a.tokenizeInputString(fail1));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail2));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail3));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail4));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail5));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail6));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail7));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail8));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail9));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.tokenizeInputString(fail10));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(a.tokenizeInputString(test2));
	REQUIRE(a.parseTokenizedInput());
	a.clear();
	REQUIRE(a.tokenizeInputString(test3));
	REQUIRE(a.parseTokenizedInput());
	a.clear();
	REQUIRE(a.tokenizeInputString(test4));
	REQUIRE(a.parseTokenizedInput());
	a.clear();
	REQUIRE(a.tokenizeInputString(test5));
	REQUIRE(!a.parseTokenizedInput());
	a.clear();
	
	
	//test invalid nesting
	string failure = "<test>stuff</test><note>content</note>";
	string failure1 = "<test>stuff<note></test></note>";
	string failure2 = "<test>stuff</test></note>";
	string failure3 = "<test>stuff</test><note>";
	string failure4 = "<test>stuff</test>content";
	string failure5 = "content<test>stuff</test>";
	
	REQUIRE(a.tokenizeInputString(failure));
	REQUIRE(!a.parseTokenizedInput());
	a.clear();
	
	REQUIRE(a.tokenizeInputString(failure1));
	REQUIRE(!a.parseTokenizedInput());
	a.clear();
	
	REQUIRE(a.tokenizeInputString(failure2));
	REQUIRE(!a.parseTokenizedInput());
	a.clear();
	
	REQUIRE(a.tokenizeInputString(failure3));
	REQUIRE(!a.parseTokenizedInput());
	a.clear();
	
	REQUIRE(!a.tokenizeInputString(failure4));
	
	REQUIRE(!a.tokenizeInputString(failure5));
}

//test returnTokenizedInput
TEST_CASE("XMLParser - returnTokenizedInput","[XMLParser]"){
	XMLParser a;
	string testString = "<test>stuff</test>";
	bool success;
	success = a.tokenizeInputString(testString);
	REQUIRE(success);
	REQUIRE(a.parseTokenizedInput());
	std::vector<TokenStruct> result = { TokenStruct{StringTokenType::START_TAG, std::string("test")},
									TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
									TokenStruct{StringTokenType::END_TAG, std::string("test")}};
	std::vector<TokenStruct> output = a.returnTokenizedInput();
	for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
	}
	a.clear();
	
	//this should pass
	string test2 = "<test>stuff<test/></test>";
	string test3 = "<test some sort of attribute>stuff<test/></test>";
	string test4 = "<test>stuff<test valid input/></test>";
	
	REQUIRE(a.tokenizeInputString(test2));
	REQUIRE(a.parseTokenizedInput());
	result = { TokenStruct{StringTokenType::START_TAG, std::string("test")},
									TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
									TokenStruct{StringTokenType::EMPTY_TAG, std::string("test")},
									TokenStruct{StringTokenType::END_TAG, std::string("test")}};
	output = a.returnTokenizedInput();
	for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
	}
	a.clear();
	
	REQUIRE(a.tokenizeInputString(test3));
	REQUIRE(a.parseTokenizedInput());
	result = { TokenStruct{StringTokenType::START_TAG, std::string("test")},
									TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
									TokenStruct{StringTokenType::EMPTY_TAG, std::string("test")},
									TokenStruct{StringTokenType::END_TAG, std::string("test")}};
	output = a.returnTokenizedInput();
	for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
	}
	a.clear();
	
	REQUIRE(a.tokenizeInputString(test4));
	REQUIRE(a.parseTokenizedInput());
	result = { TokenStruct{StringTokenType::START_TAG, std::string("test")},
									TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
									TokenStruct{StringTokenType::EMPTY_TAG, std::string("test")},
									TokenStruct{StringTokenType::END_TAG, std::string("test")}};
	output = a.returnTokenizedInput();
	for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
	}
	a.clear();
	
}

//test containsElement
TEST_CASE("XMLParser - containsElementName","[XMLParser]"){
	XMLParser a;
	string testString = "<test>stuff</test>";
	bool success;
	success = a.tokenizeInputString(testString);
	REQUIRE(success);
	REQUIRE(!a.containsElementName("test"));
	REQUIRE(a.parseTokenizedInput());
	REQUIRE(a.containsElementName("test"));
	
	a.clear();
	
	//this should pass
	string test2 = "<note>stuff<note/></note>";
	
	REQUIRE(a.tokenizeInputString(test2));
	REQUIRE(a.parseTokenizedInput());
	REQUIRE(a.containsElementName("note"));
	a.clear();
	
	//test valid token but invalid XML
	string test3 = "<note>stuff</test>";
	
	REQUIRE(a.tokenizeInputString(test3));
	REQUIRE(!a.parseTokenizedInput());
	REQUIRE(!a.containsElementName("note"));
	a.clear();
	
}

//test FrequencyElementName
TEST_CASE("XMLParser - frequencyElementName","[XMLParser]"){
	XMLParser a;
	//test that only the start tag is added
	string testString = "<test>stuff</test>";
	bool success;
	success = a.tokenizeInputString(testString);
	REQUIRE(success);
	REQUIRE(a.frequencyElementName("test")==-1);
	REQUIRE(a.parseTokenizedInput());
	REQUIRE(a.containsElementName("test"));
	REQUIRE(a.frequencyElementName("test")==1);
	
	a.clear();
	
	//testthat the empty tag is also added
	string test2 = "<note>stuff<note/></note>";
	
	REQUIRE(a.tokenizeInputString(test2));
	REQUIRE(a.frequencyElementName("test")==-1);
	REQUIRE(a.parseTokenizedInput());
	REQUIRE(a.containsElementName("note"));
	REQUIRE(a.frequencyElementName("note")==2);
	a.clear();
	
}
