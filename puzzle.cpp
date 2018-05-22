/*	 
*/

#include "puzzle.h"
#include  <cmath>
#include  <assert.h>

using namespace std;

//////////////////////////////////////////////////////////////
//constructor
//////////////////////////////////////////////////////////////
Puzzle::Puzzle(const Puzzle &p) : path(p.path){
	
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    board[i][j] = p.board[i][j];
		    goalBoard[i][j] = p.goalBoard[i][j];
		}
	}	
	
	x0 = p.x0;
	y0 = p.y0;
	//path = p.path;
	pathLength = p.pathLength;
	hCost = p.hCost;
	fCost = p.fCost;	
	strBoard = toString(); //uses the board contents to generate the string equivalent
	depth = p.depth;
	
}

//////////////////////////////////////////////////////////////
//constructor
//inputs:  initial state, goal state
//////////////////////////////////////////////////////////////
Puzzle::Puzzle(string const elements, string const goal){
	
	int n;
	
	n = 0;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    board[i][j] = elements[n] - '0';
		    if(board[i][j] == 0){
			    x0 = j;
			    y0 = i;
			 }
		    n++;
		} 
	}
		
	///////////////////////
	n = 0;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    goalBoard[i][j] = goal[n] - '0';
		    n++;
		} 
	}		
	///////////////////////	
	path = "";
	pathLength=0;
	hCost = 0;
	fCost = 0;
	depth = 0;
	strBoard = toString();	
}


/*Set the searching depth for PDS*/
void Puzzle::setDepth(int d){
	depth = d;
}

/*Get the searching depth for PDS*/
int Puzzle::getDepth(){
	return depth;
}

/*Update the hCost for A star search*/
void Puzzle::updateHCost(heuristicFunction hFunction){
	hCost = h(hFunction);
}

/*Update FCost for A star search. Fcost is path length plus hcost*/
void Puzzle::updateFCost(){
	fCost = pathLength + hCost;
}

/*Get the FCost for A star searching*/
int Puzzle::getFCost(){
	return fCost;
}

/*Get HCost for A star search*/
int Puzzle::getHCost(){
	return hCost;
}

/*Get the path length for A star search*/
int Puzzle::getGCost(){
	return pathLength;
}

/*Function that checks if a state will create local loop*/
bool Puzzle::isLocalLoop(int currentState) {
	return (previousStates.find(currentState) != previousStates.end());
}

/*Avoid local loops*/
set<int> Puzzle::getPreviousStates() {
	return previousStates;
}

/*Avoid local loops*/
void Puzzle::setPreviousStates(const set<int> pState) {
	previousStates = pState;
	previousStates.insert(toNumber());
}

/*Convert the puzzle to int version*/
int Puzzle::toNumber() {
	int num = 0;
	for (int i = 0; i < 3;i++) {
		for (int j = 0; j < 3;j++) {
			num = num * 10 + board[i][j];
		}
	}
	return num;
}

//Heuristic function implementation
int Puzzle::h(heuristicFunction hFunction){
	
	int sum=0;
	int h=0;
	int numOfMisplacedTiles=0;
	
	switch(hFunction){
		case misplacedTiles:			      
			for (int i = 0; i < 3;i++) {
				for (int j = 0; j < 3; j++) {
					if (board[i][j] != goalBoard[i][j] && board[i][j] != 0) {
						numOfMisplacedTiles++;
					}
				}
			}
			h = numOfMisplacedTiles; 					
			break;
		         
		case manhattanDistance:
			int temp = 0, x = 0, y = 0;
			/*Two for loop looping each position of the board array*/
			for (int tileX = 0;tileX < 3;tileX++) {
				for (int tileY = 0;tileY < 3; tileY++) {
					if(board[tileX][tileY] != 0){ /*If the value of the current coordination is not blank tile*/
						getXY(board[tileX][tileY], x, y); /*Find out the coordination in goal board*/
						temp = abs(tileX - x) + abs(tileY - y); 
						sum += temp; /*Sum up the absolute difference of x and y between board and goalboard for each tile except blank tile*/
					}
				}
			}
			h = sum;
			break;
	}
	return h;
}

/*Function that find out the coordination of the tile in goal board*/
void Puzzle::getXY(int tile, int &x, int &y) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (tile == goalBoard[i][j]) {
				x = i;
				y = j;
				break;
			}
		}
	}
}


//converts board state into its string representation
string Puzzle::toString(){
  int n;
  string stringPath;
  
  n=0;
  for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){			    
		    stringPath.insert(stringPath.end(), board[i][j] + '0');
		    n++;
		} 
  }
  return stringPath;
}


