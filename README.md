# Othello Solver

A solver for Othello (also known as Reversi) that implements monte-carlo tree search.

### Board Representation

A bitboard representation is used for the board with two 64 bit boards for white and black pieces
Monte-Carlo Tree Search is used to explore or exploit the search space

### How To Use

#### Building and Running

run `make` to build
run `./othello` to run

#### Commands:

1. `-d` - Debug flag: shows current iteration, memory used by tree in kb, and number of nodes. Also shows time taken. WARNING: Causes performance hits
2. `-b` - Budget flag: argument after this flag will specify the number of iterations. Default is 10000
3. `-u` - Constant flag: argument after this flag will be the constant used for Upper Confidence Bound. A higher number will explore more nodes, a lower number will exploit more. Default is 1.5.
4. `-p` - Player flag: argument after this will specify the order Orders listed below.
   `hh` - default: human plays against human
   `hm` - human plays first against machine swap order to swap first player
   `mm` - machine plays itself
5. `-l` - Specifies lerping through two UCB values. Argument after this is first UCB, after that is second UCB_C. Do not use with -u.

### Examples

The AI playing itself

https://github.com/Athenaphilia/OthelloSolver/assets/127344898/a433f69c-4632-4528-b87a-489a82ec5493
