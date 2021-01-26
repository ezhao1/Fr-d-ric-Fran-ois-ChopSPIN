/******************************************************************************
Main run file for the Guideline Tetris AI. First command line input is the number of pieces
to place, and the second (0 or 1) is whether to animate placing the pieces or not.

*******************************************************************************/

#include <iostream>
#include "AI.h"
#include <Windows.h>
#include<TlHelp32.h>
#include <tchar.h> // _tcscmps
#include <algorithm>
#include <cmath>
#include <queue>
#include <cstdlib>
#include "Timer.h"

using namespace std;

BYTE* GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    BYTE* dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
 
    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32
 
 
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

void getInformationFromGame(int* arrayOfInformation, HANDLE processHandle, BYTE* gameBaseAddress) {
    //X
    DWORD X_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> X_pointsOffsets {0x28, 0x30, 0x140, 0x88, 0x78, 0x3C8, 0xC};
    DWORD X_baseAddress = NULL;
    //Get value at gamebase+offset -> store it in baseAddress
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ X_offsetGameToBaseAddress), &X_baseAddress, sizeof(X_baseAddress), NULL);
    DWORD X_pointsAddress = X_baseAddress;
    for (int i = 0; i < X_pointsOffsets.size() - 1; i++) // -1 because we dont want the value at the last offset
    {
        ReadProcessMemory(processHandle, (LPVOID)(X_pointsAddress + X_pointsOffsets.at(i)), &X_pointsAddress, sizeof(X_pointsAddress), NULL);
    }
    X_pointsAddress += X_pointsOffsets.at(X_pointsOffsets.size() - 1); //Add Last offset -> done!!

    //Y
    DWORD Y_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> Y_pointsOffsets = {0x28, 0x30, 0x180, 0x170, 0x48, 0x3C8, 0x10};
    DWORD Y_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ Y_offsetGameToBaseAddress), &Y_baseAddress, sizeof(Y_baseAddress), NULL);
    DWORD Y_pointsAddress = Y_baseAddress;
    for (int i = 0; i < Y_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(Y_pointsAddress + Y_pointsOffsets.at(i)), &Y_pointsAddress, sizeof(Y_pointsAddress), NULL);
    }
    Y_pointsAddress += Y_pointsOffsets.at(Y_pointsOffsets.size() - 1);

    //R
    DWORD R_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> R_pointsOffsets = {0x28, 0x30, 0x140, 0x48, 0x3C8, 0x30, 0x48};
    DWORD R_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ R_offsetGameToBaseAddress), &R_baseAddress, sizeof(R_baseAddress), NULL);
    DWORD R_pointsAddress = R_baseAddress;
    for (int i = 0; i < R_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(R_pointsAddress + R_pointsOffsets.at(i)), &R_pointsAddress, sizeof(R_pointsAddress), NULL);
    }
    R_pointsAddress += R_pointsOffsets.at(R_pointsOffsets.size() - 1);

    //H1
    DWORD H1_offsetGameToBaseAddress = 0x00460690;
    vector<DWORD> H1_pointsOffsets = {0x2A8, 0x38, 0x78, 0xB0, 0x20, 0xB8, 0x15C};
    DWORD H1_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ H1_offsetGameToBaseAddress), &H1_baseAddress, sizeof(H1_baseAddress), NULL);
    DWORD H1_pointsAddress = H1_baseAddress;
    for (int i = 0; i < H1_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(H1_pointsAddress + H1_pointsOffsets.at(i)), &H1_pointsAddress, sizeof(H1_pointsAddress), NULL);
    }
    H1_pointsAddress += H1_pointsOffsets.at(H1_pointsOffsets.size() - 1);

    //H2
    DWORD H2_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> H2_pointsOffsets = {0x28, 0x30, 0x140, 0x48, 0x78, 0xB8, 0x160};
    DWORD H2_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ H2_offsetGameToBaseAddress), &H2_baseAddress, sizeof(H2_baseAddress), NULL);
    DWORD H2_pointsAddress = H2_baseAddress;
    for (int i = 0; i < H2_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(H2_pointsAddress + H2_pointsOffsets.at(i)), &H2_pointsAddress, sizeof(H2_pointsAddress), NULL);
    }
    H2_pointsAddress += H2_pointsOffsets.at(H2_pointsOffsets.size() - 1);

    //H3
    DWORD H3_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> H3_pointsOffsets = {0x38, 0x78, 0xB0, 0x20, 0xB8, 0x188, 0x1B4};
    DWORD H3_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ H3_offsetGameToBaseAddress), &H3_baseAddress, sizeof(H3_baseAddress), NULL);
    DWORD H3_pointsAddress = H3_baseAddress;
    for (int i = 0; i < H3_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(H3_pointsAddress + H3_pointsOffsets.at(i)), &H3_pointsAddress, sizeof(H3_pointsAddress), NULL);
    }
    H3_pointsAddress += H3_pointsOffsets.at(H3_pointsOffsets.size() - 1);

    //H4
    DWORD H4_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> H4_pointsOffsets = {0x38, 0x78, 0xB0, 0x8, 0x20, 0xB8, 0x168};
    DWORD H4_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ H4_offsetGameToBaseAddress), &H4_baseAddress, sizeof(H4_baseAddress), NULL);
    DWORD H4_pointsAddress = H4_baseAddress;
    for (int i = 0; i < H4_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(H4_pointsAddress + H4_pointsOffsets.at(i)), &H4_pointsAddress, sizeof(H4_pointsAddress), NULL);
    }
    H4_pointsAddress += H4_pointsOffsets.at(H4_pointsOffsets.size() - 1);

    //H5
    DWORD H5_offsetGameToBaseAddress = 0x00598A20;
    vector<DWORD> H5_pointsOffsets = {0x38, 0x78, 0xB0, 0x10, 0x10, 0x78, 0x16C};
    DWORD H5_baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ H5_offsetGameToBaseAddress), &H5_baseAddress, sizeof(H5_baseAddress), NULL);
    DWORD H5_pointsAddress = H5_baseAddress;
    for (int i = 0; i < H5_pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(H5_pointsAddress + H5_pointsOffsets.at(i)), &H5_pointsAddress, sizeof(H5_pointsAddress), NULL);
    }
    H5_pointsAddress += H5_pointsOffsets.at(H5_pointsOffsets.size() - 1);

    //CURRENT PIECE
    DWORD CURR_offsetGameToBaseAddress = 0x00461B20;
    vector<DWORD> CURR_pointsOffsets {0x378, 0x28, 0x20, 0xA8, 0x3C8, 0x8};
    DWORD CURR_baseAddress = NULL;
    //Get value at gamebase+offset -> store it in baseAddress
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress+ CURR_offsetGameToBaseAddress), &CURR_baseAddress, sizeof(CURR_baseAddress), NULL);
    DWORD CURR_pointsAddress = CURR_baseAddress;
    for (int i = 0; i < CURR_pointsOffsets.size() - 1; i++) // -1 because we dont want the value at the last offset
    {
        ReadProcessMemory(processHandle, (LPVOID)(CURR_pointsAddress + CURR_pointsOffsets.at(i)), &CURR_pointsAddress, sizeof(CURR_pointsAddress), NULL);
    }
    CURR_pointsAddress += CURR_pointsOffsets.at(CURR_pointsOffsets.size() - 1); //Add Last offset -> done!!


    int X_buf;
    ReadProcessMemory( processHandle, (LPVOID)(X_pointsAddress), &X_buf, sizeof(X_buf), NULL);
    arrayOfInformation[0] = X_buf;

    int Y_buf;
    ReadProcessMemory( processHandle, (LPVOID)(Y_pointsAddress), &Y_buf, sizeof(Y_buf), NULL);
    arrayOfInformation[1] = Y_buf;

    int R_buf;
    ReadProcessMemory( processHandle, (LPVOID)(R_pointsAddress), &R_buf, sizeof(R_buf), NULL);
    arrayOfInformation[2] = R_buf;

    int H1_buf;
    ReadProcessMemory( processHandle, (LPVOID)(H1_pointsAddress), &H1_buf, sizeof(H1_buf), NULL);
    arrayOfInformation[3] = H1_buf;

    int H2_buf;
    ReadProcessMemory( processHandle, (LPVOID)(H2_pointsAddress), &H2_buf, sizeof(H2_buf), NULL);
    arrayOfInformation[4] = H2_buf;

    int H3_buf;
    ReadProcessMemory( processHandle, (LPVOID)(H3_pointsAddress), &H3_buf, sizeof(H3_buf), NULL);
    arrayOfInformation[5] = H3_buf;

    int H4_buf;
    ReadProcessMemory( processHandle, (LPVOID)(H4_pointsAddress), &H4_buf, sizeof(H4_buf), NULL);
    arrayOfInformation[6] = H4_buf;

    int H5_buf;
    ReadProcessMemory( processHandle, (LPVOID)(H5_pointsAddress), &H5_buf, sizeof(H5_buf), NULL);
    arrayOfInformation[7] = H5_buf;

    int CURR_buf;
    ReadProcessMemory( processHandle, (LPVOID)(CURR_pointsAddress), &CURR_buf, sizeof(CURR_buf), NULL);
    arrayOfInformation[8] = CURR_buf;
    
}

