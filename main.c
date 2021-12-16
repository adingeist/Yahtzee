/*
* NAME: main.c
* AUTHOR: Adin Geist
* DESCRIPTION: Contains the main function for Yahtzee where a 
*              player can  play the console version of Yahzee
*/

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "dice.h" // Import dice functions and dice globals

#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"
#define RESET_COLOR "\x1b[0m"

/* Assembly Function */
void asm_print_banner(void);

/*Function Prototypes*/
void startNextSeries(void);
void welcomeMessage(void);
void printScorecard(void);
void playGame(void);
const void f(char str[3], int num);
int isOnlyDigits(const char* s);
void getAndProcessInput(void);
void updatePlayerTotals(void);
void endGame(void);
void highScoreScreen(void);
void printBanner(void); // Prints a yellow banner calling the assembly function

/*Global Variables*/
int dice[5]; // the 5 dice used to play - external variable from dice.h
int holdDice[5]; // tracks dice that shouldn't be rolled - external variable from dice.h
int turnsRolled = 0; // tracks the rolls a player takes

/*Player Struct*/
struct Player {
    char name[20];
    int ones, twos, threes, fours, fives, sixes, threeOfKind, fourOfKind, smStraight, lgStraight, fullHouse, yahtzee, chance, leftTotal, rightTotal, bonus, grandTotal;
} player1;

typedef struct HighScore { char name[20]; int score; } HighScore;

int main() {
    welcomeMessage(); // Print out the banner and obtain the player's name
    srand((unsigned int)time(0)); // Initialize random number generator with the current time

    // Continually ask for input until 'Q' Quit is given
    while (1) {
        system("cls"); // Clear the screen
        printBanner(); // Print the game banner
        printf(CYAN "------ MAIN MENU ------\n" RESET_COLOR); // Print the main menu header in cyan
        printf("(p) Play a game\n");
        printf("(v) View high scores\n");
        printf("(q) Quit.\n");

        char response = toupper(_getch()); // Ask the user for input and captialize it

        // Load the appropriate response
        if (response == 'P')
            playGame();
        else if (response == 'V')
            highScoreScreen();
        else if (response == 'Q')
            break;
    }
}

// Displays the high scores located in the local score.txt file
void highScoreScreen() {
    system("cls");
    // Create 3 blank slate players that will be filled in with data from score.txt
    HighScore p1 = { .name = "", .score = 0 };
    HighScore p2 = { .name = "", .score = 0 };
    HighScore p3 = { .name = "", .score = 0 };

    FILE* f;

    // Read the contents of the scores file
    f = fopen("score.txt", "r+"); // doesn't override current file. Open file for reading and writing
    // p1 p2 and p3 retain initial values if not found in score.txt file

    if (f != NULL) { // Print the high score table currently there in score.txt
        fscanf(f, "%s %d\n%s %d\n%s %d\n", &p1.name, &p1.score, &p2.name, &p2.score, &p3.name, &p3.score);
        printf(YELLOW "============ LEADERBOARD ============\n");
        p1.score > 0 ? printf("  1. %-20s %d\n", p1.name, p1.score) : printf("  1. ---\n");
        p2.score > 0 ? printf("  2. %-20s %d\n", p2.name, p2.score) : printf("  2. ---\n");
        p3.score > 0 ? printf("  3. %-20s %d\n", p3.name, p3.score) : printf("  3. ---\n");
        printf("=====================================\n\n" RESET_COLOR);
        fclose(f);
    }
    else { // Print an empty leaderboard since there is no score.txt file
        printf(YELLOW "============ LEADERBOARD ============\n");
        printf("  1. ---\n");
        printf("  2. ---\n");
        printf("  3. ---\n");
        printf("=====================================\n" RESET_COLOR);
        printf("There is no high score data yet!");
    } // Return to the main menu whenever a player inputs a value
    printf("\nPress any key to return to the main menu.\n");
    _getch();
}

// Returns whether or not a string is only digits, which is important to know if a user wants to hold dice
int isOnlyDigits(const char* s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}

