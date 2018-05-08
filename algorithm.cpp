/*	159302 - Assignment 1 
	Junyi Chen - 16192554
	Jack Chen - 16095664  
*/
#include "algorithm.h"

using namespace std;

Puzzle* BfsQueue::frontPuzzle() { /*function return the front puzzle inside the queue*/
	return front->puzzle;
}

/*Function that adds the state to a noraml visisted list*/
void addToList(struct VisitedList &myList, int data) {
	myList.data.insert(data);
}

/*Function that checks if the state is visited*/
bool isVisited(struct VisitedList const &myList, int data) {
	return (myList.data.find(data) != myList.data.end());
}


/*Fucntion that adds the state to a non strict list*/
void addToNonStrictList(struct NonStrictList &myList, int state, int depthLevel) {
	myList.data.insert(make_pair(state, depthLevel));
}

/*Function that checks if the state with depth level is visited, if not visited, add to the list, if the same state with lower depth level
is passed, repalce the one with higher depth level*/
bool isVisitedNonStrict(struct NonStrictList &myList, int state, int depth) {
	/*get the iterator and check if can find value*/
	map<int, int>::iterator it = myList.data.find(state);
	if (it == myList.data.end()) {
		/*Not foind. Not visited yet*/
		addToNonStrictList(myList, state, depth);
		return false;
	}
	else {
		/*We found a same state inside the list, check if it has a lower depth level*/
		/*if state has lower depth level, return true as it is already visited*/
		if (myList.data[state] <= depth) {
			return true;
		}
		/*The state inside the list has higher depth level, replace it*/
		else {
			myList.data[state] = depth;
			return false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Breadth-First Search with no visited list
//
// Move Generator:  
//
// This algorithm will run out of memory in some cases.
////////////////////////////////////////////////////////////////////////////////////////////
string breadthFirstSearch(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
 
	string path;
   
	/*start timing*/
	clock_t startTime;
	startTime = clock();
	
	/*Initialisation of the counter*/
	numOfStateExpansions = 0;  /*number of expansion is 0 at the begining*/
	maxQLength = 1; /*Max Q length is 1 at the begining(initial state)*/
	int currentQLength = 1; /*Used to keep track of currentQLength. If currentQ is greater than maxQ, set maxQ as currentQ*/

	/*Implement the queue structure*/
	BfsQueue *myQueue = new BfsQueue();
	/*Initial puzzle contain the initial state and the goal state*/
	Puzzle *initial = new Puzzle(initialState, goalState);
	
	/*Set structure that contains the previous states which used for avoiding local loops*/
	set<int> tempSet;
	initial->setPreviousStates(tempSet);
	/*Add initial to queue*/
	myQueue->join(initial);
		
   try {
	   /*while the queue is not empty*/
	   while (!myQueue->isEmpty()) {
		   numOfStateExpansions++;/*Expand one state in the front of queue*/
		   /*get the front puzzle in the queue*/
		   Puzzle *front = myQueue->frontPuzzle();
			currentQLength--; /*One state taken out from queue*/
			
		   /*check the goal*/
		   if (front->goalMatch()) {
			   break;
		   }

		   /*check if the blank tile can move up*/
		   if (front->canMoveUp()) {
			   Puzzle *temp = front->moveUp();
			   /*avoid local loop*/
			   if (!front->isLocalLoop(temp->toNumber())) {
				   temp->setPreviousStates(front->getPreviousStates());
				   myQueue->join(temp);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
			   }
			   else {
				   delete temp;
			   }
		   }
		   /*check if the blank tile can move down*/
		   if (front->canMoveDown()) {
			   Puzzle *temp = front->moveDown();
			   /*avoid local loop*/
			   if (!front->isLocalLoop(temp->toNumber())) {
				   temp->setPreviousStates(front->getPreviousStates());
				   myQueue->join(temp);
					currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
			   }
			   else {
				   delete temp;
			   }
		   }
		   /*check if the blank tile can move left*/
		   if (front->canMoveLeft()) {
			   Puzzle *temp = front->moveLeft();
			   /*avoid local loop*/
			   if (!front->isLocalLoop(temp->toNumber())) {
				   temp->setPreviousStates(front->getPreviousStates());
				   myQueue->join(temp);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
			   }
			   else {
				   delete temp;
			   }
		   }
		   /*check if the blank tile can move right*/
		   if (front->canMoveRight()) {
			   Puzzle *temp = front->moveRight();
			   /*avoid local loop*/
			   if (!front->isLocalLoop(temp->toNumber())) {
				   temp->setPreviousStates(front->getPreviousStates());
				   myQueue->join(temp);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
			   }
			   else {
				   delete temp;
			   }
		   }
			/*First element expanded, remove from queue*/
		   myQueue->leave();
	   }
   }
   catch (exception e) {
	   actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
	   delete myQueue;
	   cout << "Runing out of memory !"<<endl;
	   return NULL;
   }
	
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	if (!myQueue->isEmpty()) {
		path = myQueue->frontPuzzle()->getPath();
	}
	else {
		path = "";
	}
	
	delete myQueue;
	return path;		
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Breadth-First Search with VisitedList
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string breadthFirstSearch_with_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
	
	string path;
	
	/*Start timing*/
	clock_t startTime;
	startTime = clock();
	
	/*Initialisation of the counter*/
	numOfStateExpansions = 0; /*number of expansion is 0 at the begining*/
	maxQLength = 1;/*Max Q length is 1 at the begining(initial state)*/
	int currentQLength = 1; /*Used to keep track of currentQLength. If currentQ is greater than maxQ, set maxQ as currentQ*/
	
	/*Generate a visited list*/
	VisitedList visited;
	/*Add the initial state to the visited list*/
	Puzzle *initialPuzzle = new Puzzle(initialState, goalState);

	/*Generate a queue for BFS*/
	BfsQueue *myQueue = new BfsQueue();
	myQueue->join(initialPuzzle);
	addToList(visited, initialPuzzle->toNumber());
	try {
		while (!myQueue->isEmpty()) {
			numOfStateExpansions++;
			/*get the front puzzle in the queue*/
			Puzzle *temp = myQueue->frontPuzzle();
			currentQLength--;
			/*goal check*/
			if (temp->goalMatch()) {
				break;
			}

			/*if blank can move up*/
			if (temp->canMoveUp()) {
				/*get the number format of the state*/
				int num = temp->moveUp()->toNumber();
				/*check if next state is visited*/
				if (!isVisited(visited, num)) {
					/*no visited, add to Q add state to visited list*/
					myQueue->join(temp->moveUp());
					addToList(visited, num);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
				}
			}
			
			/*if blank can move down*/
			if (temp->canMoveDown()) {
				/*get the number format of the state*/
				int num = temp->moveDown()->toNumber();
				/*check if next state is visited*/
				if (!isVisited(visited, num)) {
					/*no visited, add to Q add state to visited list*/
					myQueue->join(temp->moveDown());
					addToList(visited, num);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
				}
			}
			
			/*if blank can move left*/
			if (temp->canMoveLeft()) {
				/*get the number format of the state*/
				int num = temp->moveLeft()->toNumber();
				/*check if next state is visited*/
				if (!isVisited(visited, num)) {
					/*no visited, add to Q add state to visited list*/
					myQueue->join(temp->moveLeft());
					addToList(visited, num);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
				}
			}
			
			/*if blank can move Right*/
			if (temp->canMoveRight()) {
				/*get the number format of the state*/
				int num = temp->moveRight()->toNumber();
				/*check if next state is visited*/
				if (!isVisited(visited, num)) {
					/*no visited, add to Q add state to visited list*/
					myQueue->join(temp->moveRight());
					addToList(visited, num);
				   currentQLength++;
					if(currentQLength > maxQLength){
						maxQLength = currentQLength;
					}
				}
			}
			
			/*First element expanded, remove from queue*/
			myQueue->leave();
		}
	}
	catch (exception e) {
		actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
		cout << "Runing out of memory !" << endl;
		delete myQueue;
		return NULL;
	}
	
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	if (!myQueue->isEmpty()) {
		path = myQueue->frontPuzzle()->getPath();
	}
	else {
		path = "";
	}

	delete myQueue;
	return path;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm: Progressive Deepening Search with no visited list
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string progressiveDeepeningSearch_No_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
	
	string path;
	
	/*Start timing*/
	clock_t startTime;
 	startTime = clock();
	
	/*Initialisation of the counter*/
	numOfStateExpansions = 0; /*number of expansion is 0 at the begining*/
	maxQLength = 1;/*Max Q length is 1 at the begining(initial state)*/
	int currentQLength = 1; /*Used to keep track of currentQLength. If currentQ is greater than maxQ, set maxQ as currentQ*/
	/*Here we use stack instead of queue as we put the path extensions to the front of Q.*/
	Stack *myStack = new Stack();
	/*set the initial state and depth level*/
	Puzzle* initialPuzzle = new Puzzle(initialState, goalState);
	int maxDepth = 3; /*Maximum of depth will start from 3*/
	
	set<int> tempSet;/*a set used to record the local states that have been visited*/
	initialPuzzle->setPreviousStates(tempSet);
	initialPuzzle->setDepth(0);
	/*push to the stack*/
	myStack->push(initialPuzzle);

	while (maxDepth <= 30) {
		
		/*get the top of the stack*/
		Puzzle *puzzle = myStack->Top();
		numOfStateExpansions++;
		myStack->pop(); /*First element expanded, delete from stack*/
		currentQLength--;
		if (puzzle->goalMatch()) {/*if goal match*/
			/*get the path*/
			path = puzzle->getPath();
			break;
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move up*/
		if (puzzle->canMoveUp(maxDepth)) {
			Puzzle *newPuzzle = puzzle->moveUp();
			/*check local loop*/
			if (!puzzle->isLocalLoop(newPuzzle->toNumber())) {
				newPuzzle->setDepth(puzzle->getDepth() + 1);
				newPuzzle->setPreviousStates(puzzle->getPreviousStates());
				/*push to the stack*/
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move down*/
		if (puzzle->canMoveDown(maxDepth)) {
			Puzzle *newPuzzle = puzzle->moveDown();
			/*check local loop*/
			if (!puzzle->isLocalLoop(newPuzzle->toNumber())) {
				
				newPuzzle->setDepth(puzzle->getDepth() + 1);
				newPuzzle->setPreviousStates(puzzle->getPreviousStates());
				/*push to the stack*/
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move left*/
		if (puzzle->canMoveLeft(maxDepth)) {
			Puzzle *newPuzzle = puzzle->moveLeft();
			/*check local loop*/
			if (!puzzle->isLocalLoop(newPuzzle->toNumber())) {
				newPuzzle->setDepth(puzzle->getDepth() + 1);
				newPuzzle->setPreviousStates(puzzle->getPreviousStates());
				/*push to the stack*/
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move right*/
		if (puzzle->canMoveRight(maxDepth)) {
			Puzzle *newPuzzle = puzzle->moveRight();
			/*check local loop*/
			if (!puzzle->isLocalLoop(newPuzzle->toNumber())) {
				newPuzzle->setDepth(puzzle->getDepth() + 1);
				newPuzzle->setPreviousStates(puzzle->getPreviousStates());
				/*push to the stack*/				
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*if stack is empty that means we reached the max depth level we set*/
		if (myStack->isEmpty()) {
			cout << "Depth now " << maxDepth <<endl;
			/*free memory*/
			delete puzzle;
			/*increase the max depth level*/
			maxDepth += 1;
			/*restart the algorithm from the begining*/
			if (maxDepth <= 30) {
				currentQLength = 1;
				numOfStateExpansions = 0;
			}
			/*set the initial state and depth level*/
			Puzzle* initialPuzzle = new Puzzle(initialState, goalState);
			set<int> tempSet;
			initialPuzzle->setPreviousStates(tempSet);
			initialPuzzle->setDepth(0);
			/*push to the stack*/
			myStack->push(initialPuzzle);
			continue;
		}
		/*free memory*/
		delete puzzle;
	}
	
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	delete myStack;
	return path;		
}
	



///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Progressive Deepening Search with non strict visited list
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string progressiveDeepeningSearch_with_NonStrict_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){

	string path;

	/*Start timing*/
	clock_t startTime;
	startTime = clock();
	
	/*Initialisation of the counter*/
	numOfStateExpansions = 0; /*number of expansion is 0 at the begining*/
	maxQLength = 1;/*Max Q length is 1 at the begining(initial state)*/
	int currentQLength = 1; /*Used to keep track of currentQLength. If currentQ is greater than maxQ, set maxQ as currentQ*/

	int maxDepth = 3;
	
	/*generate stack*/
	Stack *myStack = new Stack();
	
	/*set the initial state and depth level*/
	Puzzle* initialPuzzle = new Puzzle(initialState, goalState);
	initialPuzzle->setDepth(0);
	
	/*push the initial state to the stack*/
	myStack->push(initialPuzzle);

	/*generate a non strict visited list and add initial state & depth level to the list*/	
	NonStrictList visited;
	addToNonStrictList(visited, initialPuzzle->toNumber(), initialPuzzle->getDepth());

	/*algorithm starts*/
	while (maxDepth <= 30) {

		/*get the top of the stack and then delete it*/
		Puzzle *puzzle = myStack->Top();
		myStack->pop();
		
		numOfStateExpansions++;/*increase the number of expansion*/
		currentQLength--;
		
		if (puzzle->goalMatch()) {/*if goal match*/
		  /*get the path*/
			path = puzzle->getPath();
			break;
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move up*/
		if (puzzle->canMoveUp(maxDepth)) {
			
			Puzzle *newPuzzle = puzzle->moveUp();
			newPuzzle->setDepth(puzzle->getDepth() + 1);
			/*check visited list, if the state is not visited, the state will be added to both stack the visited list*/
			if (!isVisitedNonStrict(visited, newPuzzle->toNumber(), newPuzzle->getDepth())) {
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move down*/
		if (puzzle->canMoveDown(maxDepth)) {

			Puzzle *newPuzzle = puzzle->moveDown();
			newPuzzle->setDepth(puzzle->getDepth() + 1);
			/*check visited list, if the state is not visited, the state will be added to both stack the visited list*/
			if (!isVisitedNonStrict(visited, newPuzzle->toNumber(), newPuzzle->getDepth())) {
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move left*/
		if (puzzle->canMoveLeft(maxDepth)) {

			Puzzle *newPuzzle = puzzle->moveLeft();
			newPuzzle->setDepth(puzzle->getDepth() + 1);
			/*check visited list, if the state is not visited, the state will be added to both stack the visited list*/
			if (!isVisitedNonStrict(visited, newPuzzle->toNumber(), newPuzzle->getDepth())) {
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		
		/*check if depthlevel is smaller than the maximum depth level and the blank tile can move right*/
		if (puzzle->canMoveRight(maxDepth)) {

			Puzzle *newPuzzle = puzzle->moveRight();
			newPuzzle->setDepth(puzzle->getDepth() + 1);
			/*check visited list, if the state is not visited, the state will be added to both stack the visited list*/
			if (!isVisitedNonStrict(visited, newPuzzle->toNumber(), newPuzzle->getDepth())) {
				myStack->push(newPuzzle);
				currentQLength++;
				if(currentQLength > maxQLength){
					maxQLength = currentQLength;
				}
			}
			else {
				delete newPuzzle;
			}
		}

		/*Stack is empty, increase the max depth level, and restart the algorithm*/
		if (myStack->isEmpty()) {
			cout << "Depth now " << maxDepth << "\n";
			/*free memory*/
			delete puzzle;
			/*increase the max depth level*/
			maxDepth += 1;
			if (maxDepth <= 30) {
				currentQLength = 1;
				numOfStateExpansions = 0;
			}
			/*set the initial state and depth level*/
			Puzzle* initialPuzzle = new Puzzle(initialState, goalState);
			initialPuzzle->setDepth(0);
			/*push to the stack*/
			myStack->push(initialPuzzle);
			
			NonStrictList temp;
			visited = temp;
			addToNonStrictList(visited, initialPuzzle->toNumber(), initialPuzzle->getDepth());
			/*restart the algorithm*/
			continue;
		}

		/*free memory*/
		delete puzzle;

	}

//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	delete myStack;
	return path;
}
	
///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A Star Search with expanded list
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string aStar_ExpandedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, 
                               float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions, heuristicFunction heuristic){
											 
   string path = "";
											 
	/*Start timing*/
   clock_t startTime;
	startTime = clock();
   
	/*Initialisation of the counter*/
   numOfDeletionsFromMiddleOfHeap=0; /*Will be passed to @insertTOHeap for finding number of deletion from middle of heap*/
   numOfLocalLoopsAvoided=0;
   numOfAttemptedNodeReExpansions=0;
	numOfStateExpansions=0;
	maxQLength = 1;/*Max Q length is 1 at the begining(initial state)*/
	int currentQLength = 1; /*Used to keep track of currentQLength. If currentQ is greater than maxQ, set maxQ as currentQ*/

	/*Generate the heap data structure for storing state*/										 
	Heap *myHeap = new Heap();

	/*generate a visited list*/
	VisitedList visited;
	/*Instantiating the initial state*/
	Puzzle* initialPuzzle = new Puzzle(initialState, goalState);
	/*Update HCost and FCost*/
	initialPuzzle->updateHCost(heuristic);
	initialPuzzle->updateFCost();
	set<int> tempSet;
	initialPuzzle->setPreviousStates(tempSet);
	myHeap->insertToHeap(initialPuzzle,numOfDeletionsFromMiddleOfHeap);

	try{
		/*While the heap is not empty*/
		while(myHeap->getLast() >= 0){
			numOfStateExpansions++;/*expand the state in the front of heap*/
			Puzzle *front;
			
			myHeap->deleteHeapRoot(&front);/*Get the lowest fcost element and delete it from the heap*/
			
			/*Add the first element of the heap to the visited list*/
			int num = front->toNumber();
			addToList(visited, num);
			currentQLength--;
			
			if (front->goalMatch()) {
				/*Get the path*/
				path = front->getPath();
			   break;
		   }
			
			if (front->canMoveUp()) {
				Puzzle *temp = front->moveUp();
				temp->updateHCost(heuristic);
				temp->updateFCost();
			
				/*get the number format of the state*/
				num = temp->toNumber();
				
				/*check if the state is not visited and if it is not a local loop*/
				if (!isVisited(visited, num) && !front->isLocalLoop(num)) {	
					if(myHeap->insertToHeap(temp,numOfDeletionsFromMiddleOfHeap)){ /*Check if insertion is valid*/
						temp->setPreviousStates(front->getPreviousStates());
						currentQLength++;
						if(currentQLength > maxQLength){ /*Check if current Q length is the maximum*/
							maxQLength = currentQLength;
						}
					}
					else{		
						delete temp;
					}
				} else{
					/*Number of attempted node reexpansion +1 as we tried to add state that is visited and/or will produce local loop*/
					numOfAttemptedNodeReExpansions++; 
					delete temp;
				}
			}
			
			if (front->canMoveDown()) {
				Puzzle *temp = front->moveDown();
				temp->updateHCost(heuristic);
				temp->updateFCost();
				
				/*get the number format of the state*/
				num = temp->toNumber();

				/*check if the state is not visited and if it is not a local loop*/
				if (!isVisited(visited, num) && !front->isLocalLoop(num)) {
					if(myHeap->insertToHeap(temp,numOfDeletionsFromMiddleOfHeap)){ /*Check if insertion is valid*/
						temp->setPreviousStates(front->getPreviousStates());
						currentQLength++;
						if(currentQLength > maxQLength){ /*Check if current Q length is the maximum*/
							maxQLength = currentQLength;
						}
					}
					else{		
						delete temp;
					}

				} else{
					/*Number of attempted node reexpansion +1 as we tried to add state that is visited and/or will produce local loop*/
					numOfAttemptedNodeReExpansions++;
					delete temp;
				}
			}
			
			if (front->canMoveLeft() ) {
				Puzzle *temp = front->moveLeft();
				temp->updateHCost(heuristic);
				temp->updateFCost();
				
				/*get the number format of the state*/
				num = temp->toNumber();
				
				/*check if the state is not visited and if it is not a local loop*/
				if (!isVisited(visited, num) && !front->isLocalLoop(num)) {
					if(myHeap->insertToHeap(temp,numOfDeletionsFromMiddleOfHeap)){ /*Check if insertion is valid*/
						temp->setPreviousStates(front->getPreviousStates());
						currentQLength++;
						if(currentQLength > maxQLength){ /*Check if current Q length is the maximum*/
							maxQLength = currentQLength;
						}
					} else {
						delete temp;
					}
				} else{
					/*Number of attempted node reexpansion +1 as we tried to add state that is visited and/or will produce local loop*/
					numOfAttemptedNodeReExpansions++;
					delete temp;
				}
			}

			if (front->canMoveRight()) {
				Puzzle *temp = front->moveRight();
				temp->updateHCost(heuristic);
				temp->updateFCost();
				
				/*get the number format of the state*/
				num = temp->toNumber();
				
				/*check if the state is not visited and if it is not a local loop*/
				if (!isVisited(visited, num) && !front->isLocalLoop(num)) {
					if(myHeap->insertToHeap(temp,numOfDeletionsFromMiddleOfHeap)){ /*Check if insertion is valid*/
						temp->setPreviousStates(front->getPreviousStates());
						currentQLength++;
						if(currentQLength > maxQLength){ /*Check if current Q length is the maximum*/
							maxQLength = currentQLength;
						}
					} else{
						delete temp;
					}
				} else{
					/*Number of attempted node reexpansion +1 as we tried to add state that is visited and/or will produce local loop*/
					numOfAttemptedNodeReExpansions++;
					delete temp;
				}
			}	
			delete front;
		}	
	}		catch (exception e) {
	   actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
	   delete myHeap;
	   cout << "Runing out of memory !" << endl;
	   return NULL;
   }
	actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
	delete myHeap;
	return path;		
}