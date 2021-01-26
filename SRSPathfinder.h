/******************************************************************************
Class for an SRS pathfinder, which starting from a root node and using SRS, finds
every possible placement on the board by continuously expanding nodes from different
movements.

*******************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "TetrisBoard.h"
#include "Move.h"

using namespace std;

class SRSPathfinder {
    public:
        // SRSPathfinder();
        SRSPathfinder(int piece, int x, int y, int rot, TetrisBoard board);
        SRSPathfinder() = default;
        void updatePiece(int piece, int x, int y, int rot);
        // void setX(int x);
        // void setY(int y);
        // void setR(int r);
        // int getX();
        // int getY();
        // int getR();

        void setBoard(TetrisBoard board);
        TetrisBoard* getBoardPointer();

        bool isValidPosition(int x, int y, int r);

        bool isFloating(int x, int y, int r);

        bool tryRotation(int x, int y, int r, int direction, int* newXYR);

        bool threeCornerRule(int x, int y, int r);

        vector<Move> findPossiblePlacementsWithPaths();

    private:
        //original spawn position of piece
        int m_xPos; //0-9
        int m_yPos; //0-19
        int m_rot; //0-3
        TetrisBoard m_board;
        //piece #, 0-6 SZJLTOI
        int m_piece;

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
};