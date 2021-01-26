/******************************************************************************
Class for an individual piece move on a tetris board.

*******************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

class Move {
    public:
        Move(int x, int y, int rot, int frames, int piece, vector<int> history, bool tspin);
        // ~Move();
        Move() = default;
        void setX(int x);
        void setY(int y);
        void setR(int r);
        int getX();
        int getY();
        int getR();
        void addFrames(int frames);
        int getFrames();
        void setPiece(int piece);
        int getPiece();
        void addHistory(int history);
        vector<int> getHistory();
        bool getTspin();
        void printMove();
        bool operator==(const Move& rhs) const;
        bool operator <(const Move& rhs) const;

    private:

        int m_x;
        int m_y;
        int m_rot;
        int m_frames = 0;
        int m_piece;
        vector<int> m_moveHistory;;
        bool m_tspin = false;

};