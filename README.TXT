Connect 4 game in C

Get started!
You can compile board.c, then run it. The game mode and operation is very simple and clear!

note: if you want to try computer agent, the size of the board should be small, otherwise the algorithm will cost you too much time.

Agent introduction
1. Random Agent
The random agent will only find some place to put the chess piece randomly.

2. Computer Agent
Every time the program calls computer_agent_play, the program will do the following steps:
    1. duplicate a new board (we need to change the content of the board)
    2. from 0 to width-1, we check every possible position for play
        if the column is valid, call getGoodMove
        if it is not, continue
    3. getGoodMove will return a value representing the value (-100, 0, 100 ) of the subtrees. And we will find the biggest one and decide the move.
The computer_agent_play will call getGoodMove which is a recursive function. 
This function will check every possible column and call getGoodMove, then call unplay to restore the state.
After we get the value of every subtree, we choose the biggest one if the color of current step is the same as the color, which we pass to computer_agent_color.
And we choose the smallest one if the color of current step is different from the color that we pass to computer_agent_color. (based on minimax algorithm)
I use another variable "deepth" to record the length of paths (from root node to every leaf node) in subtrees.
And we only need the biggest deepth in all the subtrees.
for example, this is a tree:

            x
           / \
          x   x
         / \   
        x   x   
       /
      x
      
Obviously, the longest path is the left-most path. If we pick this path, the game will last for the longest time. (3 times play)
Every time we call getGoodMove, the function will update the longest path of a node (from the node to a leaf node).
The longest path means that the game will last for longer time, so opponent may make a mistake.
Hence, if all the values in subtree are the same, we choose the longest one. 
What we need to do is to update the deepth (in recursion function) and always choose the biggest one.