// Add up the scores in the leaderboard but making sure to use the max function so the flag value -1 isn't read literally
void updatePlayerTotals() {
    player1.leftTotal = max(player1.ones, 0) + max(player1.twos, 0) + max(player1.threes, 0) + max(player1.fours, 0) + max(player1.fives, 0) + max(player1.sixes, 0);
    player1.bonus = player1.leftTotal > 62 ? 35 : 0;
    player1.rightTotal = max(player1.threeOfKind, 0) + max(player1.fourOfKind, 0) + max(player1.smStraight, 0) + max(player1.lgStraight, 0) + max(player1.fullHouse, 0) + max(player1.yahtzee, 0) + max(player1.chance, 0);
    player1.grandTotal = player1.leftTotal + player1.bonus + player1.rightTotal;
}

// Updates the players scorecard totals and resets all the dice to not be locked and the turns rolled to 0
void startNextSeries() {
    updatePlayerTotals();
    // reset dice being held to none
    for (int i = 0; i < 5; i++) {
        holdDice[i] = 0;
    }
    turnsRolled = 0; // reset turn number the player is on
}

// Format the provided character array into the provided integer, but if the integer is -1 it is an empty string
const void f(char str[3], int num) {
    if (num != -1) // Check if the score is marked
        snprintf(str, 3, "%d", num);
    else { // Set to an empty string
        str[0] = ' ';
        str[1] = ' ';
        str[2] = '\0';
    }
}

// Prints the scorecard without the "-1" flag
void printScorecard() {
    // Create character arrays for each integer so the -1 won't be printed directly
    char onesS[3], twosS[3], threesS[3], foursS[3], fivesS[3],
        sixesS[3], threeOfKindS[3], fourOfKindS[3], smStraightS[3],
        lgStraightS[3], fullHouseS[3], yahtzeeS[3], chanceS[3];

    // Format each character array using the f() function so -1 isn't printed
    f(&onesS, player1.ones);        f(&threeOfKindS, player1.threeOfKind);
    f(&twosS, player1.twos);        f(&fourOfKindS, player1.fourOfKind);
    f(&threesS, player1.threes);    f(&smStraightS, player1.smStraight);
    f(&foursS, player1.fours);      f(&lgStraightS, player1.lgStraight);
    f(&fivesS, player1.fives);      f(&fullHouseS, player1.fullHouse);
    f(&sixesS, player1.sixes);      f(&yahtzeeS, player1.yahtzee);
    f(&chanceS, player1.chance);

    // Print the scoreboard using string formatting and various colors
    printf(CYAN "-----------------------------------------------------------\n");
    printf("                      %s's Scorecard                          \n", player1.name);
    printf("-----------------------------------------------------------\n" RESET_COLOR);
    printf("%12s%-13s%-8s%22s%-2s\n", " 1     Ones: ", onesS, "??ooo", "3 of Kind : ", threeOfKindS);
    printf("%12s%-13s%-8s%22s%-2s\n", " 2     Twos: ", twosS, "?oooo", "4 of Kind : ", fourOfKindS);
    printf("%12s%-13s%-8s%22s%-2s\n", " 3   Threes: ", threesS, "?2345", "Sm Straight (30pts) : ", smStraightS);
    printf("%12s%-13s%-8s%22s%-2s\n", " 4    Fours: ", foursS, "12345", "Lg Straight (40pts) : ", lgStraightS);
    printf("%12s%-13s%-8s%22s%-2s\n", " 5    Fives: ", fivesS, "xxooo", "Full House (25pts) : ", fullHouseS);
    printf("%12s%-13s%-8s%22s%-2s\n", " 6    Sixes: ", sixesS, "ooooo", "Yahtzee (50pts) : ", yahtzeeS);
    printf("%32s%24s%-2s\n", "????? ", "Chance (add dice) : ", chanceS);
    printf(CYAN "-----------------------------------------------------------\n" RESET_COLOR);
    printf("%-12s%-29d%-3s%-3d\n", "Left total : ", player1.leftTotal, "Right total : ", player1.rightTotal);
    printf("Bonus (if left total > 62)(35pts): %d\n", player1.bonus);
    printf(CYAN "-----------------------------------------------------------\n" RESET_COLOR);
    printf("                GRAND TOTAL: %d\n", player1.grandTotal);

}

