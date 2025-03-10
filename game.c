#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

// Define the size of the board
#define N 5
const int width = 900, height = 600;

// Board to track queen placements
int board[N][N] = {0};
int queenCount = 0;
int gameOver = 0;

// SDL variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *cellTexture = NULL;
SDL_Texture *queenTexture = NULL;
SDL_Texture *winTexture = NULL; // Move winTexture declaration here
TTF_Font *font = NULL;

/**
 * Function to check if placing a queen at (row, col) is safe.
 * @param row The row index.
 * @param col The column index.
 * @return True if it is safe to place a queen, false otherwise.
 */
bool isSafe(int row, int col)
{
    int i, j;

    // Check the same column
    for (i = 0; i < N; i++)
    {
        if (board[i][col] == 1)
            return false;
    }

    // Check the same row
    for (j = 0; j < N; j++)
    {
        if (board[row][j] == 1)
            return false;
    }

    // Check upper-left diagonal
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == 1)
            return false;
    }

    // Check upper-right diagonal
    for (i = row, j = col; i >= 0 && j < N; i--, j++)
    {
        if (board[i][j] == 1)
            return false;
    }

    // Check lower-left diagonal
    for (i = row, j = col; i < N && j >= 0; i++, j--)
    {
        if (board[i][j] == 1)
            return false;
    }

    // Check lower-right diagonal
    for (i = row, j = col; i < N && j < N; i++, j++)
    {
        if (board[i][j] == 1)
            return false;
    }

    return true;
}

/**
 * Function to render text on the screen.
 * @param text The text to render.
 * @param x The x-coordinate of the text.
 * @param y The y-coordinate of the text.
 * @param color The color of the text.
 */
void renderText(const char *text, int x, int y, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = {x - surface->w / 2, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture); // Free the texture after rendering
}

/**
 * Function to reset the game board.
 */
void resetGame()
{
    // Resetting variables
    queenCount = 0;
    gameOver = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i][j] = 0;
        }
    }
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 10, 20, 40, 255);
    SDL_RenderClear(renderer);
    // Show information on the screen
    SDL_Color white = {255, 255, 255, 255};
    renderText("Loading Game ...", width / 2, height / 2, white);
    SDL_RenderPresent(renderer); // Force immediate screen update
    SDL_Delay(300);
}
void gameWin(int x, int y, int width, int height)
{
    if (!winTexture)
    {
        winTexture = IMG_LoadTexture(renderer, "assets/game_win.png");
        if (!winTexture)
        {
            printf("Image Loading Failed: %s\n", IMG_GetError());
        }
    }
    SDL_Rect dstRect = {x, y, (N * width), (N * height)};
    SDL_RenderCopy(renderer, winTexture, NULL, &dstRect);
}

int main(int argc, char *argv[])
{
    // Initialize SDL, TTF, and IMG libraries
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create SDL window and renderer
    window = SDL_CreateWindow("5x5 Queen's Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load font
    font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    // Load textures
    cellTexture = IMG_LoadTexture(renderer, "assets/cell.png");
    queenTexture = IMG_LoadTexture(renderer, "assets/queen.png");
    if (!cellTexture || !queenTexture)
    {
        printf("Image Loading Failed: %s\n", IMG_GetError());
        return 1;
    }

    // Calculate cell dimensions and board start position
    int newWidth = width / 8;
    int newHeight = height / 8;
    int startX = (width - N * newWidth) / 2;
    int startY = (height - N * newHeight) / 2;

    SDL_Event event;
    int running = 1;

    // Define reset button dimensions and position
    int buttonWidth = 120, buttonHeight = 40;
    SDL_Rect resetButton = {width / 2 - buttonWidth / 2, height - 60, buttonWidth, buttonHeight};

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if clicking on the reset button
                if (x >= resetButton.x && x <= resetButton.x + resetButton.w &&
                    y >= resetButton.y && y <= resetButton.y + resetButton.h)
                {
                    resetGame();
                }
                // Check if clicking on the board to place a queen
                else if (queenCount < N && !gameOver)
                {
                    int col = (x - startX) / newWidth;
                    int row = (y - startY) / newHeight;
                    if (row >= 0 && row < N && col >= 0 && col < N && board[row][col] == 0 && isSafe(row, col))
                    {
                        board[row][col] = 1;
                        queenCount++;
                    }
                }
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 10, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Render the board and queens if the game is not over
        if (!gameOver)
        {
            for (int row = 0; row < N; row++)
            {
                for (int col = 0; col < N; col++)
                {
                    SDL_Rect dstRect = {startX + col * newWidth, startY + row * newHeight, newWidth, newHeight};
                    SDL_RenderCopy(renderer, cellTexture, NULL, &dstRect);
                    if (board[row][col] == 1)
                    {
                        SDL_RenderCopy(renderer, queenTexture, NULL, &dstRect);
                    }
                }
            }

            // Draw text "Queens Placed: X/5"
            SDL_Color white = {255, 255, 255, 255};
            char message[50];
            snprintf(message, 50, "Queens Placed: %d / 5", queenCount);
            renderText(message, width / 2, height - 100, white);

            // Draw Reset Button
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &resetButton);
            renderText("Reset", resetButton.x + resetButton.w / 2, resetButton.y + 10, white);
            SDL_RenderPresent(renderer);

            // Check if all queens are placed
            if (queenCount == N)
            {
                gameOver = 1; // Set the game over flag
                SDL_Delay(300);
            }
        }
        else if (gameOver)
        {
            // Render the win texture and reset button only once
            SDL_Color green = {0, 255, 0, 255};
            SDL_Color white = {255, 255, 255, 255};
            renderText("! Game Over - You Won !", width / 2, startY - 40, green);
            gameWin(startX, startY, newWidth, newHeight);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &resetButton);
            renderText("Reset", resetButton.x + resetButton.w / 2, resetButton.y + 10, white);
            SDL_RenderPresent(renderer);
        }
    }

    // Free resources and quit SDL subsystems
    SDL_DestroyTexture(cellTexture);
    SDL_DestroyTexture(queenTexture);
    SDL_DestroyTexture(winTexture); // Free the win texture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
