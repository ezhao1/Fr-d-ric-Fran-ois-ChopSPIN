/******************************************************************************
Class for the AI that will traverse through potential moves and select one to choose.

*******************************************************************************/

#include <iostream>
#include "SRSPathfinder.h"
// #include "evaluate.cpp"

class AI {
    public:
        AI(TetrisBoard board, int levels, int optionsToExplorePerLevel);
        AI() = default;
        //returns the selected move and board state after
        pair<Move, TetrisBoard> GreedyBFS(int piece, int x, int y, int r, int* queue, int* sends);
        // Move SimulatedAnnealing();
    private:
        TetrisBoard m_rootBoard;
        // vector<Move> m_moves;
        int m_levels;
        int m_optionsToExplorePerLevel;
};