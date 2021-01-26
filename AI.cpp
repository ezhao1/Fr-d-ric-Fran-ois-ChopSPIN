#include "AI.h"
#include <cmath>
#include <queue>
#include <unordered_map>
#include <algorithm>

//heuristic function to judge board states
double evaluate(int totalLinesSent, int linesCleared, TetrisBoard board) {
    //stack height, flatness, holes

    double LINESSENT_Weight = 2;
    double LINESCLEARED_Weight = 2;
    double HOLETYPE1_Weight = -0.5; // an empty cell where there is a filled cell somewhere above the cell
    // double HOLETYPE2_Weight = -1; //an empty cell where there is a filled cell in the column to the left at or above the cell
    // double HOLETYPE3_Weight = -1; //an empty cell where there is a filled cell in the column to the right at or above the cell
    double BUMPINESS_Weight = -0.3; //summed up differences between adjacent column heights
    double WELL_Weight = -0.5; // ab empty cell where both cells to the left and right (including wall) are filled
    double STACKHEIGHT_ExponentWeight = 2; // weight exponentially, so height^(weight). aggregate height of columns
    double STACKHEIGHT_MultipleWeight = -0.04; //multiplied value to the exponential weight

    //i guess we just iterate over every value in each row bitboard? idk how to do more efficiently
    int holeType1 = 0;
    int well = 0;
    // int holeType2 = 0;
    // int holeType3 = 0;
    double bumpiness = 0;
    double stackHeight = 0;


    double heightTotal = 0;

    //finding highest block in each column to calculate bumbpiness and stackheight, also used after to find holes
    int highestHeightPerX[10];
    for (int column = 0; column < board.getColumns(); column++) {
        bool highestYFound = false;
        for (int y = board.getRows() - 1; y >= 0; y--) {
            if (board.get(column, y)) {
                // cout << "ACTUALLY GETS HERE" << endl;
                // cout << "x, y: " << column << ", " << y << endl;
                //found
                highestHeightPerX[column] = y + 1;
                heightTotal+= y + 1;
                highestYFound = true;
                break;
            }
        }
        //only reaches here if not found
        if (!highestYFound) {
            highestHeightPerX[column] = 0;
        }
        //no need to add height, would just be +0
    }

    //calculate average height (stack height)
    stackHeight = heightTotal / (double)board.getColumns();

    //CALCULATE HOLES AND WELLS
    // for (int column = 0; column < board.getColumns(); column++) {
    //     for (int row = highestHeightPerX[column] - 1; row >= 0; row--)  {
    //         //calculate hole type 1. start at cell right below highest one
    //         if (!board.get(column, row)) {
    //             holeType1++;
    //         }

    //     }
    // }
    for (int column = 0; column < board.getColumns(); column++) {
        //CALCULATE BUMPINESS. compare height of each column to the one to the right. dont do for last column
        if (column != board.getColumns() - 1) {
            bumpiness += abs(highestHeightPerX[column + 1] - highestHeightPerX[column]);
        }

        //start at the height to calculate wells next to it
        if (highestHeightPerX[column] > 0) {
            for (int row = highestHeightPerX[column] - 1; row >= 0; row--)  {
                //calculate hole type 1. start at cell right below highest one, nope start as highest
                if (!board.get(column, row)) {
                    holeType1++;
                }
                else {
                    //check for wells on the right side, then a separate check for the first column
                    //check if the cell to the right is unfilled
                    //first check if it's a wall
                    
                    if (column != board.getColumns() - 1) {
                        if (!board.get(column + 1, row)) {
                            if (column + 1 == board.getColumns() - 1) {
                                well ++;
                            }
                            else if (board.get(column+2, row)) {
                                well++;
                            }
                        }
                    }

                    //separate check for first column
                    if (column == 1) {
                        if (!board.get(0, row)) {
                            well++;
                        }
                    }
                }

            }
        }
        
    }

    double score = 0;
    score += (double)totalLinesSent * LINESSENT_Weight;
    score += (double)linesCleared * LINESCLEARED_Weight;
    score += (double)holeType1 * HOLETYPE1_Weight;
    score += (double)well * WELL_Weight;
    // score += (double)holeType2 * HOLETYPE2_Weight;
    // score += (double)holeType3 * HOLETYPE3_Weight;
    score += bumpiness * BUMPINESS_Weight;
    score += pow(stackHeight, STACKHEIGHT_ExponentWeight) * STACKHEIGHT_MultipleWeight;
    return score;
}

