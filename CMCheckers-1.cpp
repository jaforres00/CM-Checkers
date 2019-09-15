//Authors: Jacob Forrest
//Date: November 27

#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

//Declare global constants
const int MAX_ARRAY_SIZE = 18;
const int MIN_ARRAY_SIZE = 8;
const int MAX_PIECES = 72;
const int NOPLAYER = 0;
const int WHITEWINS = 1;
const int REDWINS = 2;
const int NOONEWINS = 0;
const int WHITESOLDIER = 1;
const int WHITEMULE = 2;
const int WHITEKING = 3;
const int REDSOLDIER = 4;
const int REDMULE = 5;
const int REDKING = 6;
const int WHITEPLAYER = 1;
const int REDPLAYER = 2;

//Function Prototypes

void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex);

int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);

bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);

bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped);

bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

int main()
{
	//Declare variables
	int originalSquare = 0;
	int moveToSquare = 0;
	int numRowsInBoard = 0;
	int numAttempts = 0;
	//declare array myCMCheckersBoard as a local automatic variable in the main program
	int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = { 0 };
	//declare 2 arrays xIndicesMove[] and yIndicesMove[]
	int xIndicesMove[MAX_PIECES];
	int yIndicesMove[MAX_PIECES];
	//declare 2 arrows xIndicesJump[] and yIndicesJump[]
	int xIndicesJump[MAX_PIECES];
	int yIndicesJump[MAX_PIECES];
	//prompt for and read the size of the board (numRowsInBoard)
	//check the board size entered by the user
	//If the user enters an invalid value or a value that is out of range reprompt for and reread a new value. 
	//Read the value a maximum of 3 times, if the value is still invalid or out of range, terminate the program.
	do
	{
		numAttempts++;
		cout << "Enter the number of squares along each edge of the board\n";
		cin >> numRowsInBoard;
		//board size is not an integer check
		if (cin.fail())
		{
			cerr << "ERROR: Board size is not an integer.\n" << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << "\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		//board size is odd check
		else if (numRowsInBoard % 2 == 1)
		{
			cerr << "ERROR: Board size is odd.\n"<< MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << "\n";
			continue;
		}
		//board size is too large check
		else if (numRowsInBoard > MAX_ARRAY_SIZE)
		{
			cerr << "ERROR: Board size too large.\n" << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << "\n";
			continue;
		}
		//board size is too small check
		else if (numRowsInBoard < MIN_ARRAY_SIZE)
		{
			cerr << "ERROR: Board size too small.\n" << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << "\n";
			continue;
		}
		else 
		{
			break;
		}
		//reached max attempts check
		if (numAttempts == 3)
		{
			cerr << "ERROR: Too many errors entering the size of the board.\n";
			return 1;
		}
	} while (numAttempts < 3);
	//Initialize the game board using the InitializeBoard function
	InitializeBoard(myCMCheckersBoard, numRowsInBoard);
	//Display Board
	DisplayBoard(myCMCheckersBoard, numRowsInBoard);
	//white player starts the game
	int player = WHITEPLAYER;
	//infinite loop for the number of turns to complete the game, breaks inside will stop the game
	while (1)
	{
		//check if the player has any legal moves by calling the functions countJump and countMove1Squares
		//the return value of countJumps tells you how many of your checkers are able to jump the opponent's checkers
		int numJumps = CountJumps(myCMCheckersBoard, numRowsInBoard, player, xIndicesJump, yIndicesJump);
		//the return value of countMove1Squares tells you the sum of how many Mules and soldiers can be moved one space forward and how many kings can be moved
		int numMoves1 = CountMove1Squares(myCMCheckersBoard, numRowsInBoard, player, xIndicesMove, yIndicesMove);
		// check if there are no possible moves
		if (numJumps == 0 && numMoves1 == 0) 
		{
			if (player == WHITEPLAYER) // if white is out of moves, red wins
			{
				cout << "White is unable to move.\nGAME OVER, Red has won.\nEnter any Character to close the game.\n";
				exit(0);
			}
			if (player == REDPLAYER) // if red is out of moves, white wins
			{
				cout << "Red is unable to move.\nGAME OVER, White has won.\nEnter any Character to close the game.\n";
				exit(1);
			}
		}
		//outputs whos turn it is
		if (player == WHITEPLAYER)
		{
			cout << "White takes a turn.\n";
		}
		else
		{
			cout << "Red takes a turn.\n";
		}
		// promt for and read originalSquare (the square the player wants to move)
		cout << "Enter the square number of the checker you want to move\n";
		while (1) 
		{
			cin >> originalSquare; //reads input for square the player wants to move
			//checks if input is not an integer
			if (cin.fail())
			{
				cout << "ERROR: You did not enter an integer\n";
				cout << "Try again\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			//checks if the square is not on the board
			if (originalSquare < 0 || originalSquare >= numRowsInBoard * numRowsInBoard)
			{
				cout << "ERROR: That square is not on the board.\n";
				cout << "Try again\n";
				continue;
			}
			//checks if the square is empty
			int xPosition = originalSquare / numRowsInBoard;
			int yPosition = originalSquare % numRowsInBoard;
			if (myCMCheckersBoard[xPosition][yPosition] == 0)
			{
				cout << "ERROR: That square is empty.\n";
				cout << "Try again\n";
				continue;
			}
			//checkers if the square contains an opponent's checker
			if (((myCMCheckersBoard[xPosition][yPosition] == WHITEKING || myCMCheckersBoard[xPosition][yPosition] == WHITEMULE || myCMCheckersBoard[xPosition][yPosition] == WHITESOLDIER) && player == REDPLAYER) 
				||(myCMCheckersBoard[xPosition][yPosition] == REDKING || myCMCheckersBoard[xPosition][yPosition] == REDMULE || myCMCheckersBoard[xPosition][yPosition] == REDSOLDIER) && player == WHITEPLAYER )
			{
				cout << "ERROR: That square contains an opponent's checker.\n";
				cout << "Try again\n";
				continue;
			}
			//if the chosen checker cannot jump an opponents checker but there is another of your checkers that can jump an opponents checker, then you cannot move the chosen checker
			if (numJumps > 0 && !CheckList(xIndicesJump, yIndicesJump, xPosition, yPosition))
			{
				cout << "ERROR: You can jump with another checker, you may not move your chosen checker.\n";
				cout << "You can jump using checkers on the following squares:";
				for (int i = 0; i < numJumps; ++i) 
				{
					cout << xIndicesJump[i] * numRowsInBoard + yIndicesJump[i] << " "; //prints the position
				}
				cout << "\n";
				cout << "Try again\n";
				continue;
			}
			//checks if there are no jumps and no moves using checklist
			if (!CheckList(xIndicesJump, yIndicesJump, xPosition, yPosition) && !CheckList(xIndicesMove, yIndicesMove, xPosition, yPosition))
			{
				cout << "ERROR: There is no legal move for this checker.\n";
				cout << "Try again\n";
				continue;
			}
			break;
		}
		// prompt and read moveTo square
		cout << "Enter square number of the square you want to move your checker to\n";
		bool jumped;
		while (true)
		{
			cin >> moveToSquare;
			//check if the input is not an integer
			if (cin.fail())
			{
				cout << "ERROR: You did not enter an integer\n";
				cout << "Try again\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			//check if the inputed square is not on the board
			if (moveToSquare < 0 || moveToSquare >= numRowsInBoard * numRowsInBoard) {
				cout << "ERROR: It is not possible to move to a square that is not on the board.\n";
				cout << "Try again\n";
				continue;
			}
			//check if the square chosen contains an opponent's checker
			int xPositionMove = moveToSquare / numRowsInBoard;
			int yPositionMove = moveToSquare % numRowsInBoard;
			if (myCMCheckersBoard[xPositionMove][yPositionMove] != 0)
			{
				cout << "ERROR: It is not possible to move to a square that is already occupied.\n";
				cout << "Try again\n";
				continue;
			}
			//Check if you can make a move with the checker, if so they must make the jump
			int xPosition = originalSquare / numRowsInBoard;
			int yPosition = originalSquare % numRowsInBoard;
			if ((abs(xPosition - xPositionMove) + abs(yPosition - yPositionMove) == 2) && (CheckList(xIndicesJump, yIndicesJump, xPosition, yPosition)))
			{
				cout << "ERROR: You can jump with this checker, you must jump not move 1 space.\n";
				cout << "Try again\n";
				continue;
			}
			//make the players move using the function MakeMove, store it in the boolean variable trueMove
			bool trueMove = MakeMove(myCMCheckersBoard, numRowsInBoard, player, originalSquare, moveToSquare, jumped);
			//if trueMove is false then moving to that square is illegal
			if (!trueMove)
			{
				cout << "ERROR: Moving to that square is not legal, Try again.\n";
				cout << "Enter square number of the square you want to move your checker to\n";
				continue;
			}
			//print the board using the display board function
			DisplayBoard(myCMCheckersBoard, numRowsInBoard);
			//check if another jump is possible: if you jumped when you completed the first part of your move
			//the value of jumped returned from makeMove will be true, if jumped is true you must check to see if you can jump again as part of this move
			bool anotherJump = jumped && IsJump(myCMCheckersBoard, numRowsInBoard, player, xPositionMove, yPositionMove);
			if (!anotherJump)
			{
				break; //if there isnt another jump break the moveto square loop
			}
			cout << "You can jump again, Please enter the next square you wish to move your checker to\n";
			originalSquare = moveToSquare;
		}
		// Check if a player has won at the end of their turn using the CheckWin function
		if (CheckWin(myCMCheckersBoard, numRowsInBoard)) 
		{
			cout << "Enter any character to terminate the game then press the enter key\n";
			break;
		}

		// if CheckWin returned false start the next players turn
		if (player == WHITEPLAYER)
		{
			player = REDPLAYER;
		}
		else player = WHITEPLAYER;
	}
	
	return 0;
}

void InitializeBoard(int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	//setup the starting position of the board
	//initialize white soldiers on every second line
	for (int r = 1; r < (numRowsInBoard / 2 - 1); r += 2)
	{
		for (int c = 0; c < (numRowsInBoard - 1); c += 2)
		{
			myCMCheckersBoard[r][c] = WHITESOLDIER;
		}
	}
	//initialize white soldiers on every first line
	for (int r = 0; r < (numRowsInBoard / 2 - 1); r += 2)
	{
		for (int c = 1; c <= (numRowsInBoard - 1); c += 2)
		{
			myCMCheckersBoard[r][c] = WHITESOLDIER;
		}
	}

	//initialize white mules on first line
	for (int c = 1; c <= (numRowsInBoard - 1); c += 2)
	{
		myCMCheckersBoard[0][c] = WHITEMULE;
	}
	

	//intialize red soldiers on every second line
	for (int r = (numRowsInBoard - 2); r > (numRowsInBoard / 2 ); r -= 2) 
	{
		for (int c = 1; c <= (numRowsInBoard - 1); c += 2) 
		{
			myCMCheckersBoard[r][c] = 4;
		}
	}
	//initialize red soldiers on every first line
	for (int r = (numRowsInBoard - 1); r > (numRowsInBoard / 2); r -= 2) 
	{
		for (int c = 0; c <= (numRowsInBoard - 1); c += 2)
		{
			myCMCheckersBoard[r][c] = 4;
		}
	}
	//initialize red mules
	for (int c = 0; c <= (numRowsInBoard - 1); c += 2)
	{
		myCMCheckersBoard[numRowsInBoard - 1][c] = REDMULE;
	}
}
void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	//display the current position of the values in the board
	for (int r = 0; r < numRowsInBoard; ++r)
	{
		cout << "  ";
		for (int c = 0; c < numRowsInBoard; ++c) 
		{
			if (CMCheckersBoard[r][c]) 
			{
				int valueInPosition = CMCheckersBoard[r][c];
				if (valueInPosition == WHITESOLDIER)
				{
					cout << setw(3) << "WS";
				}
				else if (valueInPosition == WHITEMULE)
				{
					cout << setw(3) << "WM";
				}
				else if (valueInPosition == WHITEKING)
				{
					cout << setw(3) << "WK";
				}
				else if (valueInPosition == REDSOLDIER)
				{
					cout << setw(3) << "RS";
				}
				else if (valueInPosition == REDMULE)
				{
					cout << setw(3) << "RM";
				}
				else if (valueInPosition == REDKING)
				{
					cout << setw(3) << "RK";
				}
			}
			else 
			{
				cout << setw(3) << (r*numRowsInBoard) + c; //output the square number
			}
			if ((c + 1) == numRowsInBoard)
			{
				cout << endl; //new line if at the end of the line
			}
			else
			{
				cout << " "; //space if not at the end of the line
			}
		}
	}
	cout << "\n\n\n";
}
int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]) 
{		
	memset(xLocArray, -1, MAX_PIECES * sizeof(int)); //initialize xLocArray to -1
	memset(yLocArray, -1, MAX_PIECES * sizeof(int)); //initialize yLocArray to -1
	int numJumps = 0; //initialize numJumps to 0, this will be returned from the function at the end
	for (int i = 0; i < numRowsInBoard; ++i) 
	{
		for (int j = 0; j < numRowsInBoard; ++j)
		{
			if (CMCheckersBoard[i][j] != 0) //One by one each of the squares on the board are examined. For each square containing one of the player's checkers:
			{
				//the function IsJump is called to determine if the player's checker on square CMCheckersBoard[yindex][xIndex] can jump an opponents piece. If it can:
				if (IsJump(CMCheckersBoard, numRowsInBoard, player, i, j))
				{
					xLocArray[numJumps] = i; //the values of yIndex is placed in the next unused location of the array yLocArray
					yLocArray[numJumps] = j; //the value of xIndex is placed in the next unused location of the array xLocArray
					++numJumps; //The counter recording the number of the player's checkers that can jump is incremented
				}
			}
		}
	}

	return numJumps; //the function CountJumps returns an integer which contains the number of the player's checkers that can jump an opponent's checker.
}

bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER) //check if the player is white
	{
		//if the piece in question is not a white piece return false
		if (!(CMCheckersBoard[xLoc][yLoc] == WHITEKING || CMCheckersBoard[xLoc][yLoc] == WHITEMULE || CMCheckersBoard[xLoc][yLoc] == WHITESOLDIER))
		{
			return false;
		}
		//if the piece one move forward and to the right is an enemy piece:
		if (CMCheckersBoard[xLoc + 1][yLoc + 1] == REDKING || CMCheckersBoard[xLoc + 1][yLoc + 1] == REDMULE || CMCheckersBoard[xLoc + 1][yLoc + 1] == REDSOLDIER)
		{
			//check if the piece two spaces forward and to the right is a zero, if it is return true
			if (CMCheckersBoard[xLoc + 2][yLoc + 2] == 0)
			{
				if (xLoc + 2 >= numRowsInBoard) return false;
				return true;
			}
		}
		//if the piece one move forward and to the left is an enemy piece:
		if (CMCheckersBoard[xLoc + 1][yLoc - 1] == REDKING || CMCheckersBoard[xLoc + 1][yLoc - 1] == REDMULE || CMCheckersBoard[xLoc + 1][yLoc - 1] == REDSOLDIER)
		{
			//check if the piece two spaces forward and to the left is a zero, if it is return true 
			if (CMCheckersBoard[xLoc + 2][yLoc - 2] == 0)
			{
				if (xLoc + 2 >= numRowsInBoard) return false;
				return true;
			}
		}
		//check if the piece is a white king
		if (CMCheckersBoard[xLoc][yLoc] == WHITEKING)
		{
			//check if the piece back one and to the right is an enemy piece
			if (CMCheckersBoard[xLoc - 1][yLoc + 1] == REDKING || CMCheckersBoard[xLoc - 1][yLoc + 1] == REDMULE || CMCheckersBoard[xLoc - 1][yLoc + 1] == REDSOLDIER)
			{
				//check if the piece back two and to the right is empty, if it is return true
				if (CMCheckersBoard[xLoc - 2][yLoc + 2] == 0)
				{
					if (xLoc - 2 < 0) return false;
					return true;
				}
			}
			//check if the piece back one and to the left is an enemy piece
			if (CMCheckersBoard[xLoc - 1][yLoc - 1] == REDKING || CMCheckersBoard[xLoc - 1][yLoc - 1] == REDMULE || CMCheckersBoard[xLoc - 1][yLoc - 1] == REDSOLDIER)
			{
				//check if the piece back two and to the left is empty, if it is return true
				if (CMCheckersBoard[xLoc - 2][yLoc - 2] == 0)
				{
					if (xLoc - 2 < 0) return false;
					return true;
				}
			}
		}
	}
	//check if the player is red
	if (player == REDPLAYER)
	{
		//check if the piece being checked is not one of the current players pieces
		if (!(CMCheckersBoard[xLoc][yLoc] == REDKING || CMCheckersBoard[xLoc][yLoc] == REDMULE || CMCheckersBoard[xLoc][yLoc] == REDSOLDIER))
		{
			return false;
		}
		//check if the piece one forward and to the right is an enemy piece
		if (CMCheckersBoard[xLoc - 1][yLoc + 1] == WHITEKING || CMCheckersBoard[xLoc - 1][yLoc + 1] == WHITEMULE || CMCheckersBoard[xLoc - 1][yLoc + 1] == WHITESOLDIER)
		{
			//check if the piece two forward and to the right is empty
			if (CMCheckersBoard[xLoc - 2][yLoc + 2] == 0)
			{
				if (xLoc - 2 < 0) return false;
				return true;
			}
		}
		//check if the piece one forward and to the left is an enemy piece
		if (CMCheckersBoard[xLoc - 1][yLoc - 1] == WHITEKING || CMCheckersBoard[xLoc - 1][yLoc - 1] == WHITEMULE || CMCheckersBoard[xLoc - 1][yLoc - 1] == WHITESOLDIER)
		{
			//check if the piece two forward and to the left is empty
			if (CMCheckersBoard[xLoc - 2][yLoc - 2] == 0)
			{
				if (xLoc - 2 < 0) return false;
				return true;
			}
		}
		//check if the piece is a red king
		if (CMCheckersBoard[xLoc][yLoc] == REDKING)
		{
			//check if the piece one backwards and to the right is an enemy piece
			if (CMCheckersBoard[xLoc + 1][yLoc + 1] == WHITEKING || CMCheckersBoard[xLoc + 1][yLoc + 1] == WHITEMULE || CMCheckersBoard[xLoc + 1][yLoc + 1] == WHITESOLDIER)
			{
				//check if the piece two backwards and to the right is empty
				if (CMCheckersBoard[xLoc + 2][yLoc + 2] == 0)
				{
					if (xLoc + 2 >= numRowsInBoard) return false;
					return true;
				}
			}
			//check if the piece one backwards and to the left is an enemy piece
			if (CMCheckersBoard[xLoc + 1][yLoc - 1] == WHITEKING || CMCheckersBoard[xLoc + 1][yLoc - 1] == WHITEMULE || CMCheckersBoard[xLoc + 1][yLoc - 1] == WHITESOLDIER)
			{
				//check if the piece two backwards and to the left is an enemy piece
				if (CMCheckersBoard[xLoc + 2][yLoc - 2] == 0)
				{
					if (xLoc + 2 >= numRowsInBoard) return false;
					return true;
				}
			}
		}
	}
	return false;
}

