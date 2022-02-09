//  Adapated from Frank M. Carrano and Timothy M. Henry.

/** ADT Stack implementation.
    @file Stack.cpp */

#include <iostream>
#include <cstddef>
#include "Stack.hpp"
#include "Node.hpp"

/* @File "Stack.cpp"
 * Implementation of ADT stack.
 * @Author Joseph Schurer
 * @Date 6/18/2020 
 */

//default stack constructor
template<class ItemType>
Stack<ItemType>::Stack() : headPtr(nullptr), currentSize(0)
{
}  // end default constructor


//Default Destructor
template<class ItemType>
Stack<ItemType>::~Stack()
{
	//while the stack is not empty pop it
	while(isEmpty() == 0){
		pop();
	}
}  // end destructor


//isEmpty function
template<class ItemType>
bool Stack<ItemType>::isEmpty() const
{
	if(headPtr == nullptr) {return true;}
	
	return false;
}  // end isEmpty

//size function returns the size of the stack
template<class ItemType>
int Stack<ItemType>::size() const
{
	return currentSize;
}  // end size

//push an item onto the stack
template<class ItemType>
bool Stack<ItemType>::push(const ItemType& newItem)
{
	//create a new node, add the item to it, and make it the head
	Node<ItemType>* temp = new Node<ItemType>(newItem,headPtr);
	headPtr = temp;
	temp = nullptr;
	
	//increment size
	currentSize++;
	
	return true;
}  // end push

//peek returns top item on stack if not empty
template<class ItemType>
ItemType Stack<ItemType>::peek() const throw(logic_error)
{
	//throw exception if the stack is empty
	if(isEmpty()) { 
		throw std::logic_error("You cannot peeek an empty stack!");
	}
	
	//get item and return
	//ItemType returnItem = headPtr->getItem();
	return headPtr->getItem();;
}  // end peek

//pop remove the top item from the stack
template<class ItemType>
bool Stack<ItemType>::pop() 
{
	//stack must not be empty to pop
	if(isEmpty() == 0){
		//get a pointer to headptr
		Node<ItemType>* tempNode = headPtr;
		headPtr = headPtr -> getNext();
		
		//free the memory used by head
		tempNode -> setNext(nullptr);
		delete tempNode;
		tempNode = nullptr;
		
		//decrement currentSize
		currentSize--;
		
		return true;
	}
	
	//if the stack is emptry return false
	return false;
}  // end pop

//clear the stack and return the size to 0
template<class ItemType>
void Stack<ItemType>::clear()
{
	//while the stack is not empty pop it
	while(isEmpty() == 0){
		pop();
	}
	
	//reset currentSize and the headptr
	currentSize = 0;
	headPtr = nullptr;
	
}  // end clear
