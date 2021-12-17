# Yahtzee
A recreation of the beloved game, Yahtzee, using C and x86 assembly.

<img src="https://i.imgur.com/a8ezncf.png" alt="Welcome message" width=450>

<img src="https://i.imgur.com/CS99VfM.png" alt="Yahtzee gameplay" width=400>

# About the Application
The game features the full-length game of Yahtzee. Players get to roll the dice up to three times each turn. After each roll, the player can hold any dice from being rolled next. On the third roll, the player must make their move. The scorecard works the same as its played in real life. The "left side" is the "upper section" where the face of a particular die are added to mark the spot. If the left side totals over 62, a 35 point bonus is awarded. The "right side" is the "lower section" where special dice combinations can score large sums of points. Users can input their name, which can be used for local competition on the leaderboard. The three highest scores are saved in a local text file. Whenever the leaderboard needs read or updated, the text file is opened and is presented or updated if a player beats a saved score.
