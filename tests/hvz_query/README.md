# HvZ Player Query Test
An application that given a list of HvZ players, will increment the number of 
games played for all human or zombie players with a bvu email and return a list 
of those players if they have played 8 games.

This performs 2 filters and 2 transformations

### Operator Workflow
  1) Filter out (remove) tuples from non-bvu emails
  2) Increment the number of games played by 1
  3) Filter out tuples with less than 8 games played
  4) Transform data by removing the @domain part of the email
  5) Output (print) tuples

### Input data format
Format: ```playerStatus    gamesPlayed    email```
  * **playerStatus:** In the format: ```"%c"``` in [M, C, H, Z]
  * **gamesPlayed:** In the format: ```"%d"``` in range [0, 8]
  * **email:** In the format: ```"%s@%s"``` where %s is the player name and @%s in [gmail.com, bvu.edu, yahoo.com, hotmail.com]

### Output data format
Format: ```playerStatus    gamesPlayed    email```
  * **playerStatus:** In the format: ```"%c"``` in [M, C, H, Z]
  * **gamesPlayed:** In the format: ```"%d"``` in range [0, 8]
  * **email:** In the format: ```"%s"``` where %s is the player name
