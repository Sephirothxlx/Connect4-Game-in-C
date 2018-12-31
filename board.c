#include "board.h"
#include <stdlib.h>
#include <string.h>

/**
 * In this file, implement all of the functions for board.h
 *
 * Once done, make sure you pass all the tests in board_test.c
 * (make test_board)
 *
 */

struct board_t{
	player_t * board;
	unsigned int width;
	unsigned int height;
	unsigned int run;
};


/// Create a new empty board of the given game configuration.
///    width  = width of the board
///    height = height of the board
///    run    = number of pieces in a row to consider a win
///
/// Returns a new board pointer in *b
/// 
/// If initial is not NULL, then it must point to a (one-dimensional)
/// array of width*height player_t elements containing the initial
/// board configuration, ROW BY ROW, starting with the bottom ROW
/// (i.e. lowest row number, bottom of board),
/// LEFT-TO-RIGHT (i.e. incrementing column numbers).
/// 
/// The board is numbered as follows:
///
/// ROW
///  ...
///  3
///  2
///  1
///  0
///    0 1 2 3 4 COLUMN
/// 
/// In other words, the first row a piece is placed in is row 0
/// And the lower left corner of the board is row 0, column 0
///
/// When initial == NULL, the board should be initialized to be empty.
bool board_create (board_t ** b, unsigned int height, unsigned int width, unsigned int run, const player_t * initial){
	if(b==NULL)
		return false;
	(*b)=(board_t*)malloc(sizeof(board_t));
	//there are two cases
	//at this case, we need to set every position to the same value as initial 
	if(initial!=NULL){
		(*b)->board=(player_t*)malloc(sizeof(player_t)*width*height);
		memcpy((*b)->board,initial,width*height);
		(*b)->width=width;
		(*b)->height=height;
		(*b)->run=run;
	}else{
	//at this case, set them to empty
		(*b)->width=width;
		(*b)->height=height;
		(*b)->run=run;
		(*b)->board=(player_t*)malloc(sizeof(player_t)*width*height);
		memset((*b)->board,PLAYER_EMPTY,width*height);
	}
	return true;
}

/// Destroy the given board
bool board_destroy (board_t * b){
	if(b==NULL)
		return false;
	//free board
	free(b->board);
	free(b);
	return true;
}

/// Return the height of the board
unsigned int board_get_height (const board_t * b){
	return b->height;
}

/// Return the width of the board
unsigned int board_get_width (const board_t * b){
	return b->width;
}	

/// Return how many pieces need to be in a straight line before the game 
/// is considered to be won. (straight line: horizontal, diagonal or vertical)
unsigned int board_get_run (const board_t * b){
	return b->run;
}

/// Return true if the given move is valid, false otherwise
/// If true, the board was changed and the move was made.
/// If false, the board remains the same.
bool board_play (board_t * b, unsigned int column, player_t player){
	if(b==NULL)
		return false;
	if(player==PLAYER_EMPTY)
		return false;
	//find the place to put player
	for(int i=0;i<b->height;i++){
		if(*(b->board+i*b->width+column)==PLAYER_EMPTY){
			*(b->board+i*b->width+column)=player;
			return true;
		}
	}
	return false;
}

/// Return true if the specified player can make a move on the
/// board. THERE IS NO NEED TO KEEP TRACK OF THE TURN OR TO VALIDATE THIS.
/// The board is not changed.
bool board_can_play (const board_t * b, player_t p){
	//check if there is an empty place in the whole board
	for(int i=b->height-1;i>=0;i--){
		for(int j=0;j<b->width;j++){
			if(*(b->board+i*b->width+j)==PLAYER_EMPTY)
				return true;
		}
	}
	return false;
}

/// Return true if the specified move is valid
/// The board is not changed.
bool board_can_play_move (const board_t * b, player_t p, unsigned int column){
	//column needs to be smaller than width
	if(column>=b->width)
		return false;
	//check if there is an empty place at the specified column
	for(int i=0;i<b->height;i++){
		if(*(b->board+i*(b->width)+column)==PLAYER_EMPTY){
			return true;
		}
	}
	return false;
}

/// Undo a move (i.e. make the specified board slot empty)
/// Return true if OK, false on error
/// If the function returns true, the board has been modified.
/// If the function returns false, the board has not been modified.
/// There is no need to keep track of the which player last played.
bool board_unplay (board_t * b, unsigned int column){
	if(b==NULL)
		return false;
	if(column>=b->width)
		return false;
	for(int i=b->height-1;i>=0;i--){
	//find the "highest" position
		if(*(b->board+i*(b->width)+column)!=PLAYER_EMPTY){
			*(b->board+i*(b->width)+column)=PLAYER_EMPTY;
			return true;
		}
	}
	return false;
}

/// Duplicate a board
//      return true on success; New board needs to be freed 
///     with board_destroy.
///
/// Return true on success, false on error
bool board_duplicate (board_t ** newboard, const board_t * old){
	if(newboard==NULL)
		return false;
	//call create to duplicate
	if(board_create(newboard,old->height,old->width,old->run,old->board))
		return true;
	return false;
}