int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
	memset(xLocArray, -1, MAX_PIECES * sizeof(int)); //xLocArray has its elements initialized to -1
	memset(yLocArray, -1, MAX_PIECES * sizeof(int)); //yLocArray has its elements initialized to -1
	int numMoves1 = 0; //initialize numMoves1 to 0, this will be returned from the function at the end

	for (int i = 0; i < numRowsInBoard; ++i)
	{
		for (int j = 0; j < numRowsInBoard; ++j)
		{
			if (CMCheckersBoard[i][j] != 0) //One by one each of the squares on the board are examined. For each square containing one of the player's checkers:
			{
				//the function IsMove1Square is called to determine if the player's checker on square CMCheckersBoard[yindex][xIndex] has a possible move hat does not involve jumping over an opponent's piece. If it does:
				if (IsMove1Square(CMCheckersBoard, numRowsInBoard, player, i, j))
				{
					xLocArray[numMoves1] = i; //xIndex is placed in the next unused location of array xLocArray
					yLocArray[numMoves1] = j; // yIndex is placed in the next unused element of array yLocArray
					++numMoves1; // the counter recording number of checkers that can be moved one square is updated
				}
			}
		}
	}

	return numMoves1;
}

bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex)
{
	for (int j = 0; inArray1[j] != -1; ++j)
	{
		if ((inArray1[j] == xIndex) && (inArray2[j] == yIndex))
		{
			return true;
		}
	}
	return false;
}

bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped)
{
	//find the coordinates of the original and moveTo Squares
	int xOfOriginalSquare = fromSquareNum / numRowsInBoard;
	int yOfOriginalSquare = fromSquareNum % numRowsInBoard;
	int xOfMoveToSquare = toSquareNum / numRowsInBoard;
	int yOfMoveToSquare = toSquareNum % numRowsInBoard;

	//definte the xDistance as the absolute value of the difference between the x of the orginal and the moveto square
	int xDistance = abs(xOfOriginalSquare - xOfMoveToSquare);
	//define the yDistance as the absolute value of the difference between the y of the original and the moveto square
	int yDistance = abs(yOfOriginalSquare - yOfMoveToSquare);

	//Initiate the jumped squares coordinates
	int xOfJumpedSquare = -1;
	int yOfJumpedSquare = -1;

	// Use the xdistance and the ydistance to determine if the checker has moved diagonally either 1 or two squares. 
	if (!((xDistance == 1 && yDistance == 1) || (xDistance == 2 && yDistance == 2)))
	{
		cout << "Error: illegal move\n";
		return false;
	}

	// If the checker is xdistance=ydistance=2, check if there is no opponent’s checker in the square between the original and the moveTo square
	if (xDistance == 2 && yDistance == 2)
	{
		//find coordinates of the jumped square
		xOfJumpedSquare = (xOfOriginalSquare + xOfMoveToSquare) >> 1;
		yOfJumpedSquare = (yOfOriginalSquare + yOfMoveToSquare) >> 1;
		int jumpedSquare = CMCheckersBoard[xOfJumpedSquare][yOfJumpedSquare];
		//if the jumped square is empty or it contains one of the current players pieces
		if (jumpedSquare == 0 || ((jumpedSquare == WHITEKING || jumpedSquare == WHITEMULE || jumpedSquare == WHITESOLDIER) && player == WHITEPLAYER)
			|| ((jumpedSquare == REDKING || jumpedSquare == REDMULE || jumpedSquare == REDSOLDIER) && player == REDPLAYER))
		{
			cout << "Error: illegal move\n";
			return false;
		}
		jumped = true;
	}
	// Move the value of the original square to the moveToSquare
	CMCheckersBoard[xOfMoveToSquare][yOfMoveToSquare] = CMCheckersBoard[xOfOriginalSquare][yOfOriginalSquare];
	CMCheckersBoard[xOfOriginalSquare][yOfOriginalSquare] = 0; //the original square is now empty
	if (jumped)
	{
		//if a jump occured the jumped square is now empty
		CMCheckersBoard[xOfJumpedSquare][yOfJumpedSquare] = 0;
	}
	// if a soldier or mule becomes a king
	int kingChecker = CMCheckersBoard[xOfMoveToSquare][yOfMoveToSquare];
	if (player == REDPLAYER && kingChecker == REDMULE && xOfMoveToSquare == 0)
	{
		CMCheckersBoard[xOfMoveToSquare][yOfMoveToSquare] = REDKING;
		cout << "Red has created a Mule King,  White wins the game\n";
		cout << "Enter any character to terminate the game and then press the enter key";
		int closeCharacter;
		cin >> closeCharacter;
		exit(2);
	}
	else if (player == WHITEPLAYER && kingChecker == WHITEMULE && xOfMoveToSquare == numRowsInBoard - 1)
	{
		CMCheckersBoard[xOfMoveToSquare][yOfMoveToSquare] = WHITEKING;
		cout << "White has created a Mule King, Red wins the game\n";
		cout << "Enter any character to terminate the game and then press the enter key";
		int closeCharacter;
		cin >> closeCharacter;
		exit(3);
	}
	else if (player == REDPLAYER && kingChecker == REDSOLDIER && xOfMoveToSquare == 0)
	{
		CMCheckersBoard[xOfMoveToSquare][yOfMoveToSquare] = REDKING; //create a red king
	}
	else if (player == WHITEPLAYER && kingChecker == WHITESOLDIER && xOfMoveToSquare == numRowsInBoard - 1)
	{
		CMCheckersBoard[xOfMoveToSquare][yOfMoveToSquare] = WHITEKING; //create a white king
	}
	return true;
}

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER) //check if the player is white
	{
		//check if the piece being checked is not one of the current players pieces
		if (!(CMCheckersBoard[xLoc][yLoc] == WHITEKING || CMCheckersBoard[xLoc][yLoc] == WHITEMULE || CMCheckersBoard[xLoc][yLoc] == WHITESOLDIER))
		{
			return false;
		}
		//check if the square 1 forward and right is empty
		if (CMCheckersBoard[xLoc + 1][yLoc + 1] == 0)
		{
			if (xLoc + 1 >= numRowsInBoard) return false;
			return true;
		}
		//check if the square 1 forward and to the left is empty
		if (CMCheckersBoard[xLoc + 1][yLoc - 1] == 0)
		{
			if (xLoc + 1 >= numRowsInBoard) return false;
			return true;
		}
		//check if the piece is a king
		if (CMCheckersBoard[xLoc][yLoc] == WHITEKING)
		{
			//check if the square 1 backwards and right is empty
			if (CMCheckersBoard[xLoc - 1][yLoc + 1] == 0)
			{
				if (xLoc - 1 < 0) return false;
				return true;
			}
			//check if the square 1 backward and to the left is empty
			if (CMCheckersBoard[xLoc - 1][yLoc - 1] == 0)
			{
				if (xLoc - 1 < 0) return false;
				return true;
			}
		}
	}
	if (player == REDPLAYER) //check if the player is white
	{
		//check if the piece being checked is not one of the current players pieces
		if (!(CMCheckersBoard[xLoc][yLoc] == REDKING || CMCheckersBoard[xLoc][yLoc] == REDMULE || CMCheckersBoard[xLoc][yLoc] == REDSOLDIER))
		{
			return false;
		}
		//check if the square 1 forward and right is empty
		if (CMCheckersBoard[xLoc - 1][yLoc + 1] == 0)
		{
			if (xLoc - 1 < 0) return false;
			return true;
		}
		//check if the square 1 forward and to the left is empty
		if (CMCheckersBoard[xLoc - 1][yLoc - 1] == 0)
		{
			if (xLoc - 1 < 0) return false;
			return true;
		}
		//check if the piece is a king
		if (CMCheckersBoard[xLoc][yLoc] == WHITEKING)
		{
			//check if the square 1 backwards and right is empty
			if (CMCheckersBoard[xLoc + 1][yLoc + 1] == 0)
			{
				if (xLoc + 1 >= numRowsInBoard) return false;
				return true;
			}
			//check if the square 1 backward and to the left is empty
			if (CMCheckersBoard[xLoc + 1][yLoc - 1] == 0)
			{
				if (xLoc + 1 >= numRowsInBoard) return false;
				return true;
			}
		}
	}
	return false;
}
bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	//CheckWin counts the number of each kind of checker on the board
	int count[100] = { 0 };
	for (int r = 0; r < numRowsInBoard; ++r)
	{
		for (int c = 0; c < numRowsInBoard; ++c)
		{
			++count[CMCheckersBoard[r][c]];
		}
	}
	//check if the red player wins by losing all of the white mules
	if (count[REDMULE] == 0)
	{
		cout << "The Red Player has won the game by losing all of the Red Mules\n";
		return true;
	}
	//check if the white player wins by losing all of the white mules
	if (count[WHITEMULE] == 0)
	{
		cout << "The White Player has won the game by losing all of the White Mules\n";
		return true;
	}
	//check if the white player wins by capturing all white's soldiers and kings
	if (count[REDSOLDIER] + count[REDKING] == 0)
	{
		cout << "The White Player has won by capturing all of the red players soldiers and kings\n";
		return true;
	}
	//check if the red player wins by capturing all white's soldiers and kings
	if (count[WHITESOLDIER] + count[WHITEKING] == 0) 
	{
		cout << "The Red Player has won by capturing all of the white players soldiers and kings\n";
		return true;
	}

	return false; //if none of the conditions are true, return false
}