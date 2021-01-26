#include <stdexcept>
#include "TetrisBoard.h"
#include <fstream>
using namespace std;

TetrisBoard::TetrisBoard(string fileName) {
    readInBoardFromTXT(fileName);
}

TetrisBoard::TetrisBoard() {
    //initialize as an empty board
    for (int i = 0; i < rows; i++) {
        m_board.push_back(uint16_t  (0));
    }
}

void TetrisBoard::readInBoardFromTXT(string fileName) {
    m_board.clear();
    //doesn't take into account sending in another board
    for (int i = 0; i < rows; i++) {
        m_board.push_back(uint16_t  (0));
    }
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
        for (int y = rows-1; y >= 0; y--) {
            getline(myfile, line);
            for (int x = 0; x < columns; x++) {
                // cout << line.at(j);
                // cout << "\n";
                if (line.at(x) == '1') {
                    // cout << "REACHED";
                    set(x, y, true);
                }
                else {
                    set(x, y, false);
                }
                
            }
        }
        myfile.close();
    }

    else cout << "Unable to open file";
}

void TetrisBoard::printBoard() {
    for (int y = rows-1; y >= 0; y--) {
        cout << y;
        for (int x = 0; x < columns; x++) {
            if (get(x, y)) {
                cout << "# ";
            }
            else {
                cout << "_ ";
            }
        }
        cout << "\n";
    }
}

bool TetrisBoard::get(int x, int y) {
    if (x < 0 || x > 9 || y < 0 || y > 39) {
        cout << "TetrisBoard get index out of bounds." << endl;
        exit(0);
    }
    return (m_board[y] & (uint16_t(1) << x)) != 0;
}

void TetrisBoard::set(int x, int y, bool value) {
    //bitshift within row
    if (value) {
        m_board[y] |= (uint16_t(1) << x);
    }
    else {
        m_board[y] &= ~(uint16_t(1) << x);
    }
    
}

void TetrisBoard::clearRow(int line) {
    m_board.erase(m_board.begin()+line);
    uint16_t newRow = 0;
    m_board.push_back(newRow);
}

int TetrisBoard::getRows() {
    return rows;
}

int TetrisBoard::getColumns() {
    return columns;
}