// Adds the total of dice that are the same face as the given number
int getDiceTotalOf(int numToAdd) {
    int total = 0;
    int diceIndex;
    // get dice total of single number
    // Loop through the dice array
    for (diceIndex = 0; diceIndex < 5; diceIndex++) {
        if (dice[diceIndex] == numToAdd) {
            total += numToAdd;
        }
    }
    return total;
}

// Prompt the user their options, accept input, handle input with errors, and handle ok input
void getAndProcessInput() {
    if (turnsRolled != 3) {
        printf(" - Lock die - Type numbers (ex: '14' locks dice 1 and 4)\n");
        printf(" - Reroll - Press enter with no input\n");
        printf(" - Claim points - Type slot on scoreboard (ex: 'ones','full house')\n");
    }
    else {
        printf(" - Claim points - Type slot on scoreboard (ex: 'ones','full house')\n");
    }
    printf("Input your move (case insentive):\n");

    // Get input until valid input is provided
    while (1) {
        char str[100];
        //        scanf("%[\n]s", str);
        fgets(str, 100, stdin);
        str[strcspn(str, "\n")] = '\0'; // remove \n character

        // lowercase the input
        for (size_t i = 0; i < strlen(str); ++i) {
            str[i] = tolower(str[i]);
        }

        // Count the dice quantities
        int onesCount, twosCount, threesCount, foursCount, fivesCount, sixesCount;
        onesCount = twosCount = threesCount = foursCount = fivesCount = sixesCount = 0;

        int diceIndex;
        for (diceIndex = 0; diceIndex < 5; diceIndex++) {
            switch (dice[diceIndex]) {
            case 1:
                onesCount++;
                break;
            case 2:
                twosCount++;
                break;
            case 3:
                threesCount++;
                break;
            case 4:
                foursCount++;
                break;
            case 5:
                fivesCount++;
                break;
            case 6:
                sixesCount++;
                break;
            default:
                break;
            }
        }

        // Tell the user they can't roll again if they are wanting to roll again
        if (strcmp(str, "0") == 0 || strcmp(str, "r") == 0) {
            if (turnsRolled == 3) {
                printf(RED "You cannot roll the dice again. Input a slot on the scorecard.\n" RESET_COLOR);
                continue;
            }
            else {
                break;
            }
        }
        else if (strcmp(str, "ones") == 0 && player1.ones == -1) {
            player1.ones = getDiceTotalOf(1);
            startNextSeries();
        }
        else if (strcmp(str, "twos") == 0 && player1.twos == -1) {
            player1.twos = getDiceTotalOf(2);
            startNextSeries();
        }
        else if (strcmp(str, "threes") == 0 && player1.threes == -1) {
            player1.threes = getDiceTotalOf(3);
            startNextSeries();
        }
        else if (strcmp(str, "fours") == 0 && player1.fours == -1) {
            player1.fours = getDiceTotalOf(4);
            startNextSeries();
        }
        else if (strcmp(str, "fives") == 0 && player1.fives == -1) {
            player1.fives = getDiceTotalOf(5);
            startNextSeries();
        }
        else if (strcmp(str, "sixes") == 0 && player1.sixes == -1) {
            player1.sixes = getDiceTotalOf(6);
            startNextSeries();
        }
        else if (strcmp(str, "3 of kind") == 0 && player1.threeOfKind == -1) {
            if (onesCount >= 3 || twosCount >= 3 || threesCount >= 3 || foursCount >= 3 || fivesCount >= 3 || sixesCount >= 3)
                player1.threeOfKind = addAllDice();
            else
                player1.threeOfKind = 0;
            startNextSeries();
        }
        else if (strcmp(str, "4 of kind") == 0 && player1.fourOfKind == -1) {
            if (onesCount >= 4 || twosCount >= 4 || threesCount >= 4 || foursCount >= 4 || fivesCount >= 4 || sixesCount >= 4)
                player1.fourOfKind = addAllDice();
            else
                player1.fourOfKind = 0;
            startNextSeries();
        }
        else if (strcmp(str, "sm straight") == 0 && player1.smStraight == -1) {
            if (
                (onesCount >= 1 && twosCount >= 1 && threesCount >= 1 && foursCount >= 1) ||
                (twosCount >= 1 && threesCount >= 1 && foursCount >= 1 && fivesCount >= 1) ||
                (threesCount >= 1 && foursCount >= 1 && fivesCount >= 1 && sixesCount >= 1)
                )
                player1.smStraight = 30;
            else
                player1.smStraight = 0;
            startNextSeries();
        }
        else if (strcmp(str, "lg straight") == 0 && player1.lgStraight == -1) {
            if (
                (onesCount >= 1 && twosCount >= 1 && threesCount >= 1 && foursCount >= 1 && fivesCount >= 1) ||
                (twosCount >= 1 && threesCount >= 1 && foursCount >= 1 && fivesCount >= 1 && sixesCount >= 1)
                )
                player1.lgStraight = 40;
            else
                player1.lgStraight = 0;
            startNextSeries();
        }
        else if (strcmp(str, "full house") == 0 && player1.fullHouse == -1) {
            if ( // a full house requires 2 of one dice and 3 of another dice
                (onesCount == 2 || twosCount == 2 || threesCount == 2 || foursCount == 2 || fivesCount == 2 || sixesCount == 2) &&
                (onesCount == 3 || twosCount == 3 || threesCount == 3 || foursCount == 3 || fivesCount == 3 || sixesCount == 3)
                )
                player1.fullHouse = 25;
            else
                player1.fullHouse = 0;
            startNextSeries();
        }
        else if (strcmp(str, "yahtzee") == 0 && player1.yahtzee == -1) {
            if (onesCount == 5 || twosCount == 5 || threesCount == 5 || foursCount == 5 || fivesCount == 5 || sixesCount == 5)
                player1.yahtzee = 50;
            else
                player1.yahtzee = 0;
            startNextSeries();
        }
        else if (strcmp(str, "chance") == 0 && player1.chance == -1) {
            player1.chance = addAllDice();
            startNextSeries();
        }
        else if (isOnlyDigits(str)) {
            // Do not start next series. Player requests a re-roll
            int num = (int)strtol(str, NULL, 10);
            // prevent invalid number input, we can ignore duplicate dice to hold, for example 515 will hold die 1 and 5 without error
            if (num > 65432) { // when a very large input is given, the string converts to a seemingly-random large number
                printf(RED "\nToo many dice were input to be locked.\n" RESET_COLOR);
                // Input was bad. Continue the loop asking for more input.
                continue;
                // Player cannot hold dice on last roll in a round. They must claim points.
            }
            else if (turnsRolled == 3) {
                printf(RED "You cannot roll the dice again. Input a slot on the scorecard.\n" RESET_COLOR);
                continue;
            }
            else {
                int d;
                for (d = 1; d <= 10000; d *= 10) { // reads 5-digit number left to right
                    int digit = num % (d * 10) / d; // uses integer division to extract each digit
                    if (1 <= digit && digit <= 5) {
                        holdDie(digit - 1);
                    }
                }
            }
        }
        else {
            // SAD PATH
            printf(RED "Input was invalid. If you were trying to write a score, \ncheck your scorecard as you likely marked that already.\n" RESET_COLOR);
            // Input was bad. Continue the loop asking for more input.
            continue;
        }
        // Input was ok. Break out of loop.
        break;
    }
}