TetrisBoard updateBoardAfterMove(Move move, TetrisBoard board, pair<int, int>* lines) {
    //move later to centralized constants page (cant figure it out, keeps saying multiple definition)
    int SZJLTOIArray [7][4][4][2] = {{{{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
                   {{0, 1}, {0, 0}, {1, 0}, {1, -1}},
                   {{-1, -1}, {0, -1}, {0, 0}, {1, 0}},
                   {{-1, 1}, {-1, 0}, {0, 0}, {0, -1}}},

                  {{{-1, 1}, {0, 1}, {0, 0}, {1, 0}},
                   {{0, -1}, {0, 0}, {1, 0}, {1, 1}},
                   {{-1, 0}, {0, 0}, {0, -1}, {1, -1}},
                   {{-1, -1}, {-1, 0}, {0, 0}, {0, 1}}},

                  {{{-1, 1}, {-1, 0}, {0, 0}, {1, 0}},
                   {{1, 1}, {0, 1}, {0, 0}, {0, -1}},
                   {{-1, 0}, {0, 0}, {1, 0}, {1, -1}},
                   {{0, 1}, {0, 0}, {0, -1}, {-1, -1}}},

                  {{{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
                   {{0, 1}, {0, 0}, {0, -1}, {1, -1}},
                   {{-1, -1}, {-1, 0}, {0, 0}, {1, 0}},
                   {{-1, 1}, {0, 1}, {0, 0}, {0, -1}}},

                  {{{-1, 0}, {0, 0}, {0, 1}, {1, 0}},
                   {{0, 1}, {0, 0}, {1, 0}, {0, -1}},
                   {{-1, 0}, {0, 0}, {0, -1}, {1, 0}},
                   {{0, -1}, {0, 0}, {-1, 0}, {0, 1}}},

                  {{{0, 1}, {0, 0}, {1, 0}, {1, 1}},
                   {{0, 0}, {0, -1}, {1, -1}, {1, 0}},
                   {{-1, 0}, {-1, -1}, {0, -1}, {0, 0}},
                   {{-1, 1}, {-1, 0}, {0, 0}, {0, 1}}},

                  {{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
                   {{0, 1}, {0, 0}, {0, -1}, {0, -2}},
                   {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}},
                   {{0, 2}, {0, 1}, {0, 0}, {0, -1}}}};
        
        int JLSTZOffsetTable [4][5][2] = {{{ 0, 0}, { 0, 0},	{ 0, 0}, { 0, 0}, { 0, 0}},
                             {{ 0, 0}, { 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}},
                             {{ 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}},
                             {{ 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}}};

        int IOffsetTable [4][5][2] = {{{ 0, 0}, {-1, 0}, { 2, 0}, {-1, 0}, { 2, 0}},
                                {{-1, 0}, { 0, 0}, { 0, 0}, { 0, 1}, { 0,-2}},
                                {{-1, 1}, { 1, 1}, {-2, 1}, {1, 0}, {-2, 0}},
                                {{ 0, 1}, { 0, 1}, { 0, 1}, { 0,-1}, { 0, 2}}};

        int OOffsetTable [4][2] = {{0, 0},
                                {0, -1},
                                {-1, -1},
                                {-1, 0}};

    TetrisBoard newBoard = board;
    //store all relevant y values for checking for clears. since only small amount use vector instead of set
    //maybe use set for the ordering so can have consistent offsets when clearing multiple lines. order it from high to low

    vector<int> yValues;
    for (int blockPart; blockPart < 4; blockPart++) {
        int globalX = move.getX() + SZJLTOIArray[move.getPiece()][move.getR()][blockPart][0];
        int globalY = move.getY() + SZJLTOIArray[move.getPiece()][move.getR()][blockPart][1];
        newBoard.set(globalX, globalY, true);

        if (!count(yValues.begin(), yValues.end(), globalY)) {
            yValues.push_back(globalY);
        }
    }

    //sort yvalues from most to least so clearing them in that order has no problems
    sort(yValues.begin(), yValues.end(), greater<int>());

    //need to go through and clear line(s), calculate amount sent and lines cleared. back to back as well, b2b will add later
    //there must be a more efficient method than going one by one and seeing if entire row is selected, need to see later
    //TAKE INTO ACCOUNT SHIFTING INDEXES
    int numberOfLinesCleared = 0;
    for (int yIndex = 0; yIndex < yValues.size(); yIndex++) {
        int yValue = yValues[yIndex];
        bool clearRow = true;
        for (int x = 0; x < newBoard.getColumns(); x++) {
            if (!newBoard.get(x, yValue)) {
                clearRow = false;
                break;
            }
        }

        if (clearRow) {
            numberOfLinesCleared++;
            //clear the line
            newBoard.clearRow(yValue);
        }
    }

    //calculate amount sent
    int amountSentThisRound = 0;
    if (move.getTspin()) {
        amountSentThisRound = numberOfLinesCleared * 2;
    }
    else {
        if (numberOfLinesCleared == 4) {
            amountSentThisRound = 4;
        }
        else if (numberOfLinesCleared == 3) {
            amountSentThisRound = 2;
        }
        else if (numberOfLinesCleared == 2) {
            amountSentThisRound = 1;
        }
    }
    int totalAmountSent = (*lines).second + amountSentThisRound;

    //add on b2b later and combos

    // int numberOfLinesCleared = 0;
    // int amountSent = 0;
    //update
    (*lines).first = numberOfLinesCleared;
    (*lines).second = totalAmountSent;

    return newBoard;
    
}

struct MoveWithBoardAfter {
  Move move;
  TetrisBoard board;
  int originalMoveIndex;
  int totalSends;

  bool operator <(const MoveWithBoardAfter& rhs) const {
      //i dont think this matters so just making this arbitrary
      return originalMoveIndex < rhs.originalMoveIndex;
  }
};

struct OriginalMoveIndexAndTotalSends {
    int originalMoveIndex;
    int totalSends;
};

AI::AI(TetrisBoard board, int levels, int optionsToExplorePerLevel) {
    m_rootBoard = board;
    m_levels = levels;
    m_optionsToExplorePerLevel = optionsToExplorePerLevel;
}


//no hold taken into account right now
pair<Move, TetrisBoard> AI::GreedyBFS(int piece, int x, int y, int r, int* queue, int* sends) {
    //go through each possible move and select
    //the few highest eval, continue for next move from those, etc.
    //Pick move that gets the best eval board at end.
    //in each pair, int is the eval result of the move
    priority_queue<pair<int, MoveWithBoardAfter>> moves;
    SRSPathfinder pathfinder(piece, x, y, r, m_rootBoard);
    vector<Move> rootMoves = pathfinder.findPossiblePlacementsWithPaths();
    //this tetris board is of before doing the move
    vector<tuple<Move, TetrisBoard, OriginalMoveIndexAndTotalSends>> potentialMoves;
    for (int i = 0; i < rootMoves.size(); i++) {
        OriginalMoveIndexAndTotalSends originalAndSends;
        originalAndSends.originalMoveIndex = i;
        originalAndSends.totalSends = 0;
        potentialMoves.push_back(tuple<Move, TetrisBoard, OriginalMoveIndexAndTotalSends>(rootMoves[i], m_rootBoard, originalAndSends));
    }
    // cout << "BFS 1" << endl;
    // for (int i = 0; i < potentialMoves.size(); i++) {
    //     //this part just takes all potential moves and the boards after and evaluates them.
    //     pair<int, int> linesClearedSent;
    //     TetrisBoard newBoard  = updateBoardAfterMove(get<0>(potentialMoves[i]), get<1>(potentialMoves[i]), &linesClearedSent);
    
    //     int eval = evaluate(linesClearedSent.first, linesClearedSent.second, newBoard);
    //     MoveWithBoardAfter item;
    //     item.move = get<0>(potentialMoves[i]);
    //     item.board = newBoard;
    //     item.originalMoveIndex = get<2>(potentialMoves[i]);
    //     moves.push(pair<int, MoveWithBoardAfter>(eval, item));
    // }
    // potentialMoves.clear();
    
    // cout << "MARKER 1" << endl;

    //this is real janky. the loop should be rearranged or something
    
    for (int level = 0; level < m_levels; level++) {
        // cout << "BFS 1.25: level: " << level << endl;
        for (int i = 0; i < potentialMoves.size(); i++) {
            //this part just takes all potential moves and the boards after and evaluates them.
            pair<int, int> linesClearedSent;
            linesClearedSent.second = get<2>(potentialMoves[i]).totalSends;
            TetrisBoard newBoard  = updateBoardAfterMove(get<0>(potentialMoves[i]), get<1>(potentialMoves[i]), &linesClearedSent);
            // cout << "TOTAL LINES SENT: " << linesClearedSent.second << endl;
            // cout << "TOTAL LINES CLEARED: " << linesClearedSent.first << endl;
            int eval = evaluate(linesClearedSent.first, linesClearedSent.second, newBoard);
            MoveWithBoardAfter item;
            item.move = get<0>(potentialMoves[i]);
            item.board = newBoard;
            item.originalMoveIndex = get<2>(potentialMoves[i]).originalMoveIndex;
            item.totalSends = linesClearedSent.second;
            moves.push(pair<int, MoveWithBoardAfter>(eval, item));
            // cout << "SIZE OF MOVES PQ IN LOOP: " << i << " " << moves.size() << endl;
        }
        potentialMoves.clear();
        // cout << "MARKER 2.1" << endl;
        // cout << "SIZE OF MOVES PQ: " << moves.size() << endl;
        // cout << "MOVES SIZE: " << moves.size() << endl;
        for (int i = 0; i < m_optionsToExplorePerLevel; i++) {
            // cout << "HI0.5" << endl;
            MoveWithBoardAfter item = moves.top().second;
            SRSPathfinder piecePathfinder(queue[level], 4, 19, 0,
                item.board);
            vector<Move> piecePotentialMoves = piecePathfinder.findPossiblePlacementsWithPaths();
            // cout << piecePotentialMoves.size() << endl;
            // cout << "HI" << endl;
            for (int piecePotentialMove = 0; piecePotentialMove < piecePotentialMoves.size(); piecePotentialMove++) {
                OriginalMoveIndexAndTotalSends originalAndSends;
                originalAndSends.originalMoveIndex = item.originalMoveIndex;
                originalAndSends.totalSends = item.totalSends;
                // cout << "HI1.5 " << i << endl;
                potentialMoves.push_back(tuple<Move, TetrisBoard, OriginalMoveIndexAndTotalSends>(piecePotentialMoves[piecePotentialMove], item.board, originalAndSends));
                // cout << "HI1.6" << endl;
            }
            // cout << "HI2" << endl;
            // cout << moves.size() << endl;
            moves.pop();
        }

        // cout << "MARKER 2.5" << endl;
        

        //if last level, do one final round of evaluation and then pick one and push it to potential moves
        if (level == m_levels - 1) {
            // cout << "BFS 1.5" << endl;
            // cout << "MARKER 3" << endl;
            for (int i = 0; i < potentialMoves.size(); i++) {
                //this part just takes all potential moves and the boards after and evaluates them.
                pair<int, int> linesClearedSent;
                linesClearedSent.second = get<2>(potentialMoves[i]).totalSends;
                TetrisBoard newBoard  = updateBoardAfterMove(get<0>(potentialMoves[i]), get<1>(potentialMoves[i]), &linesClearedSent);
            
                int eval = evaluate(linesClearedSent.first, linesClearedSent.second, newBoard);
                MoveWithBoardAfter item;
                item.move = get<0>(potentialMoves[i]);
                item.board = newBoard;
                item.originalMoveIndex = get<2>(potentialMoves[i]).originalMoveIndex;
                item.totalSends = linesClearedSent.second;
                moves.push(pair<int, MoveWithBoardAfter>(eval, item));
            }

            potentialMoves.clear();
            MoveWithBoardAfter item = moves.top().second;
            OriginalMoveIndexAndTotalSends originalAndSends;
            originalAndSends.originalMoveIndex = item.originalMoveIndex;
            originalAndSends.totalSends = item.totalSends;
            potentialMoves.push_back(tuple<Move, TetrisBoard, OriginalMoveIndexAndTotalSends>(item.move, item.board, originalAndSends));
            TetrisBoard finalBoard = item.board;
            // finalBoard.printBoard();
            moves.pop();

        }
        
        //empty priority queue
        moves = priority_queue<pair<int, MoveWithBoardAfter>>();
        
    }
    // cout << "BFS 2" << endl;

    // cout << "MARKER 4" << endl;

    //recalculate board after the move. could be more efficient, already done earlier
    Move finalMove = rootMoves[get<2>(potentialMoves.front()).originalMoveIndex];
    Move final5thLevelMove = get<0>(potentialMoves.front());
    // final5thLevelMove.printMove();
    // finalMove.printMove();
    // m_rootBoard.printBoard();
    // cout << "FINAL SENDS: " << get<2>(potentialMoves.front()).totalSends << endl;
    pair<int, int> linesClearedSent;
    TetrisBoard newBoard  = updateBoardAfterMove(finalMove, m_rootBoard, &linesClearedSent);
    *sends = linesClearedSent.second;

    // cout << "TESTING" << endl;
    // vector<int> newVector;
    // pair<int, int> lines;
    // Move newMove(9, 3, 3, 0, 4, newVector, true);
    // TetrisBoard testBoard = updateBoardAfterMove(newMove, m_rootBoard, &lines);
    // testBoard.printBoard();

    // cout << "GEST HERE 1" << endl;
    // cout << "BFS 3" << endl;
    return pair<Move, TetrisBoard>(finalMove, newBoard);
}