void doMove(Move move) {
    cout << "DOING MOVE: " << endl;
    vector<int> history = move.getHistory();
    for (int i = 0; i < history.size(); i++) {
        cout << history[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < history.size(); i++) {
        if (history[i] == 0) {
            //move left
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VkKeyScanA('a');
            SendInput(1, &input, sizeof(input));
            Sleep(100);
            ZeroMemory(&input, sizeof(input));
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            Sleep(5);
        }
        else if (history[i] == 1) {
            //move right
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VkKeyScanA('d');
            SendInput(1, &input, sizeof(input));
            Sleep(100);
            ZeroMemory(&input, sizeof(input));
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            Sleep(5);
        }
        else if (history[i] == 2) {
            //softdrop
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VkKeyScanA('s');
            SendInput(1, &input, sizeof(input));
            Sleep(150);
            ZeroMemory(&input, sizeof(input));
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            Sleep(5);
        }
        else if (history[i] == 3) {
            //rotate left
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VK_LEFT;
            SendInput(1, &input, sizeof(input));
            Sleep(100);
            ZeroMemory(&input, sizeof(input));
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            Sleep(5);
        }
        else if (history[i] == 4) {
            //rotate right
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VK_RIGHT;
            SendInput(1, &input, sizeof(input));
            Sleep(100);
            ZeroMemory(&input, sizeof(input));
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            Sleep(5);
        }
    }
}

TetrisBoard updateBoardAfterMoveMain(Move move, TetrisBoard board, pair<int, int>* lines) {
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

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "usage: ./main [pieces] [animated]" << endl;
        exit(0);
    }
    int pieces = atoi( argv[1]);
    bool animate = false;
    if (*argv[2] == '1') {
        animate = true;
    }
    
    vector<int> pieceQueue;
    int numberToGenerate = pieces + 7;
    while (pieceQueue.size() < numberToGenerate) {
        vector<int> sevenBag;
        for (int i = 0; i < 7; i++) {
            //pushing one of each piece
            sevenBag.push_back(i);
        }

        //randomize within bag
        srand(time(0));
        random_shuffle(sevenBag.begin(), sevenBag.end());

        //add to queue
        for (int i = 0; i < 7; i++) {
            pieceQueue.push_back(sevenBag[i]);
        }
    }

    int totalSends = 0;
    TetrisBoard board = TetrisBoard();
    for (int turn = 0; turn < pieces; turn ++) {
        //no hold taken into account
        int currentPiece = pieceQueue[turn];
        int x = 4;
        int y = 19;
        int r = 0;
        int queue[5] = {pieceQueue[turn + 1], pieceQueue[turn + 2], pieceQueue[turn + 3], pieceQueue[turn + 4], pieceQueue[turn + 5]};

        //print board with inputted piece at top for visual purposes
        if (animate) {
            Move visualMove(x, y, r, 0, currentPiece, {}, false);
            pair<int, int> lines;
            TetrisBoard boardCopy = updateBoardAfterMoveMain(visualMove, board, &lines);
            boardCopy.printBoard();
            cout << "TURN: " << turn << ", TOTAL SENDS: " << totalSends << endl << endl;
        }
        

        int sends;
        AI ai(board, 5, 15);
        // cout << currentPiece << ", ";
        // for (int i = 0; i < 5; i++) {
        //     cout << queue[i] << ", ";
        // }
        // cout << endl;
        pair<Move, TetrisBoard> result = ai.GreedyBFS(currentPiece, x, y, r, queue, &sends);
        board = result.second;
        totalSends += sends;
        if (animate) {
            Sleep(400);
        }
        else {
            if (turn % 10 == 0) {
                cout << "TURN " << turn << ", TOTAL SENDS: " << totalSends << endl;
            }
            
        }
    }

    cout << "TOTAL SENDS: " << totalSends << endl;


    return 0;
}

