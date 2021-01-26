/******************************************************************************
Class for a representation of a Tetris board, using a partial bitboard implementation (uint16 per row).
The board is 10x40 to make room for the top in a potential case in a VS match
where the player gets sent to the top.

*******************************************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TetrisBoard {
    public:
        //init
        TetrisBoard(string fileName);
        // ~TetrisBoard();
        TetrisBoard();

        void readInBoardFromTXT(string fileName);

        void printBoard();

        bool get(int x, int y);
        void set(int x, int y, bool value);
        void clearRow(int line);

        int getRows();
        int getColumns();
    
    private:
        int rows = 40;
        int columns = 10;
        //vector of bitboards, bitboard uint16 per line. 40 of them, allows for easy removal of lines compared
        // to 4 longs
        //In each bitboard, everything is left-oriented, so the final 6 digits are empty and dont mean anything
        //x, y goes from bottom left
        vector<uint16_t> m_board;

};