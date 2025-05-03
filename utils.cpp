#include "utils.h"
#include "fruitpool.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>
using namespace std;

// Clears the console screen, using platform-specific commands for Windows or Unix-based systems.
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// 1. Game Board Generation
// Find the longest word in fruitpool for block width and account for color codes
// The inputs are:
//   - board (const vector<vector<string>>&): 2D vector representing the game board with fruit names.
//   - revealed (const vector<vector<bool>>&): 2D vector indicating which cards are revealed.
//   - rows (int): Number of rows in the board.
//   - cols (int): Number of columns in the board.
// Outputs the formatted game board to the console.

void printBoard(const vector<vector<string>>& board, const vector<vector<bool>>& revealed, int rows, int cols) {
    size_t maxLength = 0;
    for (const auto& fruit : fruitNames) {
        maxLength = max(maxLength, fruit.length());
    }
    
    // Set block width (add 2 padding on each side for centering)
    
    const int blockWidth = maxLength + 4;  
    
    // Print column numbers
    
    cout << "    ";
    for (int c = 0; c < cols; c++) {
        string colNum = to_string(c);
        int padding = (blockWidth - colNum.length()) / 2;
        cout << string(padding, ' ') << colNum << string(blockWidth - padding - colNum.length(), ' ');
    }
    cout << "\n   +" << string(cols * blockWidth, '-') << "+\n";

    // Print board
    // Use actual content length for padding calculation
    
    for (int r = 0; r < rows; r++) {
        cout << setw(2) << r << " |";
        for (int c = 0; c < cols; c++) {
            string displayContent;
            string content;
            if (revealed[r][c]) {
                displayContent = fruitColors[board[r][c]] + board[r][c] + "\033[0m";
                content = board[r][c];  
            } else {
                displayContent = "*";
                content = "*";
            }
            
            // Calculate padding based on actual content length, not including color codes
            
            int padding = (blockWidth - content.length()) / 2;
            cout << string(padding, ' ') << displayContent << string(blockWidth - padding - content.length(), ' ');
        }
        cout << " |\n";
    }
    cout << "   +" << string(cols * blockWidth, '-') << "+\n";
}

// 2. Save Game Function
// inputs are: 
//   - board (const vector<vector<string>>&): 2D vector representing the game board.
//   - revealed (const vector<vector<bool>>&): 2D vector indicating which cards are revealed.
//   - deck (const vector<string>&): Vector of fruits in the deck.
//   - fruitPool (const vector<string>&): Vector of all possible fruit names.
//   - rows (int): Number of rows in the board.
//   - cols (int): Number of columns in the board.
//   - pairsFound (int): Number of pairs found so far.
//   - totalPairs (int): Total number of pairs in the game.
//   - elapsedTime (double): Time elapsed in seconds.
//   - isFirstCardFlipped (bool): Whether the first card of a pair is flipped.
//   - firstFlippedCard (pair<int, int>): Coordinates of the first flipped card, if applicable.
// Writes the game state to "game_save.txt" and outputs a success or error message to the console.

void saveGame(const vector<vector<string>>& board,
              const vector<vector<bool>>& revealed,
              const vector<string>& deck,
              const vector<string>& fruitPool,
              int rows, int cols, int pairsFound, int totalPairs,
              double elapsedTime,
              bool isFirstCardFlipped,
              pair<int, int> firstFlippedCard) {
    ofstream outFile("game_save.txt");
    if (!outFile) {
        cout << "Error: Could not save game." << endl;
        return;
    }

    // Write rows and cols (timelimit removed)
    
    outFile << rows << " " << cols << "\n";
    outFile << pairsFound << " " << totalPairs << " " << elapsedTime << "\n";

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int idx = find(fruitPool.begin(), fruitPool.end(), board[r][c]) - fruitPool.begin();
            outFile << idx << " ";
        }
        outFile << "\n";
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            outFile << (revealed[r][c] ? 1 : 0) << " ";
        }
        outFile << "\n";
    }

    outFile << deck.size() << "\n";
    for (auto& card : deck) {
        int idx = find(fruitPool.begin(), fruitPool.end(), card) - fruitPool.begin();
        outFile << idx << " ";
    }
    outFile << "\n";

    outFile << fruitPool.size() << "\n";
    for (size_t i = 0; i < fruitPool.size(); i++) {
        outFile << i << " " << fruitPool[i] << "\n";
    }

    outFile << (isFirstCardFlipped ? 1 : 0) << " ";
    if (isFirstCardFlipped) {
        outFile << firstFlippedCard.first << " " << firstFlippedCard.second << "\n";
    } else {
        outFile << "\n";
    }

    outFile.close();
    cout << "Game saved successfully.\n";
}


// 3. Load Game Function
// inputs are: 
//   - board (vector<vector<string>>&): 2D vector to store the game board.
//   - revealed (vector<vector<bool>>&): 2D vector to store which cards are revealed.
//   - deck (vector<string>&): Vector to store the deck of fruits.
//   - fruitPool (vector<string>&): Vector to store all possible fruit names.
//   - rows (int&): Number of rows in the board.
//   - cols (int&): Number of columns in the board.
//   - pairsFound (int&): Number of pairs found so far.
//   - totalPairs (int&): Total number of pairs in the game.
//   - elapsedTime (double&): Time elapsed in seconds.
//   - isFirstCardFlipped (bool&): Whether the first card of a pair is flipped.
//   - firstFlippedCard (pair<int, int>&): Coordinates of the first flipped card, if applicable.
// Returns true if the game state was loaded successfully, false otherwise (e.g., file not found or corrupted).

