/*	159302 - Assignment 1 
	Junyi Chen - 16192554
	Jack Chen - 16095664  
*/

#include <string>
#include <iostream>
#include <set>

using namespace std;


enum heuristicFunction{misplacedTiles, manhattanDistance};


class Puzzle{
		
private:

    string path;
    int pathLength;
    int hCost;
    int fCost;
    int depth; 
	 set<int> previousStates; //Used to remember previous state to avoid potential local loops
    int goalBoard[3][3];
    
    int x0, y0; //coordinates of the blank or 0-tile
    
    int board[3][3];
    
public:
    
    string strBoard;
     
    Puzzle(const Puzzle &p); //Constructor
    Puzzle(string const elements, string const goal);//Constructor
     
    void printBoard();
    
    int h(heuristicFunction hFunction);
     

    void updateFCost(); 
    void updateHCost(heuristicFunction hFunction);
    void updateDepth(){
		 depth++;
	 }
	 	  
	 
    bool goalMatch();
	 string toString();
	
    string getString(){
		return strBoard;
	 }
    
    bool canMoveLeft();
    bool canMoveRight();
    bool canMoveUp();
    bool canMoveDown();  

//----------------------------------------
//these functions will be useful for Progressive Deepening Search    
	 bool canMoveLeft(int maxDepth);
	 bool canMoveDown(int maxDepth);
	 bool canMoveRight(int maxDepth);
	 bool canMoveUp(int maxDepth);	 
//----------------------------------------
	  
    Puzzle * moveLeft();
    Puzzle * moveRight();
    Puzzle * moveUp();
    Puzzle * moveDown();
	 
	 
    const string getPath();
    
    void setDepth(int d);
    int getDepth();
    
    int getPathLength();
    int getFCost();
	int getHCost();
	int getGCost();	
	void getXY(int tile, int &x, int &y);/*function use to get the coordinates of goal board and then to calculate the manhattan value*/

	/*function return the state in a number format*/
	int toNumber();

	/*function check if the current state is local loop*/
	bool isLocalLoop(int currentState);
	
	/*function return the vector of previous states*/
	set<int> getPreviousStates();

	void setPreviousStates(const set<int> pState);

	void printStateList() {
		for (int n : previousStates)
		{
			cout << n << " , ";
		}
		cout << "\n";
	}
};
