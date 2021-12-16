/*
* NAME: dice.h
* AUTHOR: Adin Geist
* DESCRIPTION: Header file for dice.c
*/

/*Declaration of global variables*/
extern int dice[5]; // 5 dice that that player uses and sees
extern int holdDice[5]; // 5 slots that indicate if a die at an index is held using flag values

// Function prototypes that are defined inside of dice.c
int addAllDice();
int rollDie(void);
void rollDice(void);
void printDice();
void holdDie(int index);