/*	159302 - Assignment 1 
	Junyi Chen - 16192554
	Jack Chen - 16095664  
*/
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <ctime>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "puzzle.h"
#include <vector>
#include <set>
#include <map>

const heuristicFunction HEURISTIC_FUNCTION=manhattanDistance;

//Function prototypes
string progressiveDeepeningSearch_No_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth);
string progressiveDeepeningSearch_with_NonStrict_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth);

string breadthFirstSearch_with_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime);
string breadthFirstSearch(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime);

string aStar_ExpandedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, 
                               float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions, heuristicFunction heuristic);

#endif

/*Node*/
struct Node{
	Puzzle* puzzle;
	Node *next;
};

/*Visited list*/
struct VisitedList{
	std::set<int> data;/*A set data structure that contains the states in integer format*/
};

/*Non Strict list*/
struct NonStrictList{
	/*Using map data structue which contains the state as key and depth level as value*/
	std::map<int, int> data;
};

/*Queue data structure for breadth first search*/
class BfsQueue{
	
	private :
		Node *front, *rear;
		int count; /*Keep track of the number of elements in queue*/
	
	public :
		
		/*Constructor*/
		BfsQueue(){
			front = NULL;
			rear = NULL;
			count = 0;
		}
		
		/*Function that joins the new puzzle to the rear of the queue*/
		void join(Puzzle *newPuzzle){
			Node *temp = new Node;
			temp->puzzle = newPuzzle;
			temp -> next = NULL;
			if(rear != NULL){
				rear -> next = temp;
			}
			rear = temp;
			if(front == NULL){
				front = temp;
			}
			count += 1;
		}
		
		/*Remove the first element of the queue*/
		void leave(){
			if(front == NULL){
				return;
			}
			Node *temp = front;
			front = front -> next;
			if(front == NULL){
				rear = NULL;
			}
			delete temp->puzzle;
			delete temp;
			count -= 1;
		}

		/*Check if queue is empty. True if empty, false otherwise.*/
		bool isEmpty() {
			if (front == NULL) {
				return true;
			}
			else {
				return false;
			}
		}
		
		/*Return the first(front) element of the queue*/
		Node *Front() { 
			return front;
		};

		Puzzle * frontPuzzle(); /*function return the front puzzle inside the queue*/

};


/*Stack data structure for PDS*/
class Stack {
	private:
		Node * front;
	public :
		/*Constructor*/
		Stack() {
			front = NULL;
		}

		/*Put a new element at the top of the stack*/
		void push(Puzzle * newPuzzle) {
			Node *temp = new Node;
			temp->puzzle = newPuzzle;
			temp->next = front;
			front = temp;
		}

		/*Remove the top element of the stack*/
		void pop() {
			Node *temp = front;
			if(front != NULL){
				front = front->next;
				delete temp;
			}
		
		}
		
		/*Return the top(first) element of the stack*/
		Puzzle* Top() {
			if (front != NULL)
			{
				return front->puzzle;
			}
			return NULL;
		}

		/*Check if stack is empty.*/
		bool isEmpty() {
			return (front == NULL);
		}

};


/*Heap data structure for A star search*/
class Heap {

	private:		
		vector<Puzzle*> data; /*Vector structure that contains Puzzle data*/
		int last; /*Index of the last element*/

	public:
		/*Heap constructor*/
		Heap() {
			last = -1;
		}
		
		/*Get the index of last element. -1 means empty*/
		int getLast(){
			return last;
		}
	
		void printHeap(){
			for(int i = 0; i <= last;i++){
				cout<<data[i]->toNumber()<<" "<<data[i]->getFCost()<<endl;
			}
		}

		/*Function to check if there is a state that is the same as the new one but has smaller Fcost*/
		/*If the state of new puzzle is visited and stored in the heap but with smaller FCOST then we dont add it. return -1 */
		/*If the state of new puzzle is visited and stored in the heap but with greater FCOST, we return the index of the old puzzle.*/
		/*If the state of new puzzle is not visited yet return -2*/
		int hasSmallerCost(Puzzle *puzzle) {
			/*get the string representation of the state*/
			int state = puzzle->toNumber();
			int fcost = puzzle->getFCost();
			
			for (int i = 0; i <= last;i++) {
				/*check if the state is the same and has lower fcost */
				if (data[i]->toNumber() == state){
					if(data[i]->getFCost() <= fcost) {
						return -1;
					} else if (data[i]->getFCost() > fcost) {
						return i;
					}
				}
				
			}
			/*the state is the same as the new puzzle but has a lager fcost*/
				return -2;
		}

		
		/*Function that does the attempt to insert the new element into the heap. Rerurn true if insertion is executed, false otherwise.*/
		bool insertToHeap(Puzzle* puzzle, int &numOfDeletionsFromMiddleOfHeap) {

			/*check if the state of the new puzzle is visited with smaller or larger fcost, or it is not visited*/
			int index = hasSmallerCost(puzzle);

			if (index == -1) {/*The state is already existed in heap and with lower FCost. No need to add it*/
				numOfDeletionsFromMiddleOfHeap++; 
				return false;
			}
			/*The state is already existed in heap but with greater FCost. Replace the old state with the new one*/
			else if (index != -1 && index != -2) {
				numOfDeletionsFromMiddleOfHeap++;
				deleteFromMiddle(index);
			}
			
			/*Add the data to the end of the vector*/
			data.push_back(puzzle);
			
			/*Keep track of the index of last puzzle in the vector*/
			last++;

			if (last == 0) {/*The first puzzle in the heap, no need to do the swapping below*/
				return true;
			}
			
			/*Start sorting the heap by swapping the element.*/
			/*Put the puzzle in order with smallest FCOST at the front and puzzle with biggest FCOST at the end*/
			
			bool swap = true;
			/*index of the leaf node*/
			int leaf = last;
			/*index of the parent node*/
			int root = 0;

			while (swap) {
				swap = false;
				/*get the index of the parent node*/
				root = (leaf - 1) / 2;

				/*if leaf puzzle has a lower FCOST swap it with the parent puzzle*/
				if(root >= 0){
					if (data[leaf]->getFCost() < data[root]->getFCost()) {
						Puzzle*temp = data[leaf];
						data[leaf] = data[root];
						data[root] = temp;
						swap = true;
						leaf = root;
					}
				}
			}	
			return true;
		}
		
