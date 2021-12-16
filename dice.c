/*
* NAME: dice.c
* AUTHOR: Adin Geist
* DESCRIPTION: Handles all dice printing and login with functions
*/

// Include the prototypes inside the header file
#include "dice.h"

// Define colors
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define RESET_COLOR "\x1b[0m"


// Funciton that returns the sum of all dice in the dice array
int addAllDice() {
    int total = 0;
    int diceIndex;
    // get dice total of single number
    for (diceIndex = 0; diceIndex < 5; diceIndex++) {
        total += dice[diceIndex];
    }
    return total;
}

// Return a random number 1-6 like a dice would roll
int rollDie() {
    return ((rand() % 6) + 1);
}

// Roll all the dice that aren't indicated to be held by the player
void rollDice() {
    // Loop through the dice array
    for (int i = 0; i < 5; i++)
    { // Roll the dice if it isn't held/locked
        if (holdDice[i] == 0)
            dice[i] = rollDie();
    }
}

// Print out the dice in the dice array onto the console
void printDice() {
    // Create an array storing the faces of all 6 die to be printed
    char PRINT_DICE_ARRARY[6][5][12] = { // 6 dice, 5 rows of text, size 12 character arrays (size of line + '\0')
        {
            "###########",
            "#         #",
            "#    o    #",
            "#         #",
            "###########",
        },
        {
            "###########",
            "# o       #",
            "#         #",
            "#       o #",
            "###########",
        },
        {
            "###########",
            "# o       #",
            "#    o    #",
            "#       o #",
            "###########",
        },
        {
            "###########",
            "# o     o #",
            "#         #",
            "# o     o #",
            "###########",
        },
        {
            "###########",
            "# o     o #",
            "#    o    #",
            "# o     o #",
            "###########",
        },
        {
            "###########",
            "# o     o #",
            "# o     o #",
            "# o     o #",
            "###########",
        },
    };
    int diceI, row;

    printf("\n" YELLOW); // add 1 line of padding to the top
    // Print each row of the die
    for (row = 0; row < 5; row++) {
        // Print the next dice in this row with one space padding inbetween
        for (diceI = 0; diceI < 5; diceI++) {
            printf("%s ", PRINT_DICE_ARRARY[dice[diceI] - 1][row]);
        }
        printf("\n"); // Add 1 line padding to the bottom
    }
    printf(RESET_COLOR);


    // print whether each die is locked
    for (diceI = 0; diceI < 5; diceI++) {
        if (holdDice[diceI] != 0)
            printf(RED "     X      " RESET_COLOR);
        else
            printf("     %d      ", diceI + 1);
    }
    printf("\n\n"); // add 1 line of padding to the bottom
}

// Hold the dice in the holdDice array at the provided index
void holdDie(int index) {
    if (holdDice[index] == 0) {
        holdDice[index] = 1;
    }
}