//this function is used to check if there is a "run"
//use bit operation
bool isRun(unsigned int a, unsigned int run, unsigned int b){
	unsigned int c=1<<b;
	unsigned int temp=(1<<run)-1;
	while(c>=temp){
		if((a&temp)==temp)
			return true;
		temp<<=1;
	}
	return false;
}
/// Return -1 if tie, 1 if winner, 0 if no winner
/// If there is a winner, set player to the winning color.
///
/// no winner means that the game can still continue (i.e. open positions)
/// winner means somebody has N tokens in a straight line.
/// tie means there are no more valid moves and there is no winner.
///
/// You can assume that only one of the above conditions is true.
///
int board_has_winner (const board_t * b, player_t * player){
	//create arrays to store the result
	//there are two players
	//I want to use bit operation to check if there is a winner
	//This algorithm will only cost O(n)
	unsigned int h_blue[b->height];
	unsigned int h_yellow[b->height];
	unsigned int v_blue[b->width];
	unsigned int v_yellow[b->width];
	unsigned int dlr_blue[b->height+b->width-1];
	unsigned int dlr_yellow[b->height+b->width-1];
	unsigned int drl_blue[b->height+b->width-1];
	unsigned int drl_yellow[b->height+b->width-1];

	//initialization
	for(int i=0;i<b->height;i++){
		h_blue[i]=0;
		h_yellow[i]=0;
	}

	for(int i=0;i<b->width;i++){
		v_blue[i]=0;
		v_yellow[i]=0;
	}

	for(int i=0;i<b->height+b->width-1;i++){
		dlr_blue[i]=0;
		dlr_yellow[i]=0;
		drl_blue[i]=0;
		drl_yellow[i]=0;
	}
	//count the result
	unsigned int x=0;
	for(int i=0;i<b->height;i++){
		for(int j=0;j<b->width;j++){
		//set the position to 1
			if(*(b->board+i*(b->width)+j)==PLAYER_BLUE){
				h_blue[i]+=1<<j;
				v_blue[j]+=1<<i;
				dlr_blue[i+j]+=1<<x;
				drl_blue[b->width-j-1+i]+=1<<x;
			}else if(*(b->board+i*(b->width)+j)==PLAYER_YELLOW){
				h_yellow[i]+=1<<j;
				v_yellow[j]+=1<<i;
				dlr_yellow[i+j]+=1<<x;
				drl_yellow[b->width-j-1+i]+=1<<x;
			}
		}
		x++;
	}
	//find the winner
	//check every result stored in arrays
	int res1=0,res2=0;
	for(int i=0;i<b->height;i++){
		if(res1!=1&&isRun(h_blue[i],b->run,b->width))
			res1=1;
		if(res2!=1&&isRun(h_yellow[i],b->run,b->width))
			res2=1;
	}
	//find the winner
	//check every result stored in arrays
	for(int i=0;i<b->width;i++){
		if(res1!=1&&isRun(v_blue[i],b->run,b->height))
			res1=1;
		if(res2!=1&&isRun(v_yellow[i],b->run,b->height))
			res2=1;
	}
	//find the winner
	//check every result stored in arrays
	unsigned int l=b->width>b->height?b->width:b->height;
	for(int i=0;i<b->height+b->width-1;i++){
		if(res1!=1&&(isRun(drl_blue[i],b->run,l)||isRun(dlr_blue[i],b->run,l)))
			res1=1;
		if(res2!=1&&(isRun(drl_yellow[i],b->run,l)||isRun(dlr_yellow[i],b->run,l)))
			res2=1;
	}
	//return -1,0,1 according to res1 and res2
	if(res1==1&&res2==1)
		return -1;
	else if(res1==1||res2==1){
		*player=res1==1?PLAYER_BLUE:PLAYER_YELLOW;
		return 1;
	}else{
		//if someone can stll play
		if(board_can_play(b,PLAYER_BLUE)||board_can_play(b,PLAYER_YELLOW))
			return 0;
		else
		//if no one can play
			return -1;
	}
}

/// Return game piece at specified position
/// Returns false if the position is not valid on the board,
/// returns true otherwise and copies the piece at that location on the board
/// to *piece
bool board_get (const board_t * b, unsigned int row, unsigned int column, player_t * piece){
	//the position is invalid
	if(row>=b->height||column>=b->width)
		return false;
	*piece=*(b->board+row*(b->width)+column);
	return true;
}


/// Clear the board
/// Return true if success, false otherwise
bool board_clear (board_t * b){
	if(b==NULL)
		return false;
	unsigned int length=b->height*b->width;
	//set every position to empty
	for(int i=0;i<length;i++){
		*(b->board+i)=PLAYER_EMPTY;
	}
	return true;
}

/// Write the board to the given file (for visual aid for the player)
/// Return true if OK, false on error
bool board_print (const board_t * b, FILE * f){
	if(b==NULL)
		return false;
	if(f==NULL)
		return false;
	//print the board like this:
	// ROW
	//  ...
	//  3
	//  2
	//  1
	//  0
	//    0 1 2 3 4 COLUMN
	for(int i=b->height-1;i>=0;i--){
		for(int j=0;j<b->width;j++){
			char x=(*(b->board+i*b->width+j))+'0';
			fputc(x,f);
			if(j!=b->width-1)
				fputc(' ',f);
		}
		fputc('\n',f);
	}
	return true;
}

