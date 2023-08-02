#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs
struct player {
       int row;
       int col;
};

struct ball {
       int row;
       int col;
       int rd;
       int cd;
       int size;
       unsigned short color;
};

struct state {
       int size;
       struct player players;
       struct ball balls;
};



/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

#endif
