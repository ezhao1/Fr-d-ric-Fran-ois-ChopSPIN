#include "SRSPathfinder.h"
#include <map>
#include <unordered_map>
#include <queue>
#include <tuple>

using namespace std;

SRSPathfinder::SRSPathfinder(int piece, int x, int y, int rot, TetrisBoard board) {
    m_xPos = x;
    m_yPos = y;
    m_rot = rot;
    m_piece = piece;
    m_board = board;
}

void SRSPathfinder::updatePiece(int piece, int x, int y, int rot) {
    m_xPos = x;
    m_yPos = y;
    m_rot = rot;
    m_piece = piece;
}
 
// SRSPathfinder::SRSPathfinder() {
//     //empty
// }

bool SRSPathfinder::isValidPosition(int x, int y, int r) {
    for (int blockPart = 0; blockPart < 4; blockPart++) {
        int globalX = x + SZJLTOIArray[m_piece][r][blockPart][0];
        int globalY = y + SZJLTOIArray[m_piece][r][blockPart][1];
        // detect if in bounds x
        if (globalX < 0 || globalX > 9) {
            return false;
        }
        //detect if in bounds y
        if (globalY <0 || globalY >20) {
            return false;
        }
        //detect piece collision
        if (m_board.get(globalX, globalY)) {
            return false;
        }
    }
    return true;
}

bool SRSPathfinder::isFloating(int x, int y, int r) {
    unordered_map<int, int> dictionary;
    //for each x, find lowest y
    for (int blockPart = 0; blockPart < 4; blockPart++) {
        int globalX = x + SZJLTOIArray[m_piece][r][blockPart][0];
        int globalY = y + SZJLTOIArray[m_piece][r][blockPart][1];
        //if key already exists
        if (dictionary.count(globalX) == 1) {
            if (globalY < dictionary[globalX]) {
                dictionary[globalX] = globalY;
            }
        }
        else {
            dictionary[globalX] = globalY;
        }
    }
    unordered_map<int, int>::iterator it;
    for (it = dictionary.begin(); it != dictionary.end(); it++) {

        if (it->second == 0) {
            // cout << "DA FIRST" << endl;
            return false;
        }
        if (m_board.get(it->first, it->second - 1)) {
            // cout << "DA SECOND" << endl;
            return false;
        }
    }
    return true;
}

bool SRSPathfinder::tryRotation(int x, int y, int r, int direction, int* newXYR) {
    int rotationAfter = r + direction;
    if (rotationAfter == 4) {
        rotationAfter = 0;
    }
    else if (rotationAfter == -1)
    {
        rotationAfter = 3;
    }

    bool rotationSucceeds = false;

    if (m_piece == 5) {
        // o piece
        //FOR AI PURPOSES, JUST MAKE ROTATION NOT POSSIBLE SO DONT HAVE TO GO THROUGH EXTRA OPTIONS
        //nvm
        int kickTranslationX = OOffsetTable[r][0] - OOffsetTable[rotationAfter][0];
        int kickTranslationY = OOffsetTable[r][1] - OOffsetTable[rotationAfter][1];
        newXYR[0] = x + kickTranslationX;
        newXYR[1] = y + kickTranslationY;
        newXYR[2] = rotationAfter;
        rotationSucceeds = true;

        // rotationSucceeds = false;
    }
    else if (m_piece == 6)
    {
        // i ipiece
        int testNumber = 0;
        while (!rotationSucceeds) {
            if (testNumber > 4) {
                break;
            }
            int kickTranslationX = IOffsetTable[r][testNumber][0] - IOffsetTable[rotationAfter][testNumber][0];
            int kickTranslationY = IOffsetTable[r][testNumber][1] - IOffsetTable[rotationAfter][testNumber][1];
            newXYR[0] = x + kickTranslationX;
            newXYR[1] = y + kickTranslationY;
            newXYR[2] = rotationAfter;
            rotationSucceeds = isValidPosition(x + kickTranslationX, y + kickTranslationY, rotationAfter);
            testNumber++;
        }
    }
    else {
        // jlstz pieces
        int testNumber = 0;
        while (!rotationSucceeds) {
            if (testNumber > 4) {
                break;
            }
            int kickTranslationX = JLSTZOffsetTable[r][testNumber][0] - JLSTZOffsetTable[rotationAfter][testNumber][0];
            int kickTranslationY = JLSTZOffsetTable[r][testNumber][1] - JLSTZOffsetTable[rotationAfter][testNumber][1];
            newXYR[0] = x + kickTranslationX;
            newXYR[1] = y + kickTranslationY;
            newXYR[2] = rotationAfter;
            rotationSucceeds = isValidPosition(x + kickTranslationX, y + kickTranslationY, rotationAfter);
            testNumber++;
        }
    }
    return rotationSucceeds;
    
}

