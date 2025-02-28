#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define N 5 // Board size
const int width = 900, height = 600;

// Board to track queen placements
int board[N][N] = {0};
int queenCount = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *cellTexture = NULL;
SDL_Texture *queenTexture = NULL;
TTF_Font *font = NULL;

// Function to check if placing a queen at (row, col) is safe
bool isSafe(int row, int col) {
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

// Function to render text
void renderText(const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = {x - surface->w / 2, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Reset the board
void resetGame() {
    queenCount = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("5x5 Queen's Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    cellTexture = IMG_LoadTexture(renderer, "assets/cell.png");
    queenTexture = IMG_LoadTexture(renderer, "assets/queen.png");
    if (!cellTexture || !queenTexture) {
        printf("Image Loading Failed: %s\n", IMG_GetError());
        return 1;
    }

    int newWidth = width / 8;
    int newHeight = height / 8;
    int startX = (width - N * newWidth) / 2;
    int startY = (height - N * newHeight) / 2;

    SDL_Event event;
    int running = 1;

    // Reset Button
    int buttonWidth = 120, buttonHeight = 40;
    SDL_Rect resetButton = {width / 2 - buttonWidth / 2, height - 60, buttonWidth, buttonHeight}; // Centered reset button

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if clicking on the reset button
                if (x >= resetButton.x && x <= resetButton.x + resetButton.w &&
                    y >= resetButton.y && y <= resetButton.y + resetButton.h) {
                    resetGame();
                }
                else if (queenCount < N) {
                    int col = (x - startX) / newWidth;
                    int row = (y - startY) / newHeight;
                    if (row >= 0 && row < N && col >= 0 && col < N && board[row][col] == 0 && isSafe(row, col)) {
                        board[row][col] = 1;
                        queenCount++;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 10, 20, 40, 255);
        SDL_RenderClear(renderer);

        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                SDL_Rect dstRect = {startX + col * newWidth, startY + row * newHeight, newWidth, newHeight};
                SDL_RenderCopy(renderer, cellTexture, NULL, &dstRect);
                if (board[row][col] == 1) {
                    SDL_RenderCopy(renderer, queenTexture, NULL, &dstRect);
                }
            }
        }

        // Draw text "Queens Placed: X/5"
        SDL_Color white = {255, 255, 255, 255};
        char message[50];
        snprintf(message, 50, "Queens Placed: %d / 5", queenCount);
        renderText(message, width / 2, height - 100, white); // Centered text

        // Draw Reset Button
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &resetButton);
        renderText("Reset", resetButton.x + resetButton.w / 2, resetButton.y + 10, white); // Center text in button

        // Show "All Queens Placed!" message when all 5 queens are placed
        if (queenCount == N) {
            SDL_Color green = {0, 255, 0, 255};
            renderText("All Queens Placed!", width / 2, startY - 40, green); // Display at top of board
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(cellTexture);
    SDL_DestroyTexture(queenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