/* Prints a "Yahtzee" banner on the main menu by calling an assembly function */
void printBanner() {
    printf(YELLOW);
    asm_print_banner();
    printf(RESET_COLOR "\na game by Adin Geist\n\n");
}

/* Welcomes the user and asks them to input there name, which is saved into a player struct */
void welcomeMessage() {
    printBanner();
    printf("Please enter your name (max 19 characters):\n");
    fgets(player1.name, 20, stdin);
    player1.name[strcspn(player1.name, "\n")] = 0;
    printf("%s\n", player1.name);
}

// Resets the games variables and process the game order
void playGame() {
    // Reset global game variables
    player1.ones = player1.twos = player1.threes = player1.fours = player1.fives = player1.sixes = player1.threeOfKind = player1.fourOfKind = player1.smStraight = player1.lgStraight = player1.fullHouse = player1.yahtzee = player1.chance = -1;
    int i;
    for (i = 0; i < 5; i++) {
        dice[i] = -1;
        holdDice[i] = 0;
    }
    turnsRolled = 0;

    while (turnsRolled < 3 && (player1.ones == -1 || player1.twos == -1 || player1.threes == -1 || player1.fours == -1 || player1.fives == -1 || player1.sixes == -1 ||
        player1.threeOfKind == -1 || player1.fourOfKind == -1 || player1.smStraight == -1 || player1.lgStraight == -1 || player1.fullHouse == -1 ||
        player1.yahtzee == -1 || player1.chance == -1)) // player gets three oppurtunities to roll, hold, and score
    {
        system("cls");
        printScorecard();
        rollDice(); turnsRolled++;
        printDice();
        printf(YELLOW "Roll %d/3\n" RESET_COLOR, turnsRolled);
        getAndProcessInput();
    }

    // Game completed!
    system("cls");
    printScorecard();
    endGame();
}