		/*Function that delete the puzzle from the middle of the heap*/
		void deleteFromMiddle(int index) {
			/*swaping*/
			Puzzle* temp = data[index];
			data[index] = data[last];
			delete temp;
			last--;
			data.pop_back();
			
			int root = index;
			int left = (root * 2) + 1; /*Left child of the root*/
			int right = (root * 2) + 2; /*Right child of the root*/

			/*Only one puzzle left in the heap*/ 
			if (left > last) {
				return;
			}
			/*Only two puzzles left in the heap*/
			else if (right > last) {
				/*Check if the last leaf has smaller fcost, swap if true*/
				if (data[left]->getFCost() < data[root]->getFCost()) {
					Puzzle *temp = data[left];
					data[left] = data[root];
					data[root] = temp;
				}
				return;
			}
			
			int side;
			/*check which leaf of the root has a lower FCOST swap with that side*/
			if (data[left]->getFCost() <= data[right]->getFCost()) {
				side = left;
			}
			else {
				side = right;
			}

			while (data[side]->getFCost() < data[root]->getFCost()) {
				/*Swaping*/
				Puzzle* temp = data[side];
				data[side] = data[root];
				data[root] = temp;
				/*Keep trakcing the FCOST for next leaves*/
				root = side;
				left = (root * 2) + 1;
				right = (root * 2) + 2;
				/*Only one puzzle in the heap*/
				if (left > last) {
					break;
				}
				/*Only two puzzles in the heap*/
				else if (right > last) {
					/*Do the swap if necessary and break the loop*/
					if (data[left]->getFCost() < data[root]->getFCost()) {
						Puzzle *temp = data[left];
						data[left] = data[root];
						data[root] = temp;
					}
					break;
				}
				else {
					/*Check which leaf of the current root has a lower FCOST swap with that side*/
					if (data[left]->getFCost() <= data[right]->getFCost()) {
						side = left;
					}
					else {
						side = right;
					}
				}
			}
		}
		
		/*Function that delete the root of the heap and return its reference*/
		void deleteHeapRoot(Puzzle **front) {/*reference to get the puzzle in the front which has the smallest fcost*/
			/*no puzzle inside the heap*/
			if (last < 0) {
				(*front) = NULL;
				return;
			}
			/*retrive first puzzle of the heap which has lowest FCOST*/
			(*front) = data[0];
			/*Copy the last puzzle to the first and delete the last puzzle*/
			data[0] = data[last];
			data[last] = NULL;
			last--;	
			data.pop_back();
	
			int root = 0;
			int left = 1;
			int right = 2;
			/*Only one puzzle left in the heap*/
			if (left > last) {
				return;
			}
			/*Only two puzzles left in the heap*/
			else if (right > last) {
				/*check if the last leaf has smaller fcost, swap if true*/
				if (data[left]->getFCost() < data[root]->getFCost()) {
					Puzzle *temp = data[left];
					data[left] = data[root];
					data[root] = temp;
				}
				return;
			}
			int side;
			/*Check which leaf of the root has a lower FCOST swap with that side*/
			if (data[left]->getFCost() <= data[right]->getFCost()) {
				side = left;
			}	
			else {
				side = right;
			}

			while (data[side]->getFCost() < data[root]->getFCost()) {
				/*Swaping*/
				Puzzle* temp = data[side];
				data[side] = data[root];
				data[root] = temp;
				/*Keep trakcing the FCOST for next leaves*/
				root = side;
				left = (root * 2) + 1;
				right = (root * 2) + 2;
				/*One puzzle in the heap*/
				if (left > last) { 
					break; 
				}
				/*Two puzzle in the heap*/
				else if (right > last) {
					/*do the swap if necessary and break the loop*/
					if (data[left]->getFCost() < data[root]->getFCost()) {
						Puzzle *temp = data[left];
						data[left] = data[root];
						data[root] = temp;
					}
					break;
				}
				else {
					/*check which leaf of the root has a lower FCOST swap with that side*/
					if (data[left]->getFCost() <= data[right]->getFCost()) {
						side = left;
					}
					else {
						side = right;
					}
				}
			}
		}
};