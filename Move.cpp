#include "Move.h"
using namespace std;

Move::Move(int x, int y, int rot, int frames, int piece, vector<int> history, bool tspin) {
    m_x = x;
    m_y = y;
    m_rot = rot;
    m_piece = piece;
    m_moveHistory = history;
    m_frames = frames;
    m_tspin = tspin;
}

// Move::~Move() {

// }

void Move::setX(int x) {
    m_x = x;
}

int Move::getX() {
    return m_x;
}

void Move::setY(int y) {
    m_y = y;
}

int Move::getY() {
    return m_y;
}

void Move::setR(int r) {
    m_rot = r;
}

int Move::getR() {
    return m_rot;
}

void Move::addFrames(int frames) {
    m_frames += frames;
}

int Move::getFrames() {
    return m_frames;
}

void Move::setPiece(int piece) {
    m_piece = piece;
}

int Move::getPiece() {
    return m_piece;
}

void Move::addHistory(int history) {
    // cout << "History: " << history << endl;
    m_moveHistory.push_back(history);
}

vector<int> Move::getHistory() {
    return m_moveHistory;
}

bool Move::getTspin() {
    return m_tspin;
}

void Move::printMove() {
    cout << "Move: (" << m_x << ", " << m_y << ", " << m_rot << ")" << " Piece: " << m_piece << endl;
}

bool Move::operator==(const Move& rhs) const {
    return
        m_x == rhs.m_x &&
        m_y == rhs.m_y &&
        m_rot == rhs.m_rot
        ;
}

bool Move::operator <(const Move& rhs) const {
        return m_x * 100 + m_y * 10 + m_rot < rhs.m_x * 100 + rhs.m_y * 10 + rhs.m_rot;
}