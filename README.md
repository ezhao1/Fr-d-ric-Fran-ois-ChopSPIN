# Frédéric François ChopSPIN
Guideline Tetris AI that uses a greedy BFS search to look through potential boards across 5 previews and select the best move. The Tetris board is implemented through a partial bitboard implementation, and a heuristic function measuring six features (lines sent, lines cleared, holes, bumpiness, wells, and stack height) is used to evaluate boards.

# SRS Pathfinder

To find all possible placements with a certain piece on the board, I need to do a breadth first search of sorts. Just finding every possible orientation of the piece within the space and then lowering it onto the board wont get me every placement, because with SRS, you can do tucks and spins that can give placements that won’t be detected with that method. Thus, I first build an SRS pathfinder, using the kick tables provided by guideline, to start at the root node and expand with every possible movement type - left move, right move, softdrop, rotate left, and rotate right. This repeats, until I eventually find every possible move with that piece on the board.

# AI

In order to determine which of these possible placements on the board is the best to do for the best scoring, I need to do a search across the game tree. However, just directly doing a breadth first search across the state space, as mentioned earlier, is very costly. With 6 consecutive different pieces and about 35 different possible placements per piece per board, there are about 1.838 billion sequences to search through, which would take far too long to do.

This, I implement a greedy element to my BFS: on each level, I only select a certain number of the best moves (currently set to 15) according to some kind of evaluation of the board, and then expand those boards. At the end, I select the move that brings me the best end evaluation board.

# Evaluation Function

In my evaluation heuristic function, I measure 6 features: cumulative line send across the 6piece sequence, the lines cleared, holes (defined as an empty cell on the board in which there is at least one filled cell above it), bumpiness (defined as the summed up difference between adjacent columns, in which the height is taken from the highest filled cell in each column), wells (defined as an empty cell in which the cells to the left and right are filled, including walls), and finally average stack height (the height of each column averaged). This feature is weighted quadratically, because I don’t care as much about the height lower on the board since I want to be able to stack up and score higher, but I care more about it higher on the board since I don’t want to top out.