void endGame() {
    // Default scores in high scores file
    HighScore p1 = { .name = "", .score = 0 };
    HighScore p2 = { .name = "", .score = 0 };
    HighScore p3 = { .name = "", .score = 0 };

    FILE* f;

    // Read the contents of the scores file
    f = fopen("score.txt", "r+"); // doesn't override current file. Open file for reading and writing
    // p1 p2 and p3 retain initial values if not found in score.txt file

    int madeLeaderboard = 1;

    if (f != NULL) {
        fscanf(f, "%s %d\n%s %d\n%s %d\n", &p1.name, &p1.score, &p2.name, &p2.score, &p3.name, &p3.score);


        if (player1.grandTotal > p1.score) {
            // Replace player 3 data with player 2 data
            strncpy(&p3.name, &p2.name, 20);
            p3.score = p2.score;
            // Replace player 2 data with player 1 data
            strncpy(&p2.name, &p1.name, 20);
            p2.score = p1.score;
            // Replace player 1 data with current player
            strncpy(&p1.name, &player1.name, 20);
            p1.score = player1.grandTotal;        strncpy(&p3.name, &p2.name, 20);
        }
        else if (player1.grandTotal > p2.score) {
            // Replace player 3 data with player 2 data
            strncpy(&p3.name, &p2.name, 20);
            p3.score = p2.score;
            // Replace player 2 data with current player
            strncpy(&p2.name, &player1.name, 20);
            p2.score = player1.grandTotal;
        }
        else if (player1.grandTotal > p3.score) {
            // Replace player 3 data with current player
            strncpy(&p3.name, &player1.name, 20);
            p3.score = player1.grandTotal;
        }
        else {
            madeLeaderboard = 0;
        }

        fclose(f);
    }
    else {
        // File doesn't exist! Player in this game must have the highest score.
        strncpy(&p1.name, &player1.name, 20);
        p1.score = player1.grandTotal;
    }

    f = fopen("score.txt", "w+"); // create new file and open for reading and writing
    fprintf(f, "%s %d\n%s %d\n%s %d\n", &p1.name, p1.score, &p2.name, p2.score, &p3.name, p3.score);

    printf("Game complete!\nYou scored: %d points\n\n", player1.grandTotal);
    printf(YELLOW "======== LEADERBOARD ========\n");
    printf("  1. %-20s %d\n", p1.name, p1.score);
    p2.score > 0 ? printf("  2. %-20s %d\n", p2.name, p2.score) : printf("  2. ---\n");
    p3.score > 0 ? printf("  3. %-20s %d\n", p3.name, p3.score) : printf("  3. ---\n");
    printf("=============================\n\n" RESET_COLOR);

    madeLeaderboard == 1 ? printf("You made the leaderboard!\n\n") : printf("You didn't make the leaderboard.\n\n");

    fclose(f); // close the file
    printf("Press any key to return to the main menu...");
    _getch(); // return to the main menu when any key is pressed
}
