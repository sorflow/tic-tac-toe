// Derek Grant
// Tic-Tac-Toe
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

using namespace std;

class TicTacToe {
private:
    vector<char> board;
    char currentPlayer;
    char humanPlayer;
    char aiPlayer;
    int difficulty; // 0: Easy, 1: Medium, 2: Hard

    bool checkWin(const vector<char>& b, char player) const {
        const int winPatterns[8][3] = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
            {0, 4, 8}, {2, 4, 6}             // Diagonals
        };

        for (const auto& pattern : winPatterns) {
            if (b[pattern[0]] == player && b[pattern[1]] == player && b[pattern[2]] == player) {
                return true;
            }
        }
        return false;
    }

    bool isBoardFull(const vector<char>& b) const {
        return find(b.begin(), b.end(), ' ') == b.end();
    }

    int minimax(vector<char>& b, int depth, bool isMaximizing) {
        if (checkWin(b, aiPlayer)) return 10 - depth;
        if (checkWin(b, humanPlayer)) return depth - 10;
        if (isBoardFull(b)) return 0;

        int bestScore = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();
        char currentChar = isMaximizing ? aiPlayer : humanPlayer;

        for (int i = 0; i < 9; i++) {
            if (b[i] == ' ') {
                b[i] = currentChar;
                int score = minimax(b, depth + 1, !isMaximizing);
                b[i] = ' ';
                bestScore = isMaximizing ? max(score, bestScore) : min(score, bestScore);
            }
        }
        return bestScore;
    }

    int getBestMove() {
        int bestScore = numeric_limits<int>::min();
        int bestMove = -1;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                board[i] = aiPlayer;
                int score = minimax(board, 0, false);
                board[i] = ' ';
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = i;
                }
            }
        }
        return bestMove;
    }

    int getAIMove() {
        switch (difficulty) {
            case 0: // Easy: Random move
                return getRandomMove();
            case 1: // Medium: 50% chance of best move, 50% chance of random
                return (rand() % 2 == 0) ? getBestMove() : getRandomMove();
            case 2: // Hard: Always best move
                return getBestMove();
            default:
                return getRandomMove();
        }
    }

    int getRandomMove() {
        vector<int> availableMoves;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') availableMoves.push_back(i);
        }
        return availableMoves[rand() % availableMoves.size()];
    }

public:
    TicTacToe() : board(9, ' '), currentPlayer('X'), humanPlayer('X'), aiPlayer('O'), difficulty(0) {}

    void printBoard() const {
        cout << "\n";
        for (int i = 0; i < 9; i += 3) {
            cout << " " << board[i] << " | " << board[i+1] << " | " << board[i+2] << "\n";
            if (i < 6) cout << "---+---+---\n";
        }
        cout << "\n";
    }

    void setDifficulty(int diff) {
        difficulty = diff;
    }

    void play() {
        cout << "Welcome to Advanced Tic Tac Toe!\n";
        cout << "Choose difficulty (0: Easy, 1: Medium, 2: Hard): ";
        cin >> difficulty;

        cout << "Do you want to play as X or O? ";
        cin >> humanPlayer;
        aiPlayer = (humanPlayer == 'X') ? 'O' : 'X';
        currentPlayer = 'X';

        while (true) {
            printBoard();
            int move;
            if (currentPlayer == humanPlayer) {
                cout << "Your turn. Enter your move (1-9): ";
                cin >> move;
                move--; // Convert to 0-based index

                if (move < 0 || move >= 9 || board[move] != ' ') {
                    cout << "Invalid move. Try again.\n";
                    continue;
                }
            } else {
                cout << "AI is thinking...\n";
                move = getAIMove();
            }

            board[move] = currentPlayer;

            if (checkWin(board, currentPlayer)) {
                printBoard();
                cout << (currentPlayer == humanPlayer ? "You win!" : "AI wins!") << "\n";
                break;
            }

            if (isBoardFull(board)) {
                printBoard();
                cout << "It's a draw!\n";
                break;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
};

int main() {
    srand(time(0)); // Seed for random number generation
    TicTacToe game;
    game.play();
    return 0;
}