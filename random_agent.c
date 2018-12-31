#include "random_agent.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * In this file, put your implementation of a 'random agent', i.e. a player
 * who will make a VALID but random move.
 */
typedef struct{
	char * name;
} random_agent_data_t;

static const char * random_agent_describe (agent_t * gthis){
//get its name
	random_agent_data_t * data = (random_agent_data_t *) gthis->data;
	return data->name;
}


static int random_agent_play (agent_t * gthis, const board_t * b, player_t color, unsigned int * move){
	const unsigned int width = board_get_width(b);
	unsigned int column=0;
	while (true){
		//find a random number in [0,width-1]
		column=rand()%width;
		//check if this number is valid
		if (!board_can_play_move(b, color, column)){
			continue;
		}
		*move=column;
		return 0;
	}
}

static bool random_agent_destroy (agent_t * this){
	random_agent_data_t * data = (random_agent_data_t *) this->data;
	free (data->name);
	free (this->data);
	return true;
}

agent_t * random_agent_create (const char * name){
	// Allocate vtable structure
	agent_t * n = malloc (sizeof(agent_t));

	//assign function
	n->destroy = random_agent_destroy;
	n->play = random_agent_play;
	n->describe = random_agent_describe;

	random_agent_data_t * data = malloc (sizeof(random_agent_data_t));
	n->data = data;
	
	//assign a name
	data->name =(char*)malloc(sizeof(char)*(13));
	strcpy(data->name,"Random Agent");

	return n;
}
