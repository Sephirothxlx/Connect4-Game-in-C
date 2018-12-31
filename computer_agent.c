#include "computer_agent.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
/*
 * In this file, implement your AI agent.
 *
 * Use the minimax algorithm to determine the next move.
 *
 * Look at player_agent.h and player_agent.c for an example of how to
 * implement each function.
 */

typedef struct{
	char * name;
} computer_agent_data_t;

//get the name of this agent
static const char * computer_agent_describe (agent_t * gthis){
	computer_agent_data_t * data = (computer_agent_data_t *) gthis->data;
	return data->name;
}

//get the best move based on the minimax algorithm
static int getGoodMove(board_t *b, player_t color,player_t nextcolor,int * d){
	unsigned int width=board_get_width(b);
	player_t winner=PLAYER_EMPTY;
	int temp=board_has_winner(b,&winner);
	
	//base case 
	//if the leaf node is tie
	if(temp==-1){
		*d=1;
		return 0;
	}else if(temp==1){
	//if the leaf node has a winner
		*d=1;
		return winner==color?100:-100;
	}
	//use deepth to record the longest path
	int deepth=0;
	int res=color==nextcolor?-101:101;
	player_t newc=nextcolor==PLAYER_YELLOW?PLAYER_BLUE:PLAYER_YELLOW;
	//check every possible column
	for(int i=0;i<width;i++){
		if (!board_can_play_move(b, nextcolor, i)){
			continue;
		}
		//play at i
		board_play(b,i,nextcolor);
		deepth=0;
		//get the value recursively
		int temp=getGoodMove(b,color,newc,&deepth);
		//unplay
		board_unplay(b,i);
		if(color==nextcolor&&temp>res){
			res=temp;
			*d=deepth+1;
		}else if(color!=nextcolor&&temp<res){
		//according to different color
		//oppponent will choose the position that makes the player get smallest value
			res=temp;
			*d=deepth+1;
		}else if(temp==res){
		//update the length of longest path
			if(deepth+1>*d)
				*d=deepth+1;
		}
	}
	return res;
}

static int computer_agent_play (agent_t * gthis, const board_t * b, player_t color, unsigned int * move){
	const unsigned int width = board_get_width(b);
	unsigned int column=0;
	board_t * newb;
	board_duplicate(&newb,b);
	player_t newc=color==PLAYER_YELLOW?PLAYER_BLUE:PLAYER_YELLOW;
	//we need to find a max deepth if all the value is equally bad
	int value=-101, deepth=0, maxdeepth=-1;
	for(int i=0;i<width;i++){
		if (!board_can_play_move(newb, color, i)){
			continue;
		}
		//play
		board_play(newb,i,color);
		deepth=0;
		//get teh value
		int temp=getGoodMove(newb,color,newc,&deepth);
		//unplay
		board_unplay(newb,i);
		if(temp>value){
			value=temp;
			column=i;
			maxdeepth=deepth+1;
		}else if(temp==value){
		//calculate the deepth of the subtrees
			if(deepth+1>maxdeepth){
				maxdeepth=deepth+1;
				column=i;
			}
		}
	}
	board_destroy(newb);
	*move=column;
	return 0;
}

static bool computer_agent_destroy (agent_t * this){
	computer_agent_data_t * data = (computer_agent_data_t *) this->data;
	//destroy the name
	free (data->name);
	free (this->data);
	return true;
}

agent_t * computer_agent_create (const char * name){
	// Allocate vtable structure
	agent_t * n = malloc (sizeof(agent_t));
	
	//assign the function
	n->destroy = computer_agent_destroy;
	n->play = computer_agent_play;
	n->describe = computer_agent_describe;

	computer_agent_data_t * data = malloc (sizeof(computer_agent_data_t));
	n->data = data;
	
	//assign the name
	data->name =(char*)malloc(sizeof(char)*(15));
	strcpy(data->name,"Computer Agent");

	return n;
}
