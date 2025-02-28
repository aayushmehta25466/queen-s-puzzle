#include <stdio.h>
#include <stdbool.h>

#define N 5 // Size of the board (5x5)

// Function to print the chessboard
void printBoard(int board[N][N]) {
    printf("\nCurrent Chessboard:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 1)
                printf("Q "); // Print Queen
            else
                printf(". "); // Print empty square
        }
        printf("\n");
    }
}

// Function to check if placing a queen at board[row][col] is safe
bool isSafe(int board[N][N], int row, int col) {
    int i, j;

    // Check the same column
    for (i = 0; i < N; i++) {
        if (board[i][col] == 1)
            return false;
    }

    // Check the same row
    for (j = 0; j < N; j++) {
        if (board[row][j] == 1)
            return false;
    }

    // Check upper-left diagonal
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1)
            return false;
    }

    // Check upper-right diagonal
    for (i = row, j = col; i >= 0 && j < N; i--, j++) {
        if (board[i][j] == 1)
            return false;
    }

    // Check lower-left diagonal
    for (i = row, j = col; i < N && j >= 0; i++, j--) {
        if (board[i][j] == 1)
            return false;
    }

    // Check lower-right diagonal
    for (i = row, j = col; i < N && j < N; i++, j++) {
        if (board[i][j] == 1)
            return false;
    }

    return true;
}

// Main program
int main() {
    int board[N][N] = {0}; // Initialize the chessboard with all squares empty
    int row, col;

    printf("Welcome to the 5-Queens Puzzle!\n");
    printf("You need to place 5 queens on a %dx%d chessboard.\n", N, N);
    printf("Rules:\n");
    printf("1. No two queens can be in the same row, column, or diagonal.\n");
    printf("2. Enter the row and column numbers (0 to %d) for each queen.\n\n", N - 1);

    // Input coordinates for 5 queens
    for (int i = 0; i < N; i++) {
        printf("Enter coordinates for Queen %d (row and column): ", i + 1);
        scanf("%d %d", &row, &col);

        // Check if the input is valid
        if (row < 0 || row >= N || col < 0 || col >= N) {
            printf("Invalid input! Row and column must be between 0 and %d. Try again.\n", N - 1);
            i--; // Ask for the same queen's position again
            continue;
        }

        // Check if the placement is safe
        if (isSafe(board, row, col)) {
            board[row][col] = 1; // Place the queen
            printBoard(board);   // Display the board
        } else {
            printf("Invalid placement! Another queen is attacking this position. Try again.\n");
            i--; // Ask for the same queen's position again
        }
    }

    // Final check
    printf("\nAll queens placed successfully! Here's your final chessboard:\n");
    printBoard(board);

    return 0;
}
