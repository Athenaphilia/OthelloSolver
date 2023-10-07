# Othello Solver

This is a solver for Othello (also known as Reversi) that implements monte-carlo tree search.

### Board Representation

A bitboard representation is used for the board with two 64 bit boards for white and black pieces

### How To Use

Commands:

1. `-d` - Debug flag: shows current iteration, memory used by tree in kb, and number of nodes. Also shows time taken. WARNING: Causes performance hits
2. `-b` - Budget flag: argument after this flag will specify the number of iterations. Default is 10000
3. `-u` - Constant flag: argument after this flag will be the constant used for Upper Confidence Bound. A higher number will explore more nodes, a lower number will exploit more. Default is 1.5.
4. `-p` - Player flag: argument after this will specify the order.
   `hh` - default: human plays against human
   `hm` - human plays first against machine swap order to swap first player
   `mm` - machine plays itself
5. `-l` - Specifies lerping through two UCB_C values. Argument after this is first UCB_C, after that is second UCB_C