/*If each tile is match in board and goal board, we find the goal*/
bool Puzzle::goalMatch(){
	bool result=true;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] != goalBoard[i][j]) {
				result = false;
				break;
			}
		}
	}
	return result;
}
/*Return the path of the puzzle*/
const string Puzzle::getPath(){
	return path;
}

/*Check if blank tile can move left*/
bool Puzzle::canMoveLeft(){
   return (x0 > 0);	
}

/*Check if blank tile can move right*/
bool Puzzle::canMoveRight(){
   return (x0 < 2);
}

/*Check if blank tile can move up*/
bool Puzzle::canMoveUp(){
   return (y0 > 0);
}

/*Check if blank tile can move down*/
bool Puzzle::canMoveDown(){
   return (y0 < 2);
}

///////////////////////////////////////////////
//these functions will be useful for Progressive Deepening Search 

bool Puzzle::canMoveLeft(int maxDepth){
	if (!canMoveLeft()) {/*check if can move left inside the puzzle*/
		return false;
	}
	return (depth + 1 <= maxDepth);
}

bool Puzzle::canMoveRight(int maxDepth){
	if (!canMoveRight()) {/*check if can move right inside the puzzle*/
		return false;
	}
	return (depth + 1 <= maxDepth);
}

bool Puzzle::canMoveUp(int maxDepth){
	if (!canMoveUp()) {/*check if can move up inside the puzzle*/
		return false;
	}
	return (depth + 1 <= maxDepth);
}

bool Puzzle::canMoveDown(int maxDepth){
	if (!canMoveDown()) {/*check if can move down inside the puzzle*/
		return false;
	}
	return (depth + 1 <= maxDepth);
}

///////////////////////////////////////////////

/*Move the blank tile left*/
Puzzle *Puzzle::moveLeft(){
	
	Puzzle *p = new Puzzle(*this);
		
   if(x0 > 0){
		
		p->board[y0][x0] = p->board[y0][x0-1];/*Fill blank tile with the tile on the left*/
		p->board[y0][x0-1] = 0; /*Move the blank tile left*/
		
		p->x0--; /*Alter the position of the blank tile*/
		
		/*Alter path, path length and depth*/
		p->path = path + "L";
		p->pathLength = pathLength + 1;  
		p->depth = depth + 1; 
		
	}
	p->strBoard = p->toString();
	return p;
}

/*Move the blank tile right*/
Puzzle *Puzzle::moveRight(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(x0 < 2){
		
		p->board[y0][x0] = p->board[y0][x0+1];/*Fill blank tile with the tile on the right*/
		p->board[y0][x0+1] = 0;/*Move the blank tile right*/
		
		p->x0++; /*Alter the position of the blank tile*/
		
		/*Alter path, path length and depth*/
		p->path = path + "R";
		p->pathLength = pathLength + 1; 
     	
		p->depth = depth + 1;
		
	}
	
	p->strBoard = p->toString();
	
	return p;
	
}

/*Move the blank tile up*/

Puzzle *Puzzle::moveUp(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(y0 > 0){
		
		p->board[y0][x0] = p->board[y0-1][x0];/*Fill blank tile with the tile on the up*/
		p->board[y0-1][x0] = 0;/*Move the blank tile up*/
		
		p->y0--; /*Alter the position of the blank tile*/
		
		/*Alter path, path length and depth*/
		p->path = path + "U";
		p->pathLength = pathLength + 1;  
	
		p->depth = depth + 1;
		
	}
	p->strBoard = p->toString();
	
	return p;
	
}

/*Move the blank tile down*/
Puzzle *Puzzle::moveDown(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(y0 < 2){
		
		p->board[y0][x0] = p->board[y0+1][x0];/*Fill blank tile with the tile on the down*/
		p->board[y0+1][x0] = 0;/*Move the blank tile down*/
		
		p->y0++; /*Alter the position of the blank tile*/
		
		/*Alter path, path length and depth*/
		p->path = path + "D";
		p->pathLength = pathLength + 1;  
		
		p->depth = depth + 1;
		
	}
	p->strBoard = p->toString();	
	
	return p;
	
}

/////////////////////////////////////////////////////

/*print the board*/
void Puzzle::printBoard(){
	/*cout << "board: "<< endl;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		  cout << endl << "board[" << i << "][" << j << "] = " << board[i][j];
		}
	}
	cout << endl;*/
	for (int i = 0; i < 3;i++) {
		for (int j = 0; j < 3;j++) {
			cout << board[i][j] << " ";
			if (j == 2) {
				cout << "\n";
			}
		}
	}
	
}

/*return the path length*/
int Puzzle::getPathLength(){
	return pathLength;
}
