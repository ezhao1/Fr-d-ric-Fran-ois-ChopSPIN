# Frédéric François ChopSPIN
Guideline Tetris AI that uses a greedy BFS search to look through potential boards across 5 previews and select the best move. The Tetris board is implemented through a partial bitboard implementation, and a heuristic function measuring six features (lines sent, lines cleared, holes, bumpiness, wells, and stack height) is used to evaluate boards.