bool SRSPathfinder::threeCornerRule(int x, int y, int r) {
    if (m_piece == 4) {
        int totalCorners = 0;
        //top left corner
        if (x-1 < 0) {
            totalCorners++;
        }
        else {
            if (m_board. get(x-1, y+1)) {
                totalCorners++;
            }
        }
        //top right corner
        if (x +1 > m_board.getColumns() - 1) {
            totalCorners++;
        }
        else {
            if (m_board.get(x+1, y+1)) {
                totalCorners++;
            }
        }
        //bottom left and right corners
        if (y-1 < 0) {
            totalCorners+=2;
        }
        else {
            //bottom left
            if (x-1 < 0) {
                totalCorners++;
            }
            else {
                if (m_board. get(x-1, y-1)) {
                    totalCorners++;
                }
            }
            //bottom right
            if (x +1 > m_board.getColumns() - 1) {
                totalCorners++;
            }
            else {
                if (m_board.get(x+1, y-1)) {
                    totalCorners++;
                }
            }
        }
        if (totalCorners == 3) {
            return true;
        }
    }
    return false;
}

vector<Move> SRSPathfinder::findPossiblePlacementsWithPaths() {
    // # with the root node being your spawning position,
    // # expand each node of the graph in all three directions, merging nodes which have matching (x,y,r) pairs,
    // # picking the fastest option when merging.
    // # Put in checks to avoid backtracking?
    // # 
    // # movementLeft = 0
    // # movementRight = 1
    // # softdrop = 2
    // # rotateLeft = 3
    // # rotateRight = 4
    // harddrop = 5
    // # later modify to include das? maybe just for first move.
    vector<Move> possibleMoves;
    //key will be x *100 + y*10 + r to make a unique key for each position. this doesnt work
    // use y*100 + x*10 + r instead b/c y goes to 39 (double digits)
    unordered_map<int, Move> dictionary;
    Move root(m_xPos, m_yPos, m_rot, 0, m_piece, {}, false);
    queue<Move> movesToExpand;

    // possibleMoves.push_back(&root);
    dictionary[m_yPos * 100 + m_xPos * 10 + m_rot] = root;
    movesToExpand.push(root);
    // int timesItRuns = 0;

    while (!movesToExpand.empty()) {
        // cout << "AYO" << timesItRuns << endl;
        // timesItRuns++;
        // at each level, expand each node in all 5 directions. then merge nodes.
        //check if move is possible.
        //for now, add one frame per move. in this scheme, all identical positions found later take longer,
        // so identical moves found later will be thrown out (modify later)
        // with this scheme, dont need to worry about backtrack
        // bool backtrack = false;

        // //harddrop. make it so dont add to queue after, final
        // if (isFloating(  (movesToExpand.front()).getX(), (movesToExpand.front()).getY(),
        //     (movesToExpand.front()).getR())  ) {
            
        //     for (int levelsDown = 1; levelsDown < (movesToExpand.front()).getY() + 1; levelsDown++) {
        //         int harddropX = (movesToExpand.front()).getX();
        //         int harddropY = (movesToExpand.front()).getY() - levelsDown;
        //         int harddropR = (movesToExpand.front()).getR();
        //         if (isValidPosition(harddropX, harddropY, harddropR) && !isFloating(harddropX, harddropY, harddropR)) {
        //             int key = (harddropY) *100 + harddropX*10 + harddropR;
        //             if (dictionary.count(key) == 0) {
        //                 Move possibleHarddrop(harddropX, harddropY, harddropR,
        //                     (movesToExpand.front()).getFrames(), (movesToExpand.front()).getHistory());
        //                 possibleHarddrop.addFrames(1);
        //                 possibleHarddrop.addHistory(5);
        //                 dictionary[key] = possibleHarddrop;
        //             }
        //             break;
        //         }
        //     }
        // }
        

        // movement left
        int movementLeftX = (movesToExpand.front()).getX() - 1;
        int movementLeftY = (movesToExpand.front()).getY();
        int movementLeftR = (movesToExpand.front()).getR();
        if (isValidPosition(movementLeftX, movementLeftY, movementLeftR)) {
            int key = movementLeftY * 100 + movementLeftX * 10 + movementLeftR;
            if (dictionary.count(key) == 0) {
                Move possibleLeftMove(movementLeftX, movementLeftY, movementLeftR,
                      (movesToExpand.front()).getFrames(), m_piece, (movesToExpand.front()).getHistory(), false);
                possibleLeftMove.addFrames(1);
                possibleLeftMove.addHistory(0);
                dictionary[key] = possibleLeftMove;
                movesToExpand.push(possibleLeftMove);
            }
        }
        //movement right
        int movementRightX = (movesToExpand.front()).getX() + 1;
        int movementRightY = (movesToExpand.front()).getY();
        int movementRightR = (movesToExpand.front()).getR();
        if (isValidPosition(movementRightX, movementRightY, movementRightR)) {
            int key = movementRightY *100 + movementRightX * 10 + movementRightR;
            if (dictionary.count(key) == 0) {
                Move possibleRightMove(movementRightX, movementRightY, movementRightR,
                    (movesToExpand.front()).getFrames(), m_piece, (movesToExpand.front()).getHistory(), false);
                possibleRightMove.addFrames(1);
                possibleRightMove.addHistory(1);
                dictionary[key] = possibleRightMove;
                movesToExpand.push(possibleRightMove);
            }
        }
        //softdrop
        int softdropX = (movesToExpand.front()).getX();
        int softdropY = (movesToExpand.front()).getY() -1;
        int softdropR = (movesToExpand.front()).getR();
        if (isValidPosition(softdropX, softdropY, softdropR)) {
            int key = softdropY*100 + softdropX*10 + softdropR;
            if (dictionary.count(key) == 0) {
                Move possibleSoftdrop(softdropX, softdropY, softdropR,
                    (movesToExpand.front()).getFrames(), m_piece, (movesToExpand.front()).getHistory(), false);
                possibleSoftdrop.addFrames(1);
                possibleSoftdrop.addHistory(2);
                dictionary[key] = possibleSoftdrop;
                movesToExpand.push(possibleSoftdrop);
            }
        }
        //rotate left
        int rotateLeftXYR[3] = {0, 0, 0};
        bool rotateLeftSuccess = tryRotation((movesToExpand.front()).getX(), (movesToExpand.front()).getY(),
            (movesToExpand.front()).getR(), -1, rotateLeftXYR);
            // for (int i = 0; i < 3; i++) {
        if (rotateLeftSuccess) {
            //     cout << "(" << rotateLeftXYR[i];
            // }
            // cout << ")" << endl;
            int key = rotateLeftXYR[1]*100 + rotateLeftXYR[0]*10 + rotateLeftXYR[2];
            if (dictionary.count(key) == 0) {
                Move possibleRotateLeft(rotateLeftXYR[0], rotateLeftXYR[1], rotateLeftXYR[2],
                    (movesToExpand.front()).getFrames(), m_piece, (movesToExpand.front()).getHistory(),
                    threeCornerRule(rotateLeftXYR[0], rotateLeftXYR[1], rotateLeftXYR[2]));
                possibleRotateLeft.addFrames(1);
                possibleRotateLeft.addHistory(3);
                dictionary[key] = possibleRotateLeft;
                movesToExpand.push(possibleRotateLeft);
            }
        }
        //rotate right
        int rotateRightXYR[3] = {0, 0, 0};
        bool rotateRightSuccess = tryRotation((movesToExpand.front()).getX(), (movesToExpand.front()).getY(),
            (movesToExpand.front()).getR(), 1, rotateRightXYR);
        if (rotateRightSuccess) {
            // for (int i = 0; i < 3; i++) {
            //     cout << "(" << rotateRightXYR[i];
            // }
            // cout << ")" << endl;
            int key = rotateRightXYR[1]*100 + rotateRightXYR[0]*10 + rotateRightXYR[2];
            if (dictionary.count(key) == 0) {
                Move possibleRotateRight(rotateRightXYR[0], rotateRightXYR[1], rotateRightXYR[2],
                    (movesToExpand.front()).getFrames(), m_piece, (movesToExpand.front()).getHistory(),
                    threeCornerRule(rotateLeftXYR[0], rotateLeftXYR[1], rotateLeftXYR[2]));
                possibleRotateRight.addFrames(1);
                possibleRotateRight.addHistory(4);
                dictionary[key] = possibleRotateRight;
                movesToExpand.push(possibleRotateRight);
            }
        }

        movesToExpand.pop();
    }

    // cout << "DICT SIZE" << endl << dictionary.size() << endl;

    unordered_map<int, Move>::iterator it;
    for (it = dictionary.begin(); it != dictionary.end(); it++) {
        // cout << "(" << it->second.getX() << ", " << it->second.getY() << ", " << it->second.getR() << ")" << endl;
        if (!isFloating(it->second.getX(), it->second.getY(), it->second.getR())) {
            // cout << "BAZINGA" << endl;
            possibleMoves.push_back(it->second);
        }
    }
    // cout << "NUMBER OF 0,0,0: " << dictionary.count(0) << endl;
    return possibleMoves;
}