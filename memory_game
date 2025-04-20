#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std;
using Clock = chrono::steady_clock;

// Grid dimensions (must be even total)
static const int ROWS = 4;
static const int COLS = 4;

// Pause (in milliseconds) after showing a failed match
static const int PAUSE_MS = 1500;

// Utility to clear the screen 
void clearScreen() {
    #ifdef _WIN32
      system("cls");
    #else
      system("clear");
    #endif
}

// Print the current board, showing revealed cards and hiding others
void printBoard(const vector<vector <string> > & board,
                const vector<vector <bool> > & revealed)
{
    cout << "    ";
    for(int c = 0; c < COLS; c++)
        cout << setw(4) << c;
    cout << "\n   +" << string(COLS*4, '-') << "+\n";

    for(int r = 0; r < ROWS; r++) {
        cout << setw(2) << r << " |";
        for(int c = 0; c < COLS; c++) {
            if (revealed[r][c]) {
                cout << setw(4) << board[r][c];
            } else {
                cout << setw(4) << "*";
            }
        }
        cout << " |\n";
    }
    cout << "   +" << string(COLS*4, '-') << "+\n";
}
